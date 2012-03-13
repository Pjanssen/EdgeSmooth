#include "EdgeSmooth.h"
#include "iepoly.h"
#include "mnmath.h"
#include "mnmesh.h"
#include "polyobj.h"
#include "modstack.h"
#include "iEPolyMod.h"

#define HARD_EDGE_FLAG MN_USER


EPoly* _get_epoly(INode* node)
{
	if (node)
		return (EPoly*)node->GetObjectRef()->FindBaseObject()->GetInterface(EPOLY_INTERFACE);

	return NULL;
}



MNMesh* _get_mesh(INode* node)
{
	EPoly* poly = _get_epoly(node);
	if (poly)
		return poly->GetMeshPtr();
	
	return NULL;
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
		throw MAXException(_M("EPoly edge index out of range"));

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
	mesh->FenceSmGroups();
	mesh->PropegateComponentFlags(MNM_SL_EDGE, HARD_EDGE_FLAG, MNM_SL_EDGE, MN_EDGE_NOCROSS);
	mesh->ClearEFlags(MN_EDGE_NOCROSS);
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



//================================================
// IsSoft / IsHard implementation
//================================================
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




//================================================
// CanApplyToSelection implementation
//================================================
BOOL EdgeSmooth::CanApplyToSel()
{
	Interface* ip = GetCOREInterface();
	
	// Check selection count.
	if (ip->GetSelNodeCount() != 1)
		return false;

	Object* selObj = ip->GetSelNode(0)->GetObjectRef()->FindBaseObject();

	// Check that selection is EPoly.
	if (selObj->ClassID() != EPOLYOBJ_CLASS_ID)
		return false;

	// Verify that we're editing the base object, not a modifier.
	if (ip->GetCurEditObject() != selObj)
		return false;

	// Check if sub-object level is 'edge'.
	if (selObj->GetSubselState() != MNM_SL_EDGE)
		return false;

	return true;
}


//================================================
// Apply implementation
//================================================
void apply(bool makeSoft, INode* node, BitArray* edges, bool undoable)
{
	EPoly* poly = _get_epoly(node);
	MNMesh* mesh =  poly->GetMeshPtr();
	int numSet = edges->NumberSet();
	
	if (undoable)
	{
		if (!theHold.Holding())
			theHold.Begin();

		//Split the edges list into originally soft and hard edges.
		int edgesSize = edges->GetSize();
		BitArray* softEdges = new BitArray(edgesSize);
		BitArray* hardEdges = new BitArray(edgesSize);
		for (int e = 0; e < edgesSize; e++)
		{
			if ((*edges)[e])
			{
				if (_isSoft(mesh, e))
					softEdges->Set(e);
				else 
					hardEdges->Set(e);
			}
		}
		theHold.Put(new EdgeSmoothRestoreObj(makeSoft, node, softEdges, hardEdges));
		node->SetAFlag(A_HELD);
	}

	if (numSet == mesh->ENum())
		mesh->Resmooth(makeSoft);
	else if (numSet > 0)
		_resmooth(mesh, *edges, makeSoft);

	if (undoable)
	{
		//It appears that theHold.Accept(int) is broken, and leads to unexpected 
		//junk strings in the undo list. Otherwise I would have used that.
		TCHAR* undoName = GetString(makeSoft ? IDS_MAKE_SOFT : IDS_MAKE_HARD);
		theHold.Accept(undoName);
	}

	//Notify EPoly of change and refresh screen.
	poly->LocalDataChanged(PART_TOPO);
	poly->RefreshScreen();
}

void EdgeSmooth::Apply(bool makeSoft) 
{
	if (!CanApplyToSel())
		throw MAXException(_M("Cannot apply EdgeSmooth to the current selection"));

	//Get selected node.
	Interface* ip = GetCOREInterface();
	INode* selNode = ip->GetSelNode(0);

	//Get edge selection.
	BitArray* edgeSel = new BitArray();
	MNMesh* mesh = _get_mesh(selNode);
	mesh->getEdgeSel(*edgeSel);
	
	Apply(makeSoft, selNode, edgeSel);
}

void EdgeSmooth::Apply(bool makeSoft, INode* node, BitArray* edges) 
{
	apply(makeSoft, node, edges, true);
}




//================================================
// RestoreObject implementation
//================================================
void EdgeSmoothRestoreObj::Restore(int isUndo)
{
	apply(true, this->node, this->softEdges, false);
	apply(false, this->node, this->hardEdges, false);
}

void EdgeSmoothRestoreObj::Redo()
{
	apply(this->makeSoft, this->node, this->AllEdges(), false);
}
