#include "personalscrap.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

static void GetTodayDateString(char* buffer, int size);// Helper: 获取当前日期字符串，格式为 YYYY-MM-DD

static int ConvertUtf8ToAnsi(const char* source, char* destination, int destinationSize)// Helper: 将 UTF-8 编码的字符串转换为 ANSI 编码的字符串
{
    if (!source || !destination || destinationSize <= 0) return 0;
    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, source, -1, wideText, 128);
    if (wideLength <= 0) return 0;
    return WideCharToMultiByte(CP_ACP, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

void QueryPersonalScrapStatus(void)// 查询个人车辆报废状态
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (state->scrapLicensePlate[0] == '\0' || state->scrapOwnerName[0] == '\0') {
        state->scrapFound = 0;
        state->scrapStatus[0] = '\0';
        state->scrapDate[0] = '\0';
        state->scrapReason[0] = '\0';
        strcpy(state->scrapMessage, "请输入车牌号和车主姓名");
        return;
    }

    FILE* file = fopen("personal_vehicle_data.txt", "r");
    if (!file) {
        state->scrapFound = 0;
        state->scrapStatus[0] = '\0';
        state->scrapDate[0] = '\0';
        state->scrapReason[0] = '\0';
        strcpy(state->scrapMessage, "未查询到");
        return;
    }

    char line[512];
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
        char ownerName[64] = {0};
        char vehicleStatus[32] = {0};
        char scrapDate[32] = {0};
        char reasonText[128] = {0};
        if (!ConvertUtf8ToAnsi(fields[0], licensePlate, sizeof(licensePlate)) ||
            !ConvertUtf8ToAnsi(fields[1], ownerName, sizeof(ownerName)) ||
            !ConvertUtf8ToAnsi(fields[7], vehicleStatus, sizeof(vehicleStatus))) {
            continue;
        }

        if (strcmp(licensePlate, state->scrapLicensePlate) == 0 &&strcmp(ownerName, state->scrapOwnerName) == 0) 
        {
            strcpy(state->scrapStatus, vehicleStatus);
            state->scrapFound = 1;
            if (count >= 10 && fields[9] && ConvertUtf8ToAnsi(fields[9], scrapDate, sizeof(scrapDate))) {
                strncpy(state->scrapDate, scrapDate, sizeof(state->scrapDate) - 1);
                state->scrapDate[sizeof(state->scrapDate) - 1] = '\0';// 如果有报废日期，则保存
            } else {
                state->scrapDate[0] = '\0';
            }
            if (count >= 9 && fields[8] && ConvertUtf8ToAnsi(fields[8], reasonText, sizeof(reasonText))) {
                strncpy(state->scrapReason, reasonText, sizeof(state->scrapReason) - 1);
                state->scrapReason[sizeof(state->scrapReason) - 1] = '\0';// 如果有报废原因，则保存
            } else {
                state->scrapReason[0] = '\0';
            }
            if (state->scrapDate[0] == '\0') GetTodayDateString(state->scrapDate, sizeof(state->scrapDate));
            strcpy(state->scrapMessage, "查询成功");
            fclose(file);
            return;
        }
    }

    fclose(file);
    state->scrapFound = 0;
    state->scrapStatus[0] = '\0';
    state->scrapDate[0] = '\0';
    state->scrapReason[0] = '\0';
    strcpy(state->scrapMessage, "未查询到");
}

static void GetTodayDateString(char* buffer, int size)// Helper: 获取当前日期字符串，格式为 YYYY-MM-DD
{
    time_t now = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &now);
    snprintf(buffer, size, "%04d-%02d-%02d", localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday);
}

static void DeleteCharFromField(char* dest)// Helper: 删除输入框中的最后一个字符
{
    int len = (int)strlen(dest);
    if (len <= 0) return;
    if ((unsigned char)dest[len - 1] >= 0x80 && len >= 2) len -= 2;
    else --len;
    dest[len] = '\0';
}

static void AppendCharToField(char* dest, int maxLen, int focus, char key)// Helper: 向输入框追加字符，限制输入长度和合法性
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
    if (focus == 0) {// 处理车牌号输入框
        if ((len == 0 && (key < 'A' || key > 'Z')) ||
            (len > 0 && !((key >= 'A' && key <= 'Z') ||
                          (key >= '0' && key <= '9'))) || len >= 5) return;
        dest[len] = key;
        dest[len + 1] = '\0';
        return;
    }
    if (focus == 2) {// 处理报废原因输入框
        if (!((unsigned char)key >= 0x80 || (key >= 32 && key <= 126))) return;
        dest[len] = key;
        dest[len + 1] = '\0';
        return;
    }
    if (!((unsigned char)key >= 0x80 || (key >= 32 && key <= 126))) return;
    dest[len] = key;
    dest[len + 1] = '\0';
}

