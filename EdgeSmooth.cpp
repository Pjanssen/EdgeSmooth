#include "EdgeSmooth.h"
#include "iepoly.h"
#include "mnmath.h"
#include "mnmesh.h"

#define HARD_EDGE_FLAG MN_USER

//Gets the EPoly* from an INode*.
//Throws an exception if the inode is not an epoly.
EPoly* _get_epoly(INode* node)
{
	BaseInterface* ip;
	Object* obj = node->GetObjectRef();
	if ((ip = obj->GetInterface(EPOLY_INTERFACE)) == NULL)
		throw MAXException("Unable to get EPoly interface from node.", 0);
	
	return (EPoly*)ip;
}




//Returns true if the edge is one-sided (i.e. it has only one face).
bool _isOneSidedEdge(MNMesh* mesh, int e)
{
	return mesh->E(e)->f2 == -1;
}



//Returns true if the faces on the edge share one or more smoothing groups.
//Note: returns false if the edge is one-sided.
bool _isSoft(MNMesh* mesh, int e)
{
	if (e < 0 || e >= mesh->ENum())
		throw MAXException("EPoly edge index out of range");

	if (_isOneSidedEdge(mesh, e))
		return false;
	else
	{
		MNEdge* edge = mesh->E(e);
		return (mesh->F(edge->f1)->smGroup & mesh->F(edge->f2)->smGroup) != 0;
	}
}



//Returns true if the faces on the edge don't share any smoothing groups.
//Note: returns true if the edge is one-sided.
bool _isHard(MNMesh* mesh, int e)
{
	return !_isSoft(mesh, e);
}


typedef bool (*indexedMeshOp)(MNMesh*, int);

// Applies a function to each x that is set in the xs parameter,
// and returns true if none of these function calls return false.
bool _all(MNMesh* mesh, BitArray xs, indexedMeshOp f)
{
	int xsSize = xs.GetSize();
	for (int x = 0; x < xsSize; x++)
	{
		if (xs[x] && !f(mesh, x))
			return false;
	}

	return true;
}


void _flagHardEdges(MNMesh* mesh, DWORD flag)
{
	int edgeCount = mesh->ENum();
	for (int e = 0; e < edgeCount; e++)
	{
		if (_isHard(mesh, e) && !_isOneSidedEdge(mesh, e))
			mesh->E(e)->SetFlag(flag, true);
	}
}

void _addFlagHardEdges(MNMesh* mesh, BitArray edges, DWORD flag, bool val)
{
	int edgesSize = edges.GetSize();
	for (int e = 0; e < edgesSize; e++)
	{
		if (edges[e] && !_isOneSidedEdge(mesh, e)) 
			mesh->E(e)->SetFlag(HARD_EDGE_FLAG, val);
	}
}



void _resmooth(MNMesh* mesh, BitArray edges, bool makeHard)
{
	mesh->ClearEFlags(HARD_EDGE_FLAG);
	
	//Flag all edges that are currently hard.
	_flagHardEdges(mesh, HARD_EDGE_FLAG);

	//Add/Remove flags from edges that should be smoothed/unsmoothed.
	_addFlagHardEdges(mesh, edges, HARD_EDGE_FLAG, makeHard);

	//Smooth the mesh.
	mesh->SmoothByCreases(HARD_EDGE_FLAG);

	//TODO clean up unneeded smoothing groups
	//I.e. remove smoothing groups which have no use because
	//all its edges are hard.

	mesh->ClearEFlags(HARD_EDGE_FLAG);
}


void _redraw(INode* node, MNMesh* mesh)
{
	mesh->InvalidateTopoCache(false);
	node->NotifyDependents(FOREVER, PART_TOPO, REFMSG_CHANGE);
	Interface* ip = GetCOREInterface();
	ip->RedrawViews(ip->GetTime());
}



BOOL IsSoft(INode* node, BitArray* edges) 
{
	EPoly* poly = _get_epoly(node);
	return _all(poly->GetMeshPtr(), *edges, _isSoft);
}


BOOL IsHard(INode* node, BitArray* edges) 
{
	EPoly* poly = _get_epoly(node);
	return _all(poly->GetMeshPtr(), *edges, _isHard);
}


BOOL MakeSoft(INode* node, BitArray* edges) 
{ 
	EPoly* poly = _get_epoly(node);
	MNMesh* mesh = poly->GetMeshPtr();
	int numSet = edges->NumberSet();

	if (numSet == mesh->ENum())
		mesh->Resmooth(true);
	else
		_resmooth(mesh, *edges, false);

	_redraw(node, mesh);

	return true;
}


BOOL MakeHard(INode* node, BitArray* edges) 
{
	EPoly* poly = _get_epoly(node);
	MNMesh* mesh = poly->GetMeshPtr();
	int numSet = edges->NumberSet();

	if (numSet == mesh->ENum())
		mesh->Resmooth(false);
	else
		_resmooth(mesh, *edges, true);

	_redraw(node, mesh);

	return true;
}