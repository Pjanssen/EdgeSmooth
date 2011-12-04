#include "EdgeSmooth.h"
#include "iepoly.h"
#include "mnmath.h"


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


BOOL MakeSoft(INode* node, BitArray* edges) { return false; }
BOOL MakeHard(INode* node, BitArray* edges) { return false; }