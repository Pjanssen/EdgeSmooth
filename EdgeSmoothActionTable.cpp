#include "3dsmaxsdk_preinclude.h"
#include "max.h"
#include "resource.h"
#include "EdgeSmoothActionTable.h"
#include "EdgeSmooth.h"

#define NumElements(array) (sizeof(array) / sizeof(array[0]))



static ActionDescription spActions[] = {
   IDS_MAKE_SOFT,
   IDS_MAKE_HARD,
};


ActionTable* BuildActionTable()
{
   TSTR name = "EdgeSmooth"; //GetString(IDS_RB_FFDGEN);
   HACCEL hAccel = LoadAccelerators(hInstance,
             MAKEINTRESOURCE(IDR_EDGESMOOTH_SHORTCUTS));

   int numOps = NumElements(spActions);

   ActionTable* pTab;

   pTab = new ActionTable(edgeSmoothActionsId,
                          edgeSmoothContext,
                          name,
                          hAccel,
                          numOps,
                          spActions,
                          hInstance);

   GetCOREInterface()->GetActionManager()->RegisterActionContext (edgeSmoothContext, name.data());
   //GetCOREInterface()->GetActionManager()->RegisterActionTable(pTab);

   return pTab;
}




BOOL EdgeSmoothActionCB::ExecuteAction(int id)
{
   switch (id)
   {
   case ID_MAKE_SOFT:
	   DebugPrint("Make soft");
	   EdgeSmooth::Apply(true);
	   return TRUE;
   case ID_MAKE_HARD:
	   DebugPrint("Make hard");
	   EdgeSmooth::Apply(false);
	   return TRUE;
   }

   return FALSE;
}

