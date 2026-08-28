#ifndef PERSONALINSPECTION_H
#define PERSONALINSPECTION_H

#include "public.h"
#include <windows.h>

typedef struct {
    char licensePlate[10];
    char ownerName[20];
    char vehicleStatus[16];
    char nextInspectionDate[11];
    char registrationDate[11];
    char originalVehicleStatus[16];
    char message[64];
    int queryFound;
    int statusUpdated;
    int focus;
} PersonalInspectionState;

void InitPersonalInspectionState(void);
PersonalInspectionState* GetPersonalInspectionState(void);
void HandlePersonalInspectionKey(char key);
void HandlePersonalInspectionChar(TCHAR key);
void QueryPersonalInspection(void);
void UpdatePersonalInspectionStatus(void);
void SavePersonalInspectionUpdate(void);
void RestorePersonalInspectionState(void);

#endif
