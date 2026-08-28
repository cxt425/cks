#include "personalregistration.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

static const char* PERSONAL_DATA_FILE = "personal_vehicle_data.txt";

static int IsValidInputChar(int focus, char key)// Helper: 判断当前输入框是否允许输入该字符
{
    if (focus == 4)
        return (key >= '0' && key <= '9');
    if (focus == 6)
        return (key >= '0' && key <= '9') || key == '-';
    return (unsigned char)key >= 0x80 || (key >= 32 && key <= 126);
}

static void AppendCharToField(char* dest, int maxLen, int focus, char key)// Helper: 向当前输入框追加字符，限制输入长度
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
    if (focus == 0) {
        if ((len == 0 && (key < 'A' || key > 'Z')) ||
            (len > 0 && !((key >= 'A' && key <= 'Z') ||
                          (key >= '0' && key <= '9'))) || len >= 5) return;
        dest[len] = key;
        dest[len + 1] = '\0';
        return;
    }
    if (focus == 3) {
        if ((len == 0 && (key < 'A' || key > 'Z')) ||
            (len > 0 && (key < '0' || key > '9')) || len >= 10) return;
        dest[len] = key;
        dest[len + 1] = '\0';
        return;
    }
    if (!IsValidInputChar(focus, key)) return;
    dest[len] = key;
    dest[len + 1] = '\0';
}

static void DeleteCharFromField(char* dest)// Helper: 删除当前输入框最后一个字符
{
    int len = (int)strlen(dest);
    if (len <= 0) return;
    if ((unsigned char)dest[len - 1] >= 0x80 && len >= 2) len -= 2;
    else --len;
    dest[len] = '\0';
}

static int ConvertAnsiToUtf8(const char* source, char* destination, int destinationSize)// Helper: 将 ANSI 字符串转换为 UTF-8 字符串
{
    if (!source || !destination || destinationSize <= 0) return 0;

    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_ACP, 0, source, -1, wideText, sizeof(wideText) / sizeof(wideText[0]));
    if (wideLength <= 0) return 0;

    return WideCharToMultiByte(CP_UTF8, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

static int ConvertUtf8ToAnsi(const char* source, char* destination, int destinationSize)// Helper: 将 UTF-8 字符串转换为 ANSI 字符串
{
    if (!source || !destination || destinationSize <= 0) return 0;
    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, source, -1, wideText, 128);
    if (wideLength <= 0) return 0;
    return WideCharToMultiByte(CP_ACP, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

void QueryPersonalVehicleInfo(void)// Helper: 按车牌号查询车辆信息
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    state->queryFound = 0;
    strcpy(state->queryMessage, "未找到对应车辆");
    PersonalVehicleRecord record;
    FILE* file = fopen(PERSONAL_DATA_FILE, "r");
    if (!file) return;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char* fields[8] = {0};
        char* field = strtok(line, "|\r\n");
        int count = 0;
        while (field && count < 8) {
            fields[count++] = field;
            field = strtok(NULL, "|\r\n");
        }
        if (count < 7) continue;

        char ansiFields[8][64] = {{0}};
        int valid = 1;
        for (int i = 0; i < count; ++i) {
            if (!ConvertUtf8ToAnsi(fields[i], ansiFields[i], sizeof(ansiFields[i]))) {
                valid = 0;
                break;
            }
        }
        if (valid && strcmp(ansiFields[0], state->queryLicensePlate) == 0) {
            memset(&record, 0, sizeof(record));
            strcpy(record.licensePlate, ansiFields[0]);
            strcpy(record.ownerName, ansiFields[1]);
            strcpy(record.college, ansiFields[2]);
            strcpy(record.personalID, ansiFields[3]);
            strcpy(record.ownerPhone, ansiFields[4]);
            strcpy(record.vehicleType, ansiFields[5]);
            strcpy(record.registrationDate, ansiFields[6]);
            if (count >= 8) strcpy(record.vehicleStatus, ansiFields[7]);
            else strcpy(record.vehicleStatus, "正常");
            state->queryFound = 1;
            state->queryResult = record;
            strcpy(state->queryMessage, "查询成功");
            fclose(file);
            return;
        }
    }
    fclose(file);
}

