#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "max.h"

//Returns true if the faces on ALL edges in the bitarray share one or more smoothing groups.
//Note: returns false if one or more edges are one-sided (i.e. have only one face).
//Usage in maxscript on current selection: 
//EdgeSmooth.IsSoft $ (polyOp.getEdgeSelection $)
BOOL IsSoft(INode* node, BitArray* edges);

//Returns true if the faces on ALL the edges in the bitarray do not share any smoothing groups.
//Note: if an edge is one-sided (i.e. it has only one face), this counts as a hard edge.
//Usage in maxscript on current selection: 
//EdgeSmooth.IsHard $ (polyOp.getEdgeSelection $)
BOOL IsHard(INode* node, BitArray* edges);


//Smooths the supplied edges in the mesh and returns true upon success.
//Usage in maxscript on current selection: 
//EdgeSmooth.MakeSoft $ (polyOp.getEdgeSelection $)
BOOL MakeSoft(INode* node, BitArray* edges);


//Unsmooths the supplied edges in the mesh and returns true upon success.
//Usage in maxscript on current selection: 
//EdgeSmooth.MakeHard $ (polyOp.getEdgeSelection $)
BOOL MakeHard(INode* node, BitArray* edges);


//Two wrapper functions that apply the MakeSoft and MakeHard functions
//to the current selection.
BOOL MakeSelSoft();
BOOL MakeSelHard();