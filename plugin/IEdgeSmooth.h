#pragma once

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "iFnPub.h"
#include <guplib.h>

#include "EdgeSmooth.h"
#include "EdgeSmoothClassDesc.h"

const Interface_ID EDGESMOOTH_FN_INTERFACE(0x34363b16, 0x48cf5a07);
const Interface_ID EDGESMOOTH_ACT_INTERFACE(0x565b5802, 0x28db52d4);

inline class IEdgeSmooth* GetEdgeSmooth() 
{ 
	return (IEdgeSmooth*) GetCOREInterface(EDGESMOOTH_FN_INTERFACE); 
}

inline class IEdgeSmooth* GetEdgeSmoothActions()
{
	return (IEdgeSmooth*) GetCOREInterface(EDGESMOOTH_ACT_INTERFACE);
}


class IEdgeSmooth : public FPStaticInterface
{
public:
	void Apply(bool soften, INode* node, BitArray* edges);

	enum {
		es_isSoft,
		es_isHard,
		es_canApplyToSel,
		es_apply,
	};

	DECLARE_DESCRIPTOR_INIT(IEdgeSmooth)
	BEGIN_FUNCTION_MAP
		FN_2(IEdgeSmooth::es_isSoft, TYPE_BOOL, EdgeSmooth::IsSoft, TYPE_INODE, TYPE_BITARRAY)
		FN_2(IEdgeSmooth::es_isHard, TYPE_BOOL, EdgeSmooth::IsHard, TYPE_INODE, TYPE_BITARRAY)
		FN_0(IEdgeSmooth::es_canApplyToSel, TYPE_BOOL, EdgeSmooth::CanApplyToSel);
		VFN_3(IEdgeSmooth::es_apply, IEdgeSmooth::Apply, TYPE_bool, TYPE_INODE, TYPE_BITARRAY)
	END_FUNCTION_MAP
};


class IEdgeSmoothActions : public FPStaticInterface
{
public:
	FPStatus ApplySoft();
	FPStatus ApplyHard();

	enum {
		es_applySoft,
		es_applyHard,
		es_isEnabled,
	};

	DECLARE_DESCRIPTOR_INIT(IEdgeSmoothActions)
	BEGIN_FUNCTION_MAP
		FN_ACTION(IEdgeSmoothActions::es_applySoft, ApplySoft)
		FN_ACTION(IEdgeSmoothActions::es_applyHard, ApplyHard)
		FN_PRED(IEdgeSmoothActions::es_isEnabled, EdgeSmooth::CanApplyToSel)
	END_FUNCTION_MAP
};