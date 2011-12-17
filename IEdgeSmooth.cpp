#include "IEdgeSmooth.h"

const Class_ID edgesmooth_CLASS_ID(0x753a72b6, 0x796c6b3c);


class edgesmooth_gup : public GUP
{
public:
	static HWND hParams;

	// GUP Methods
	DWORD	Start			( );
	void	Stop			( );
	DWORD_PTR	Control			( DWORD parameter );
		
	// Loading/Saving
	IOResult Save(ISave *isave);
	IOResult Load(ILoad *iload);

	//Constructor/Destructor
	edgesmooth_gup();
	~edgesmooth_gup();		
};


edgesmooth_gup::edgesmooth_gup() { }
edgesmooth_gup::~edgesmooth_gup() { }

DWORD edgesmooth_gup::Start( ) {
	return GUPRESULT_KEEP; // Activate and Stay Resident
}

void edgesmooth_gup::Stop( ) { }

DWORD_PTR edgesmooth_gup::Control( DWORD parameter ) {
	return 0;
}

IOResult edgesmooth_gup::Save(ISave* isave) {
	return IO_OK;
}

IOResult edgesmooth_gup::Load(ILoad *iload) {
	return IO_OK;
}


//================================================
//Our class descriptor
//================================================
class edgeSmoothClassDesc : public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new edgesmooth_gup(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return GUP_CLASS_ID; }
	Class_ID		ClassID() { return edgesmooth_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("edgesmooth"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }			// returns owning module handle
};

static edgeSmoothClassDesc edgeSmoothDesc;
ClassDesc2* GetEdgeSmoothDesc() { return &edgeSmoothDesc; }



//================================================
//Function Publishing implementations
//================================================

const Interface_ID EDGESMOOTH_INTERFACE(0x34363b16, 0x48cf5a07);

const ActionContextId kActionContext         = 0;

//This is a contructor for class FP_Basic that takes a variable number of arugments.
IEdgeSmoothImpl edgesmooth_desc(
	EDGESMOOTH_INTERFACE,   //Interface_ID
	_T("edgesmooth"),		//Internal Fixed Name
	IDS_CLASS_NAME,		    //localized string resource ID
	&edgeSmoothDesc,		//owning class descriptor
	FP_ACTIONS, 
	kActionMainUIContext,
		IEdgeSmooth::es_makeHard, _T("MakeHard"), IDS_MAKE_HARD, 0, 
			f_menuText, IDS_MAKE_HARD,
			f_buttonText, IDS_MAKE_HARD,
			f_toolTip, IDS_MAKE_HARD_TT, 
			f_icon, "edgesmooth", 1,
			f_shortCut, FSHIFT, 72,
			end,	
		IEdgeSmooth::es_makeSoft, _T("MakeSoft"), IDS_MAKE_SOFT, 0,
			f_menuText, IDS_MAKE_SOFT,
			f_buttonText, IDS_MAKE_SOFT,
			f_toolTip, IDS_MAKE_SOFT_TT, 
			f_icon, "edgesmooth", 2,
			f_shortCut, FSHIFT, 83,
			end,
		
	end
);

void IEdgeSmoothImpl::init() { }

FPStatus IEdgeSmoothImpl::MakeSoft()
{
	DebugPrint(_T("MakeSoft\n"));
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

FPStatus IEdgeSmoothImpl::MakeHard()
{
	 DebugPrint(_T("MakeHard\n"));
	 try {
		EdgeSmooth::Apply(false); 
		return FPS_OK;
	} catch (MAXException e) {
		MessageBox(NULL, _T(e.message), _T("EdgeSmooth error"), MB_OK);
		return FPS_FAIL;
	}
}