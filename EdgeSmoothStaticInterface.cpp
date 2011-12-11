#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "ifnpub.h"
#include "EdgeSmooth.h"

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
