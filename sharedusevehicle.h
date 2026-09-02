#ifndef SHAREDUSEVEHICLE_H
#define SHAREDUSEVEHICLE_H

#include "sharedsignout.h"

void HandleSharedUseVehicleChar(TCHAR key);
void HandleSharedUseVehicleKey(char key);
void QuerySharedVehicleInfo(void);
void TryUnlockSharedVehicle(void);
void HandleSharedSettlementChar(TCHAR key);
void HandleSharedSettlementKey(char key);
void ConfirmSharedSettlementPayment(void);
void UpdateSharedBicycleRecordInFile(const char* plate, const char* status, const char* battery);
void ResetSharedOrderScroll(void);
void ScrollSharedOrder(int offset);
void BeginSharedOrderDrag(int y);
void UpdateSharedOrderDrag(int y);
void EndSharedOrderDrag(void);
void SetSharedOrderCount(int count);

#endif
