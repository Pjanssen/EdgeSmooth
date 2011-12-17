#pragma once

#include "ActionTable.h"

const ActionTableId edgeSmoothActionsId   = 0x3b1a2031;
const ActionContextId edgeSmoothContext = 0x3b1a2031;

ActionTable* BuildActionTable();

class EdgeSmoothActionCB : public ActionCallback
{
   public:
     EdgeSmoothActionCB() { }
     BOOL ExecuteAction(int id);
};