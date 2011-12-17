#pragma once

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "iFnPub.h"
#include <guplib.h>

#include "EdgeSmooth.h"

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

extern const Class_ID iedgesmooth_CLASS_ID;

extern const Interface_ID EDGESMOOTH_INTERFACE;

inline class IEdgeSmooth* GetFP_basic() 
{ 
	return (IEdgeSmooth*) GetCOREInterface(EDGESMOOTH_INTERFACE); 
}

class IEdgeSmooth : public FPStaticInterface
{
public: 
	virtual FPStatus MakeSoft() = 0;
	virtual FPStatus MakeHard() = 0;

	enum {
		es_makeSoft,
		es_makeHard,
	};
};

class IEdgeSmoothImpl : public IEdgeSmooth
{
public:
	FPStatus MakeSoft();
	FPStatus MakeHard();

	DECLARE_DESCRIPTOR_INIT(IEdgeSmoothImpl)
	BEGIN_FUNCTION_MAP
		FN_ACTION(IEdgeSmooth::es_makeSoft, MakeSoft)
		FN_ACTION(IEdgeSmooth::es_makeHard, MakeHard)
	END_FUNCTION_MAP
};