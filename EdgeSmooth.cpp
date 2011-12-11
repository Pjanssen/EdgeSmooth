#include "EdgeSmooth.h"
#include "iepoly.h"
#include "mnmath.h"
#include "mnmesh.h"
#include "polyobj.h"
#include "modstack.h"
#include "iEPolyMod.h"

#define HARD_EDGE_FLAG MN_USER



MNMesh* _get_mesh(INode* node)
{
	Object* baseObj = node->GetObjectRef()->FindBaseObject();
	if (baseObj == NULL)
		throw MAXException("Could not find base object.");

	EPoly* poly = (EPoly*)baseObj->GetInterface(EPOLY_INTERFACE);
	if (poly == NULL)
		throw MAXException("Could not get EPoly interface.");

	return poly->GetMeshPtr();
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



void _resmooth(MNMesh* mesh, BitArray edges, bool makeSoft)
{
	mesh->ClearEFlags(HARD_EDGE_FLAG);
	
	//Flag all edges that are currently hard.
	_flagHardEdges(mesh, HARD_EDGE_FLAG);

	//Add/Remove flags from edges that should be smoothed/unsmoothed.
	_addFlagHardEdges(mesh, edges, HARD_EDGE_FLAG, !makeSoft);

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



BOOL EdgeSmooth::IsSoft(INode* node, BitArray* edges) 
{
	MNMesh* mesh = _get_mesh(node);
	return _all(mesh, *edges, _isSoft);
}


BOOL EdgeSmooth::IsHard(INode* node, BitArray* edges) 
{
	MNMesh* mesh = _get_mesh(node);
	return _all(mesh, *edges, _isHard);
}




void EdgeSmooth::Apply(bool makeSoft) 
{
	Interface* ip = GetCOREInterface();
	
	//Check selection count.
	int selCount = ip->GetSelNodeCount();
	if (selCount == 0)
		throw MAXException("Selection set empty");
	if (selCount > 1)
		throw MAXException("EdgeSmooth can only be applied to a single object");

	//Get edge selection.
	INode* selNode = ip->GetSelNode(0);
	BitArray* edgeSel = new BitArray();
	MNMesh* mesh = _get_mesh(selNode);
	mesh->getEdgeSel(*edgeSel);
	
	Apply(makeSoft, selNode, edgeSel);
}

void EdgeSmooth::Apply(bool makeSoft, INode* node, BitArray* edges) 
{
	MNMesh* mesh =  _get_mesh(node);
	int numSet = edges->NumberSet();

	if (numSet == mesh->ENum())
		mesh->Resmooth(makeSoft);
	else
		_resmooth(mesh, *edges, makeSoft);

	_redraw(node, mesh);
}

void EdgeSmooth::ApplyFP(bool makeSoft, INode* node, BitArray* edges) 
{
	if (node == NULL && edges == NULL)
		Apply(makeSoft);
	else
		Apply(makeSoft, node, edges);
}