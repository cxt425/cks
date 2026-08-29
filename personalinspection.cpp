#include "personalinspection.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

static PersonalInspectionState gInspectionState;
static const char* PERSONAL_DATA_FILE = "personal_vehicle_data.txt";

static int ConvertUtf8ToAnsi(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;

    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, source, -1,
        wideText, sizeof(wideText) / sizeof(wideText[0]));
    if (wideLength <= 0) return 0;

    return WideCharToMultiByte(CP_ACP, 0, wideText, -1,
        destination, destinationSize, NULL, NULL) > 0;
}

static int ConvertAnsiToUtf8(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;

    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_ACP, 0, source, -1,
        wideText, sizeof(wideText) / sizeof(wideText[0]));
    if (wideLength <= 0) return 0;

    return WideCharToMultiByte(CP_UTF8, 0, wideText, -1,
        destination, destinationSize, NULL, NULL) > 0;
}

static int CalculateNextInspectionDate(const char* registrationDate,
    char* nextInspectionDate, int destinationSize)
{
    int year;
    int month;
    int day;
    if (!registrationDate || !nextInspectionDate || destinationSize < 11 ||
        sscanf(registrationDate, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }

    sprintf(nextInspectionDate, "%04d-%02d-%02d", year + 1, month, day);
    return 1;
}

static int GetToday(char* date, int capacity)
{
    if (!date || capacity < 11) return 0;

    SYSTEMTIME today;
    GetLocalTime(&today);
    sprintf(date, "%04d-%02d-%02d", today.wYear, today.wMonth, today.wDay);
    return 1;
}

static int WriteUpdatedRecord(FILE* file, char* fields[8], const char* today)
{
    char normalStatus[32] = {0};
    char utf8Today[32] = {0};
    if (!ConvertAnsiToUtf8("正常", normalStatus, sizeof(normalStatus)) ||
        !ConvertAnsiToUtf8(today, utf8Today, sizeof(utf8Today))) {
        return 0;
    }

    return fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s\n",
        fields[0], fields[1], fields[2], fields[3], fields[4], fields[5],
        utf8Today, normalStatus) >= 0;
}

static void AppendChar(char* destination, int capacity, char key)
{
    int length = (int)strlen(destination);
    if (length >= capacity - 1) return;

    if (gInspectionState.focus == 0) {
        if (length == 0 && (key < 'A' || key > 'Z')) return;
        if (length > 0 && !((key >= 'A' && key <= 'Z') ||
                            (key >= '0' && key <= '9'))) return;
        if (length >= 5) return;
    } else if (gInspectionState.focus == 1) {
        if ((unsigned char)key < 0x80 && (key < 32 || key > 126)) return;
    }

    destination[length] = key;
    destination[length + 1] = '\0';
}

static void DeleteChar(char* destination)
{
    int length = (int)strlen(destination);
    if (length <= 0) return;

    if ((unsigned char)destination[length - 1] >= 0x80 && length >= 2)
        length -= 2;
    else
        --length;
    destination[length] = '\0';
}

void InitPersonalInspectionState(void)
{
    memset(&gInspectionState, 0, sizeof(gInspectionState));
    strcpy(gInspectionState.message, "请输入车牌号和车主姓名");
}

PersonalInspectionState* GetPersonalInspectionState(void)
{
    return &gInspectionState;
}

void QueryPersonalInspection(void)
{
    PersonalInspectionState* state = &gInspectionState;
    state->queryFound = 0;
    state->statusUpdated = 0;
    state->nextInspectionDate[0] = '\0';
    strcpy(state->message, "查询失败：车牌号和车主姓名不匹配");

    FILE* file = fopen(PERSONAL_DATA_FILE, "r");
    if (!file) {
        strcpy(state->message, "查询失败：未找到车辆数据");
        return;
    }

    char line[512];
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
        char ownerName[64] = {0};
        char registrationDate[32] = {0};
        char vehicleStatus[32] = {0};
        if (!ConvertUtf8ToAnsi(fields[0], licensePlate, sizeof(licensePlate)) ||
            !ConvertUtf8ToAnsi(fields[1], ownerName, sizeof(ownerName)) ||
            !ConvertUtf8ToAnsi(fields[6], registrationDate, sizeof(registrationDate)) ||
            !ConvertUtf8ToAnsi(fields[7], vehicleStatus, sizeof(vehicleStatus))) {
            continue;
        }

        if (strcmp(licensePlate, state->licensePlate) == 0 &&
            strcmp(ownerName, state->ownerName) == 0) {
            if (!CalculateNextInspectionDate(registrationDate,
                state->nextInspectionDate, sizeof(state->nextInspectionDate))) {
                strcpy(state->message, "查询失败：注册日期格式错误");
                fclose(file);
                return;
            }
            strcpy(state->vehicleStatus, vehicleStatus);
            strcpy(state->originalVehicleStatus, vehicleStatus);
            strcpy(state->registrationDate, registrationDate);
            state->queryFound = 1;
            if (strcmp(vehicleStatus, "报废") == 0) {
                strcpy(state->message, "该车辆已报废，不能进行年审更新");
            } else {
                strcpy(state->message, "查询成功");
            }
            fclose(file);
            return;
        }
    }
    fclose(file);
}