static int WritePersonalRecordUtf8(FILE* file, const PersonalVehicleRecord* record)
{
    char fields[8][128];
    const char* source[8] = { record->licensePlate, record->ownerName, record->college,
        record->personalID, record->ownerPhone, record->vehicleType,
        record->registrationDate, record->vehicleStatus };
    for (int i = 0; i < 8; ++i) {
        if (!ConvertAnsiToUtf8(source[i], fields[i], sizeof(fields[i]))) return 0;
    }
    return fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s\n",
        fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], fields[7]) >= 0;
}

void UpdatePersonalVehicleInfo(void)// Helper: 更新个人车辆信息
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (!state->queryFound) return;

    FILE* sourceFile = fopen(PERSONAL_DATA_FILE, "r");
    FILE* tempFile = fopen("personal_vehicle_data.tmp", "w");
    if (!sourceFile || !tempFile) {
        if (sourceFile) fclose(sourceFile);
        if (tempFile) fclose(tempFile);
        strcpy(state->queryMessage, "保存失败");
        return;
    }

    char line[512];
    int updated = 0;
    while (fgets(line, sizeof(line), sourceFile)) {
        char original[512];
        strcpy(original, line);
        char* licensePlate = strtok(line, "|\r\n");
        if (!updated && licensePlate) {
            char ansiLicensePlate[32];
            if (ConvertUtf8ToAnsi(licensePlate, ansiLicensePlate, sizeof(ansiLicensePlate)) &&
                strcmp(ansiLicensePlate, state->queryLicensePlate) == 0) {
                updated = WritePersonalRecordUtf8(tempFile, &state->queryResult);
                continue;
            }
        }
        fputs(original, tempFile);
    }
    fclose(sourceFile);
    fclose(tempFile);

    if (updated) {
        remove(PERSONAL_DATA_FILE);
        rename("personal_vehicle_data.tmp", PERSONAL_DATA_FILE);
        strcpy(state->queryMessage, "保存成功");
    } else {
        remove("personal_vehicle_data.tmp");
        strcpy(state->queryMessage, "保存失败");
    }
}

void HandlePersonalInformationKey(char key)// Helper: 处理车辆信息查询输入
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (key == 8 || key == 127) {
        switch (state->focus) {
            case 7: DeleteCharFromField(state->queryLicensePlate); state->queryFound = 0; break;
            case 8: DeleteCharFromField(state->queryResult.ownerName); break;
            case 9: DeleteCharFromField(state->queryResult.college); break;
            case 10: DeleteCharFromField(state->queryResult.personalID); break;
            case 11: DeleteCharFromField(state->queryResult.ownerPhone); break;
        }
    } else if (key == 13 || key == 10) {
        QueryPersonalVehicleInfo();
    } else {
        switch (state->focus) {
            case 7: AppendCharToField(state->queryLicensePlate, sizeof(state->queryLicensePlate), 0, key); break;
            case 8: AppendCharToField(state->queryResult.ownerName, sizeof(state->queryResult.ownerName), 1, key); break;
            case 9: AppendCharToField(state->queryResult.college, sizeof(state->queryResult.college), 2, key); break;
            case 10: AppendCharToField(state->queryResult.personalID, sizeof(state->queryResult.personalID), 3, key); break;
            case 11: AppendCharToField(state->queryResult.ownerPhone, sizeof(state->queryResult.ownerPhone), 4, key); break;
        }
    }
}

void HandlePersonalInformationChar(TCHAR key)// Helper: 处理车辆信息查询字符
{
    if (key == 8 || key == 127 || key == 13 || key == 10) {
        HandlePersonalInformationKey((char)key);
        return;
    }
    char converted[MB_LEN_MAX] = {0};
#ifdef UNICODE
    int byteCount = WideCharToMultiByte(CP_ACP, 0, &key, 1, converted, sizeof(converted), NULL, NULL);
#else
    converted[0] = (char)key;
    int byteCount = 1;
#endif
    for (int i = 0; i < byteCount; ++i)
        HandlePersonalInformationKey(converted[i]);
}