void HandlePersonalScrapKey(char key)// Helper: 处理个人车辆报废页面的按键输入
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (key == 8 || key == 127) {
        if (state->focus == 0) DeleteCharFromField(state->scrapLicensePlate);
        else if (state->focus == 1) DeleteCharFromField(state->scrapOwnerName);
        else if (state->focus == 2) DeleteCharFromField(state->scrapReason);
        return;
    }
    if (key == 13 || key == 10) {
        if (state->focus == 2) {
            state->scrapReasonType = 3;
            return;
        }
        QueryPersonalScrapStatus();
        return;
    }
    if (key == 9) {// Tab 键切换焦点
        if (state->focus == 2) state->focus = 0;
        else state->focus = (state->focus == 0) ? 1 : 0;
        return;
    }

    if (state->focus == 0) {// 处理车牌号输入框
        AppendCharToField(state->scrapLicensePlate, sizeof(state->scrapLicensePlate), 0, key);
    } else if (state->focus == 1) {
        AppendCharToField(state->scrapOwnerName, sizeof(state->scrapOwnerName), 1, key);
    } else if (state->focus == 2) {
        AppendCharToField(state->scrapReason, sizeof(state->scrapReason), 2, key);
    }
}

void SavePersonalScrapUpdate(void)// Helper: 保存个人车辆报废状态更新
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (!state->scrapFound) {
        strcpy(state->scrapMessage, "请先查询车辆信息");
        return;
    }

    if (state->scrapDate[0] == '\0') {
        GetTodayDateString(state->scrapDate, sizeof(state->scrapDate));
    }

    const char* reasonText = "";
    switch (state->scrapReasonType) {
        case 0: reasonText = "车体损坏"; break;
        case 1: reasonText = "达到使用年限"; break;
        case 2: reasonText = "丢失无法找回"; break;
        case 3: reasonText = state->scrapReason[0] ? state->scrapReason : "其他原因"; break;
        default: reasonText = "其他原因"; break;
    }

    FILE* sourceFile = fopen("personal_vehicle_data.txt", "r");
    FILE* tempFile = fopen("personal_vehicle_data.tmp", "w");
    if (!sourceFile || !tempFile) {
        if (sourceFile) fclose(sourceFile);
        if (tempFile) fclose(tempFile);
        strcpy(state->scrapMessage, "保存失败");
        return;
    }

    char line[512];
    int updated = 0;
    while (fgets(line, sizeof(line), sourceFile)) {
        char original[512];
        strcpy(original, line);
        char* fields[16] = {0};
        char* field = strtok(line, "|\r\n");
        int count = 0;
        while (field && count < 16) {
            fields[count++] = field;
            field = strtok(NULL, "|\r\n");
        }
        if (count >= 8) {
            char licensePlate[32] = {0};
            char ownerName[64] = {0};
            if (ConvertUtf8ToAnsi(fields[0], licensePlate, sizeof(licensePlate)) &&
                ConvertUtf8ToAnsi(fields[1], ownerName, sizeof(ownerName)) &&
                strcmp(licensePlate, state->scrapLicensePlate) == 0 &&
                strcmp(ownerName, state->scrapOwnerName) == 0) {
                char ansiReason[128];
                char ansiStatus[32];
                char ansiDate[32];
                snprintf(ansiReason, sizeof(ansiReason), "%s", reasonText);
                snprintf(ansiStatus, sizeof(ansiStatus), "%s", "报废");
                snprintf(ansiDate, sizeof(ansiDate), "%s", state->scrapDate);

                char utf8Reason[256];
                char utf8Status[64];
                char utf8Date[64];
                wchar_t wideReason[128];
                wchar_t wideStatus[32];
                wchar_t wideDate[32];
                MultiByteToWideChar(CP_ACP, 0, ansiReason, -1, wideReason, 128);
                WideCharToMultiByte(CP_UTF8, 0, wideReason, -1, utf8Reason, sizeof(utf8Reason), NULL, NULL);
                MultiByteToWideChar(CP_ACP, 0, ansiStatus, -1, wideStatus, 32);
                WideCharToMultiByte(CP_UTF8, 0, wideStatus, -1, utf8Status, sizeof(utf8Status), NULL, NULL);
                MultiByteToWideChar(CP_ACP, 0, ansiDate, -1, wideDate, 32);
                WideCharToMultiByte(CP_UTF8, 0, wideDate, -1, utf8Date, sizeof(utf8Date), NULL, NULL);

                fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
                    fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], utf8Status, utf8Reason, utf8Date);
                updated = 1;
                continue;
            }
        }
        fputs(original, tempFile);
    }
    fclose(sourceFile);
    fclose(tempFile);

    if (updated) {
        remove("personal_vehicle_data.txt");
        rename("personal_vehicle_data.tmp", "personal_vehicle_data.txt");
        strcpy(state->scrapStatus, "报废");
        strcpy(state->scrapMessage, "报废成功");
    } else {
        remove("personal_vehicle_data.tmp");
        strcpy(state->scrapMessage, "保存失败");
    }
}

void HandlePersonalScrapChar(TCHAR key)// Helper: 处理个人车辆报废页面的字符输入
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandlePersonalScrapKey((char)key);
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
        if (state->focus == 0) {
            AppendCharToField(state->scrapLicensePlate, sizeof(state->scrapLicensePlate), 0, converted[i]);
        } else if (state->focus == 1) {
            AppendCharToField(state->scrapOwnerName, sizeof(state->scrapOwnerName), 1, converted[i]);
        } else if (state->focus == 2) {
            AppendCharToField(state->scrapReason, sizeof(state->scrapReason), 2, converted[i]);
        }
    }
}
