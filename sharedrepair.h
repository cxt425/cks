#ifndef SHAREDREPAIR_H
#define SHAREDREPAIR_H

#include "sharedsignout.h"

void ResetSharedRepairState(void);
void HandleSharedRepairKey(char key);
void HandleSharedRepairChar(TCHAR key);
void SubmitSharedRepair(void);

#endif
