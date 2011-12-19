#include "EdgeSmoothClassDesc.h"

//================================================
//Class descriptor
//================================================
class edgeSmoothClassDesc : public ClassDesc2 
{
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