void UpdatePersonalInspectionStatus(void)
{
    PersonalInspectionState* state = &gInspectionState;
    if (!state->queryFound) {
        strcpy(state->message, "请先查询车辆信息");
        return;
    }
    if (strcmp(state->vehicleStatus, "报废") == 0) {
        strcpy(state->message, "该车辆已报废，不能更新年审状态");
        return;
    }

    char today[11] = {0};
    if (!GetToday(today, sizeof(today))) return;

    strcpy(state->vehicleStatus, "正常");
    int year;
    int month;
    int day;
    sscanf(today, "%d-%d-%d", &year, &month, &day);
    sprintf(state->nextInspectionDate, "%04d-%02d-%02d", year + 1, month, day);
    state->statusUpdated = 1;
    strcpy(state->message, "状态已更新，点击保存写入数据");
}

void SavePersonalInspectionUpdate(void)
{
    PersonalInspectionState* state = &gInspectionState;
    if (!state->queryFound) {
        strcpy(state->message, "请先查询车辆信息");
        return;
    }
    if (strcmp(state->vehicleStatus, "报废") == 0) {
        strcpy(state->message, "该车辆已报废，不能保存年审信息");
        return;
    }
    if (!state->statusUpdated) {
        strcpy(state->message, "请先点击更新年审状态");
        return;
    }

    char today[11] = {0};
    if (!GetToday(today, sizeof(today))) return;

    FILE* sourceFile = fopen(PERSONAL_DATA_FILE, "r");
    FILE* tempFile = fopen("personal_vehicle_data.tmp", "w");
    if (!sourceFile || !tempFile) {
        if (sourceFile) fclose(sourceFile);
        if (tempFile) fclose(tempFile);
        strcpy(state->message, "保存失败：无法打开车辆数据");
        return;
    }

    char line[512];
    int updated = 0;
    while (fgets(line, sizeof(line), sourceFile)) {
        char original[512];
        strcpy(original, line);
        char* fields[8] = {0};
        char* field = strtok(line, "|\r\n");
        int count = 0;
        while (field && count < 8) {
            fields[count++] = field;
            field = strtok(NULL, "|\r\n");
        }

        char licensePlate[32] = {0};
        char ownerName[64] = {0};
        if (!updated && count >= 8 &&
            ConvertUtf8ToAnsi(fields[0], licensePlate, sizeof(licensePlate)) &&
            ConvertUtf8ToAnsi(fields[1], ownerName, sizeof(ownerName)) &&
            strcmp(licensePlate, state->licensePlate) == 0 &&
            strcmp(ownerName, state->ownerName) == 0) {
            updated = WriteUpdatedRecord(tempFile, fields, today);
        } else {
            fputs(original, tempFile);
        }
    }
    fclose(sourceFile);
    fclose(tempFile);

    if (!updated) {
        remove("personal_vehicle_data.tmp");
        strcpy(state->message, "保存失败：未找到匹配车辆");
        return;
    }

    remove(PERSONAL_DATA_FILE);
    rename("personal_vehicle_data.tmp", PERSONAL_DATA_FILE);
    strcpy(state->registrationDate, today);
    strcpy(state->message, "保存成功：车辆状态为正常");
}

void RestorePersonalInspectionState(void)
{
    PersonalInspectionState* state = &gInspectionState;
    if (!state->queryFound) return;

    strcpy(state->vehicleStatus, state->originalVehicleStatus);
    if (state->registrationDate[0]) {
        CalculateNextInspectionDate(state->registrationDate,
            state->nextInspectionDate, sizeof(state->nextInspectionDate));
    }
    state->statusUpdated = 0;
    strcpy(state->message, "已恢复原年审状态");
}

void HandlePersonalInspectionKey(char key)
{
    if (key == 8 || key == 127) {
        if (gInspectionState.focus == 0)
            DeleteChar(gInspectionState.licensePlate);
        else
            DeleteChar(gInspectionState.ownerName);
        gInspectionState.queryFound = 0;
        return;
    }

    if (key == 13 || key == 10) {
        QueryPersonalInspection();
        return;
    }

    if (key == 9) {
        gInspectionState.focus = (gInspectionState.focus + 1) % 2;
        return;
    }

    if (gInspectionState.focus == 0)
        AppendChar(gInspectionState.licensePlate, sizeof(gInspectionState.licensePlate), key);
    else
        AppendChar(gInspectionState.ownerName, sizeof(gInspectionState.ownerName), key);
    gInspectionState.queryFound = 0;
}

void HandlePersonalInspectionChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandlePersonalInspectionKey((char)key);
        return;
    }

    char converted[MB_LEN_MAX] = {0};
#ifdef UNICODE
    int byteCount = WideCharToMultiByte(CP_ACP, 0, &key, 1,
        converted, sizeof(converted), NULL, NULL);
#else
    converted[0] = (char)key;
    int byteCount = 1;
#endif
    for (int i = 0; i < byteCount; ++i)
        HandlePersonalInspectionKey(converted[i]);
}
