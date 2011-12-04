#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "max.h"

//Returns true if the faces on ALL edges in the bitarray share one or more smoothing groups.
//Note: returns false if one or more edges are one-sided (i.e. have only one face).
BOOL IsSoft(INode* node, BitArray* edges);

//Returns true if the faces on ALL the edges in the bitarray do not share any smoothing groups.
//Note: if an edge is one-sided (i.e. it has only one face), this counts as a hard edge.
BOOL IsHard(INode* node, BitArray* edges);


BOOL MakeSoft(INode* node, BitArray* edges);
BOOL MakeHard(INode* node, BitArray* edges);
