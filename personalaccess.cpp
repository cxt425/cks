#include "personalaccess.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

static const int ACCESS_TIME_MAX_LEN = 11;

static int IsAccessTimeValid(const char* timeStr)
{
    if (!timeStr) return 0;
    int len = (int)strlen(timeStr);
    if (len != 10) return 0;
    if (timeStr[4] != '-' || timeStr[7] != '-') return 0;
    for (int i = 0; i < len; ++i) {
        if (i == 4 || i == 7) continue;
        if (timeStr[i] < '0' || timeStr[i] > '9') return 0;
    }
    return 1;
}

static int IsAccessTimeKeyAllowed(const char* dest, char key)
{
    int len = (int)strlen(dest);
    if (len >= ACCESS_TIME_MAX_LEN) return 0;
    if ((len == 4 || len == 7) && key == '-') return 1;
    if ((len == 4 || len == 7) && key != '-') return 0;
    if (key < '0' || key > '9') return 0;
    return 1;
}

static void AppendAccessTimeChar(char* dest, int maxLen, char key)
{
    int len = (int)strlen(dest);
    if (len >= ACCESS_TIME_MAX_LEN || len >= maxLen - 1) return;
    if (!IsAccessTimeKeyAllowed(dest, key)) return;
    dest[len] = key;
    dest[len + 1] = '\0';
}

static void AppendAccessPlateChar(char* dest, int maxLen, char key)
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
    if ((len == 0 && (key < 'A' || key > 'Z')) ||
        (len > 0 && !((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))) || len >= 5) return;
    dest[len] = key;
    dest[len + 1] = '\0';
}

