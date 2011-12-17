#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "EdgeSmoothActions.h"
#include "ifnpub.h"
#include "EdgeSmooth.h"
#include "resource.h"
#include "iparamb2.h"

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

/*
class EdgeSmoothStaticInterface : public FPInterfaceDesc
{
public:

    static const Interface_ID id;
    
    enum FunctionIDs
    {
		isSoft,
		isHard,
		apply,
    };

	EdgeSmoothStaticInterface() : FPInterfaceDesc(id, _M("EdgeSmooth"), 0, NULL, FP_CORE, end)
    {
		//AppendFunction parameters:
		//AppendFunction(function ID, internal name, function name string resource name,
        //               Return type, Flags, Number of arguments, argument internal name,
        //               argument localizable name string resource id, arg type, end);

		AppendFunction(isSoft, _M("IsSoft"), 0, TYPE_BOOL, 0, 2,
                                             _M("node"), 0, TYPE_INODE,
				                             _M("edges"), 0, TYPE_BITARRAY,
                                             end);

		AppendFunction(isHard, _M("IsHard"), 0, TYPE_BOOL, 0, 2,
			                                 _M("node"), 0, TYPE_INODE,
											 _M("edges"), 0, TYPE_BITARRAY,
                                             end);


		AppendFunction(apply, _M("Apply"), 0, TYPE_VOID, 0, 3,
			                               _M("makeSoft"), 0, TYPE_BOOL,
										   _M("node"), 0, TYPE_INODE, f_keyArgDefault, NULL, 
										   _M("edges"), 0, TYPE_BITARRAY, f_keyArgDefault, NULL,
										   end);
    }


    BEGIN_FUNCTION_MAP
		FN_2(isSoft, TYPE_BOOL, EdgeSmooth::IsSoft, TYPE_INODE, TYPE_BITARRAY)
		FN_2(isHard, TYPE_BOOL, EdgeSmooth::IsHard, TYPE_INODE, TYPE_BITARRAY)
		VFN_3(apply, EdgeSmooth::ApplyFP, TYPE_BOOL, TYPE_INODE, TYPE_BITARRAY)
    END_FUNCTION_MAP
};

const Interface_ID EdgeSmoothStaticInterface::id = Interface_ID(0x739b2746, 0xa5033c1);
static EdgeSmoothStaticInterface edgeSmooth;
*/

//const ActionContextId edgeSmoothContext = 0x3b1a2031;


class EdgeSmoothClassDesc : public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new fp_basic_gup(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return GUP_CLASS_ID; }
	Class_ID		ClassID() { return fp_basics_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("edgesmooth"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }			// returns owning module handle
	

};

static EdgeSmoothClassDesc edgeSmoothCD;
ClassDesc2* GetEdgeSmoothCD() { return &edgeSmoothCD; }


class EdgeSmoothActionsImp : public EdgeSmoothActions
{
public:

	DECLARE_DESCRIPTOR(EdgeSmoothActionsImp)

	BEGIN_FUNCTION_MAP
		FN_ACTION(softenEdge, SoftenEdge)
		FN_ACTION(hardenEdge, HardenEdge)
	END_FUNCTION_MAP

	FPStatus SoftenEdge() 
	{
		try
		{
			EdgeSmooth::Apply(true); 
			return FPS_OK;
		}
		catch (MAXException e)
		{
			MessageBox(NULL, _T(e.message), _T("EdgeSmooth error"), MB_OK);
			return FPS_FAIL;
		}
	}
	FPStatus HardenEdge() 
	{
		try {
			EdgeSmooth::Apply(false); 
			return FPS_OK;
		} catch (MAXException e) {
			MessageBox(NULL, _T(e.message), _T("EdgeSmooth error"), MB_OK);
			return FPS_FAIL;
		}
	}

	
};

const Interface_ID EDGESMOOTH_INTERFACE = Interface_ID(0x21b10c92, 0x34063113);

static EdgeSmoothActionsImp esai(EDGESMOOTH_INTERFACE, _T("EdgeSmoothActions"), 0, 0, FP_ACTIONS,
								 EdgeSmoothActions::softenEdge, _T("SoftenEdge"), 0, 0, end,
																						end);
