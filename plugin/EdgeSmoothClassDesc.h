#pragma once

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <guplib.h>

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

const Class_ID edgesmooth_CLASS_ID(0x753a72b6, 0x796c6b3c);

ClassDesc2* GetEdgeSmoothDesc();



class edgesmooth_gup : public GUP
{
public:
	static HWND hParams;

	// GUP Methods
	DWORD	Start			( );
	void	Stop			( );
	DWORD_PTR	Control		( DWORD parameter );
		
	// Loading/Saving
	IOResult Save(ISave *isave);
	IOResult Load(ILoad *iload);

	//Constructor/Destructor
	edgesmooth_gup() { }
	~edgesmooth_gup() { }	
};