static int ConvertUtf8ToAnsi(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;
    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, source, -1, wideText, 128);
    if (wideLength <= 0) return 0;
    return WideCharToMultiByte(CP_ACP, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

static int ConvertAnsiToUtf8(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;
    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_ACP, 0, source, -1, wideText, 128);
    if (wideLength <= 0) return 0;
    return WideCharToMultiByte(CP_UTF8, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

static int GetVehicleStatusByPlate(const char* plate, char* statusOut, int statusSize)
{
    if (!plate || !statusOut || statusSize <= 0) return 0;
    FILE* file = fopen("personal_vehicle_data.txt", "r");
    if (!file) return 0;

    char line[512];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char* fields[16] = {0};
        char* field = strtok(line, "|\r\n");
        int count = 0;
        while (field && count < 16) {
            fields[count++] = field;
            field = strtok(NULL, "|\r\n");
        }
        if (count < 8) continue;

        char licensePlate[32] = {0};
        char statusValue[32] = {0};
        if (!ConvertUtf8ToAnsi(fields[0], licensePlate, sizeof(licensePlate)) ||
            !ConvertUtf8ToAnsi(fields[7], statusValue, sizeof(statusValue))) {
            continue;
        }
        if (strcmp(licensePlate, plate) == 0) {
            snprintf(statusOut, statusSize, "%s", statusValue);
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

static void RefreshAccessScrapCheck(void)
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (state->accessLicensePlate[0] == '\0') {
        state->accessMessage[0] = '\0';
        return;
    }

    char statusValue[32] = {0};
    if (GetVehicleStatusByPlate(state->accessLicensePlate, statusValue, sizeof(statusValue)) &&
        strcmp(statusValue, "报废") == 0) {
        strcpy(state->accessMessage, "该车辆已报废，不能提交出入记录");
        return;
    }

    if (strcmp(state->accessMessage, "该车辆已报废，不能提交出入记录") == 0) {
        state->accessMessage[0] = '\0';
    }
}

void SubmitPersonalAccessRecord(void)
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    RefreshAccessScrapCheck();
    if (strcmp(state->accessMessage, "该车辆已报废，不能提交出入记录") == 0) {
        return;
    }
    if (state->accessLicensePlate[0] == '\0') {
        strcpy(state->accessMessage, "请输入车牌号");
        return;
    }
    if (state->accessTime[0] == '\0') {
        strcpy(state->accessMessage, "请输入出入日期");
        return;
    }
    if (strlen(state->accessLicensePlate) != 5 || !(state->accessLicensePlate[0] >= 'A' && state->accessLicensePlate[0] <= 'Z')) {
        strcpy(state->accessMessage, "车牌号格式错误");
        return;
    }
    if (!IsAccessTimeValid(state->accessTime)) {
        strcpy(state->accessMessage, "日期格式应为 YYYY-MM-DD");
        return;
    }

    FILE* file = fopen("personal_vehicle_data.txt", "r");
    if (!file) {
        strcpy(state->accessMessage, "车辆未登记或未查询到");
        return;
    }

    char line[512];
    int found = 0;
    char ownerName[64] = {0};
    char vehicleStatus[32] = {0};
    while (fgets(line, sizeof(line), file)) {
        char* fields[8] = {0};
        char* field = strtok(line, "|\r\n");
        int count = 0;
        while (field && count < 8) {
            fields[count++] = field;
            field = strtok(NULL, "|\r\n");
        }
        if (count < 8) continue;

        char licensePlate[32] = {0};
        char tempOwner[64] = {0};
        char tempStatus[32] = {0};
        if (!ConvertUtf8ToAnsi(fields[0], licensePlate, sizeof(licensePlate)) ||
            !ConvertUtf8ToAnsi(fields[1], tempOwner, sizeof(tempOwner)) ||
            !ConvertUtf8ToAnsi(fields[7], tempStatus, sizeof(tempStatus))) {
            continue;
        }
        if (strcmp(licensePlate, state->accessLicensePlate) == 0) {
            found = 1;
            strcpy(ownerName, tempOwner);
            strcpy(vehicleStatus, tempStatus);
            break;
        }
    }
    fclose(file);

    if (!found) {
        strcpy(state->accessMessage, "车辆未登记或未查询到");
        return;
    }
    if (strcmp(vehicleStatus, "报废") == 0) {
        strcpy(state->accessMessage, "该车辆已报废，不能提交出入记录");
        return;
    }

    char utf8Owner[64] = {0};
    char utf8Type[16] = {0};
    char utf8Line[256] = {0};
    ConvertAnsiToUtf8(ownerName, utf8Owner, sizeof(utf8Owner));
    ConvertAnsiToUtf8(state->accessType, utf8Type, sizeof(utf8Type));

    FILE* accessFile = fopen("access_records.txt", "a");
    if (!accessFile) {
        strcpy(state->accessMessage, "记录保存失败");
        return;
    }

    fprintf(accessFile, "%s|%s|%s|%s\n", state->accessLicensePlate, utf8Owner, utf8Type, state->accessTime);
    fclose(accessFile);

    state->accessLicensePlate[0] = '\0';
    state->accessTime[0] = '\0';
    state->accessFocus = 0;
    state->accessTypeSelected = 0;
    strcpy(state->accessType, "入校");
    strcpy(state->accessMessage, "记录已提交成功");
}

void HandlePersonalAccessKey(char key)
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (key == 8 || key == 127) {
        if (state->accessFocus == 0) {
            int len = (int)strlen(state->accessLicensePlate);
            if (len > 0) state->accessLicensePlate[len - 1] = '\0';
        } else if (state->accessFocus == 1) {
            int len = (int)strlen(state->accessTime);
            if (len > 0) state->accessTime[len - 1] = '\0';
        }
        return;
    }
    if (key == 13 || key == 10) {
        SubmitPersonalAccessRecord();
        return;
    }
    if (key == 9) {
        state->accessFocus = (state->accessFocus == 0) ? 1 : 0;
        return;
    }

    if (state->accessFocus == 0) {
        AppendAccessPlateChar(state->accessLicensePlate, sizeof(state->accessLicensePlate), key);
        RefreshAccessScrapCheck();
    } else {
        if ((key >= '0' && key <= '9') || key == '-' || key == ' ' || key == ':') {
            AppendAccessTimeChar(state->accessTime, sizeof(state->accessTime), key);
        }
    }
}

void HandlePersonalAccessChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandlePersonalAccessKey((char)key);
        return;
    }

    PersonalUserInfo* state = GetPersonalRegistrationState();
    char converted[MB_LEN_MAX] = {0};
    int byteCount = 0;
#ifdef UNICODE
    byteCount = WideCharToMultiByte(CP_ACP, 0, &key, 1, converted, sizeof(converted), NULL, NULL);
#else
    converted[0] = (char)key;
    byteCount = 1;
#endif
    if (byteCount <= 0) return;
    for (int i = 0; i < byteCount; ++i) {
        if (state->accessFocus == 0) {
            AppendAccessPlateChar(state->accessLicensePlate, sizeof(state->accessLicensePlate), converted[i]);
            RefreshAccessScrapCheck();
        } else {
            if ((converted[i] >= '0' && converted[i] <= '9') || converted[i] == '-' || converted[i] == ' ' || converted[i] == ':') {
                AppendAccessTimeChar(state->accessTime, sizeof(state->accessTime), converted[i]);
            }
        }
    }
}
