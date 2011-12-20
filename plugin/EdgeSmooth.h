#pragma once

#include "max.h"
#include "resource.h"

extern TCHAR* GetString(int id);

class EdgeSmooth
{
public:
	/// Returns true if the faces on ALL edges in the bitarray share one or more smoothing groups.
	/// Note: returns false if one or more edges are one-sided (i.e. have only one face).
	/// Usage in maxscript on current selection: 
	/// EdgeSmooth.IsSoft $ (polyOp.getEdgeSelection $)
	static BOOL IsSoft(INode* node, BitArray* edges);

	/// Returns true if the faces on ALL the edges in the bitarray do not share any smoothing groups.
	/// Note: if an edge is one-sided (i.e. it has only one face), this counts as a hard edge.
	/// Usage in maxscript on current selection: 
	/// EdgeSmooth.IsHard $ (polyOp.getEdgeSelection $)
	static BOOL IsHard(INode* node, BitArray* edges);

	/// Returns true if the EdgeSmooth::Apply function can be applied to the current selection.
	/// Will return false if the current selection count != 1, or if the currently selected object is
	/// not a poly object.
	static BOOL CanApply();

	/// Applies the EdgeSmooth function to the currerntly seleceted object and edge selection.
	/// If makeSoft is true the edges will be smoothed. Maxscript usage example:
	/// EdgeSmooth.Apply false
	static void Apply(bool makeSoft);

	/// Applies the EdgeSmooth function to the given node and edges.
	/// If makeSoft is true the edges will be smoothed. Maxscript usage example:
	/// EdgeSmooth.Apply false node:$ edges:(polyop.getEdgeSelection $)
	static void Apply(bool makeSoft, INode* node, BitArray* edges);
};


class EdgeSmoothRestoreObj : public RestoreObj
{
private:
	bool makeSoft;
	INode* node;
	BitArray* softEdges;
	BitArray* hardEdges;
	
public:
	EdgeSmoothRestoreObj(bool makeSoft, INode* node, BitArray* softEdges, BitArray* hardEdges)
	{
		this->makeSoft = makeSoft;
		this->node = node;
		this->softEdges = softEdges;
		this->hardEdges = hardEdges;
	}
	~EdgeSmoothRestoreObj() {}

	BitArray* AllEdges() { return new BitArray((*(this->softEdges)) | (*(this->hardEdges))); }

	int Size() { return sizeof(INode*) + sizeof(bool) + 2 * sizeof(BitArray*); }
	void EndHold() { node->ClearAFlag(A_HELD); }

	void Restore(int isUndo);
	void Redo();
};