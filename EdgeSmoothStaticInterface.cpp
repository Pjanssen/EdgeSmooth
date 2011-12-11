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
		makeSoft,
		makeHard,
		makeSelSoft,
		makeSelHard,
    };

    EdgeSmoothStaticInterface() : FPInterfaceDesc(id, _M("EdgeSmooth"), 0, NULL, FP_CORE, end)
    {
		AppendFunction(
            isSoft,           /* function ID */
            _M("IsSoft"),     /* internal name */
            0,                /* function name string resource name */ 
            TYPE_BOOL,        /* Return type */
            0,                /* Flags */
            2,                /* Number of arguments */
                _M("node"),   /* argument internal name */
                0,            /* argument localizable name string resource id */
				TYPE_INODE,   /* arg type */
				_M("edges"),
				0,
				TYPE_BITARRAY,
            end);

		AppendFunction(
            isHard,           /* function ID */
            _M("IsHard"),     /* internal name */
            0,                /* function name string resource name */ 
            TYPE_BOOL,        /* Return type */
            0,                /* Flags */
            2,                /* Number of arguments */
                _M("node"),   /* argument internal name */
                0,            /* argument localizable name string resource id */
                TYPE_INODE,   /* arg type */
				_M("edges"),
				0,
				TYPE_BITARRAY,
            end);

        AppendFunction(
            makeSoft,         /* function ID */
            _M("MakeSoft"),   /* internal name */
            0,                /* function name string resource name */ 
            TYPE_VOID,        /* Return type */
            0,                /* Flags */
            2,                /* Number of arguments */
                _M("node"),   /* argument internal name */
                0,            /* argument localizable name string resource id */
                TYPE_INODE,   /* arg type */
				_M("edges"),
				0,
				TYPE_BITARRAY,
            end);

		AppendFunction(
            makeHard,         /* function ID */
            _M("MakeHard"),   /* internal name */
            0,                /* function name string resource name */ 
            TYPE_VOID,        /* Return type */
            0,                /* Flags */
            2,                /* Number of arguments */
                _M("node"),   /* argument internal name */
                0,            /* argument localizable name string resource id */
                TYPE_INODE,   /* arg type */
				_M("edges"),
				0,
				TYPE_BITARRAY,
            end);

		 AppendFunction(
            makeSelSoft,         /* function ID */
            _M("MakeSelSoft"),   /* internal name */
            0,                /* function name string resource name */ 
            TYPE_VOID,        /* Return type */
            0,                /* Flags */
            0,
            end);

		AppendFunction(
            makeSelHard,         /* function ID */
            _M("MakeSelHard"),   /* internal name */
            0,                /* function name string resource name */ 
            TYPE_VOID,        /* Return type */
            0,                /* Flags */
            0,
            end);
    }


    BEGIN_FUNCTION_MAP
		FN_2(isSoft, TYPE_BOOL, IsSoft, TYPE_INODE, TYPE_BITARRAY)
		FN_2(isHard, TYPE_BOOL, IsHard, TYPE_INODE, TYPE_BITARRAY)
		VFN_2(makeSoft, MakeSoft, TYPE_INODE, TYPE_BITARRAY)
		VFN_2(makeHard, MakeHard, TYPE_INODE, TYPE_BITARRAY)
		VFN_0(makeSelSoft, MakeSelSoft)
		VFN_0(makeSelHard, MakeSelHard)
    END_FUNCTION_MAP
};

const Interface_ID EdgeSmoothStaticInterface::id = Interface_ID(0x739b2746, 0xa5033c1);
static EdgeSmoothStaticInterface edgeSmooth;
