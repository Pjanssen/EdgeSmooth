#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "max.h"



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

	/// Applies the EdgeSmooth function to the currerntly seleceted object and edge selection.
	/// If makeSoft is true the edges will be smoothed. Maxscript usage example:
	/// EdgeSmooth.Apply false
	static void Apply(bool makeSoft);

	/// Applies the EdgeSmooth function to the given node and edges.
	/// If makeSoft is true the edges will be smoothed. Maxscript usage example:
	/// EdgeSmooth.Apply false node:$ edges:(polyop.getEdgeSelection $)
	static void Apply(bool makeSoft, INode* node, BitArray* edges);

	/// A wrapper function for the SDK's FunctionPublishing mechanism to allow both overloads
	/// of the Apply function to be called from maxscript. Should not be called directly other
	/// than by the static interface.
	static void ApplyFP(bool makeSoft, INode* node, BitArray* edges);
};