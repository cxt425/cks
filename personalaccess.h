#ifndef _PERSONALACCESS_H_
#define _PERSONALACCESS_H_

#include "personalregistration.h"

void HandlePersonalAccessKey(char key);
void HandlePersonalAccessChar(TCHAR key);
void SubmitPersonalAccessRecord(void);
void QueryPersonalAccessRecords(void);
void HandlePersonalAccessQueryKey(char key);
void HandlePersonalAccessQueryChar(TCHAR key);
void ScrollPersonalAccessQuery(int delta);

#endif
