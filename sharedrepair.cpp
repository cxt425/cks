#include "sharedrepair.h"
#include "sharedusevehicle.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

void ResetSharedRepairState(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    strcpy(state->repairType, "无法开锁");
    state->repairDetail[0] = '\0';
}

void HandleSharedRepairKey(char key)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (key == 8 || key == 127) {
        int len = (int)strlen(state->repairDetail);
        if (len > 0) state->repairDetail[len - 1] = '\0';
        return;
    }
    if (key == 13 || key == 10) {
        SubmitSharedRepair();
        return;
    }
    if (key == 9) {
        return;
    }
    if ((unsigned char)key >= 0x80 || (key >= 32 && key <= 126) || key == ' ') {
        int len = (int)strlen(state->repairDetail);
        if (len < (int)sizeof(state->repairDetail) - 1) {
            state->repairDetail[len] = key;
            state->repairDetail[len + 1] = '\0';
        }
    }
}

void HandleSharedRepairChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandleSharedRepairKey((char)key);
        return;
    }

    char converted[MB_LEN_MAX] = {0};
    int byteCount = 0;
#ifdef UNICODE
    byteCount = WideCharToMultiByte(CP_ACP, 0, &key, 1, converted, sizeof(converted), NULL, NULL);
#else
    converted[0] = (char)key;
    byteCount = 1;
#endif
    if (byteCount <= 0) return;

    SharedUserInfo* state = GetSharedSignoutState();
    for (int i = 0; i < byteCount; ++i) {
        char ch = converted[i];
        if ((unsigned char)ch >= 0x80 || (ch >= 32 && ch <= 126) || ch == ' ') {
            int len = (int)strlen(state->repairDetail);
            if (len < (int)sizeof(state->repairDetail) - 1) {
                state->repairDetail[len] = ch;
                state->repairDetail[len + 1] = '\0';
            }
        }
    }
}

void SubmitSharedRepair(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (state->sharedUsePlate[0] == '\0') return;

    char detailText[128];
    snprintf(detailText, sizeof(detailText), "%s", state->repairDetail[0] ? state->repairDetail : "--");

    char statusText[256];
    snprintf(statusText, sizeof(statusText), "报修中（%s|%s）", state->repairType[0] ? state->repairType : "无法开锁", detailText);

    char batteryText[16];
    snprintf(batteryText, sizeof(batteryText), "%s", state->sharedUseBattery[0] ? state->sharedUseBattery : "--");
    UpdateSharedBicycleRecordInFile(state->sharedUsePlate, statusText, batteryText);

    strcpy(state->sharedUseStatus, statusText);
    strcpy(state->sharedUseMessage, "报修提交成功：已进入维修流程");

    ResetSharedRepairState();
    currentPage = PAGE_SHARED_SETTLEMENT;
}
