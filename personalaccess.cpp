#include "personalaccess.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

static const int ACCESS_TIME_MAX_LEN = 11;// 最大长度为 YYYY-MM-DD 格式的字符串长度为 10，加上终止符为 11

static int IsAccessTimeValid(const char* timeStr)// 检查时间字符串是否为有效的 YYYY-MM-DD 格式
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

static int IsAccessTimeKeyAllowed(const char* dest, char key)// 检查当前输入框是否允许输入该字符，确保时间格式为 YYYY-MM-DD
{
    int len = (int)strlen(dest);
    if (len >= ACCESS_TIME_MAX_LEN) return 0;
    if ((len == 4 || len == 7) && key == '-') return 1;
    if ((len == 4 || len == 7) && key != '-') return 0;
    if (key < '0' || key > '9') return 0;
    return 1;// 允许输入数字和特定位置的 '-'，确保时间格式为 YYYY-MM-DD
}

static void AppendAccessTimeChar(char* dest, int maxLen, char key)// 向时间输入框追加字符，限制输入长度和格式
{
    int len = (int)strlen(dest);
    if (len >= ACCESS_TIME_MAX_LEN || len >= maxLen - 1) return;
    if (!IsAccessTimeKeyAllowed(dest, key)) return;
    dest[len] = key;
    dest[len + 1] = '\0';
}

static void AppendAccessPlateChar(char* dest, int maxLen, char key)// 向车牌号输入框追加字符，限制输入长度和格式
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
    if ((len == 0 && (key < 'A' || key > 'Z')) ||
        (len > 0 && !((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))) || len >= 5) return;
    dest[len] = key;
    dest[len + 1] = '\0';
}

static int ConvertUtf8ToAnsi(const char* source, char* destination, int destinationSize)// 将 UTF-8 编码的字符串转换为 ANSI 编码的字符串
{
    if (!source || !destination || destinationSize <= 0) return 0;
    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, source, -1, wideText, 128);
    if (wideLength <= 0) return 0;
    return WideCharToMultiByte(CP_ACP, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

static int ConvertAnsiToUtf8(const char* source, char* destination, int destinationSize)// 将 ANSI 编码的字符串转换为 UTF-8 编码的字符串
{
    if (!source || !destination || destinationSize <= 0) return 0;
    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_ACP, 0, source, -1, wideText, 128);
    if (wideLength <= 0) return 0;
    return WideCharToMultiByte(CP_UTF8, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

static int GetVehicleStatusByPlate(const char* plate, char* statusOut, int statusSize)// 根据车牌号获取车辆状态，返回是否找到匹配的车辆
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

static void RefreshAccessScrapCheck(void)// 检查当前输入的车牌号是否已报废，并更新状态提示信息
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (state->accessLicensePlate[0] == '\0') {// 如果车牌号为空，则清空状态提示信息
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

void SubmitPersonalAccessRecord(void)// 提交个人电动车出入记录，保存到文件中
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (state->accessType[0] == '\0' ||
        (strcmp(state->accessType, "入校") != 0 && strcmp(state->accessType, "出校") != 0)) {
        state->accessTypeSelected = 0;
        strcpy(state->accessType, "入校");
    }
    RefreshAccessScrapCheck();// 检查当前输入的车牌号是否已报废，并更新状态提示信息
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
    while (fgets(line, sizeof(line), file))
    {
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
            !ConvertUtf8ToAnsi(fields[7], tempStatus, sizeof(tempStatus))) 
            {
            continue;
            }
        if (strcmp(licensePlate, state->accessLicensePlate) == 0) 
        {
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

void HandlePersonalAccessKey(char key)// 处理个人电动车出入记录页面的键盘输入
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (key == 8 || key == 127) {// 处理退格键，删除当前输入框的最后一个字符
        if (state->accessFocus == 0) {
            int len = (int)strlen(state->accessLicensePlate);
            if (len > 0) state->accessLicensePlate[len - 1] = '\0';
        } else if (state->accessFocus == 1) {
            int len = (int)strlen(state->accessTime);
            if (len > 0) state->accessTime[len - 1] = '\0';
        }
        return;
    }
    if (key == 13 || key == 10) {// 处理回车键，提交当前输入的出入记录
        SubmitPersonalAccessRecord();
        return;
    }
    if (key == 9) {// 处理 Tab 键，切换输入框焦点
        state->accessFocus = (state->accessFocus == 0) ? 1 : 0;
        return;
    }

    if (state->accessFocus == 0) {// 当前焦点在车牌号输入框，处理车牌号输入
        AppendAccessPlateChar(state->accessLicensePlate, sizeof(state->accessLicensePlate), key);
        RefreshAccessScrapCheck();
    } else {
        if ((key >= '0' && key <= '9') || key == '-' || key == ' ' || key == ':') {// 当前焦点在时间输入框，处理时间输入
            AppendAccessTimeChar(state->accessTime, sizeof(state->accessTime), key);
        }
    }
}

void HandlePersonalAccessChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {// 处理退格、回车和 Tab 键
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

static void NormalizeQueryPlate(char* plate)// 将车牌号转换为大写字母，确保查询时不区分大小写
{
    if (!plate) return;
    int len = (int)strlen(plate);
    for (int i = 0; i < len; ++i) {
        if (plate[i] >= 'a' && plate[i] <= 'z') plate[i] = plate[i] + 'A' - 'a';
    }
}

void QueryPersonalAccessRecords(void)// 查询个人电动车出入记录，根据输入的车牌号从文件中读取匹配的记录
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    NormalizeQueryPlate(state->accessQueryPlate);
    state->accessQueryCount = 0;
    state->accessQueryScroll = 0;
    state->accessQueryMessage[0] = '\0';
    for (int i = 0; i < 12; ++i) state->accessQueryRecords[i][0] = '\0';

    if (state->accessQueryPlate[0] == '\0') {
        strcpy(state->accessQueryMessage, "请输入车牌号");
        return;
    }

    FILE* file = fopen("access_records.txt", "r");
    if (!file) {
        strcpy(state->accessQueryMessage, "暂无出入记录");
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file) && count < 12) {
        char record[128] = {0};
        char plate[32] = {0};
        char owner[64] = {0};
        char type[16] = {0};
        char time[32] = {0};

        if (sscanf(line, "%31[^|]|%63[^|]|%15[^|]|%31[^|\n]", plate, owner, type, time) != 4) {
            continue;
        }

        if (strcmp(plate, state->accessQueryPlate) != 0) {
            continue;
        }

        snprintf(record, sizeof(record), "%s|%s|%s|%s", plate, owner, type, time);
        snprintf(state->accessQueryRecords[count], 64, "%s", record);
        count++;
    }
    fclose(file);

    state->accessQueryCount = count;
    if (count == 0) {
        strcpy(state->accessQueryMessage, "未查询到该车出入记录");
    } else {
        strcpy(state->accessQueryMessage, "查询成功");
    }
}

void HandlePersonalAccessQueryKey(char key)// 处理个人电动车出入记录查询页面的键盘输入
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (key == 8 || key == 127) {// 处理退格键，删除查询车牌号输入框的最后一个字符
        int len = (int)strlen(state->accessQueryPlate);
        if (len > 0) state->accessQueryPlate[len - 1] = '\0';
        return;
    }
    if (key == 13 || key == 10) {// 处理回车键，执行查询操作
        QueryPersonalAccessRecords();
        return;
    }
    if (key == 9) {// 处理Tab键，切换焦点
        state->accessQueryFocus = 1 - state->accessQueryFocus;
        return;
    }
    if (state->accessQueryFocus == 0) {// 当前焦点在查询车牌号输入框，处理车牌号输入
        if (strlen(state->accessQueryPlate) < 9) {
            char ch = key;
            if (ch >= 'a' && ch <= 'z') ch = ch + 'A' - 'a';
            if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
                int len = (int)strlen(state->accessQueryPlate);
                state->accessQueryPlate[len] = ch;// 将输入的字符追加到查询车牌号输入框中
                state->accessQueryPlate[len + 1] = '\0';
            }
        }
    }
}

void HandlePersonalAccessQueryChar(TCHAR key)// 处理个人电动车出入记录查询页面的字符输入
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandlePersonalAccessQueryKey((char)key);
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

    for (int i = 0; i < byteCount; ++i) {
        char ch = converted[i];
        if (ch >= 'a' && ch <= 'z') ch = ch + 'A' - 'a';
        if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) {
            if (strlen(GetPersonalRegistrationState()->accessQueryPlate) < 9) {
                int len = (int)strlen(GetPersonalRegistrationState()->accessQueryPlate);
                GetPersonalRegistrationState()->accessQueryPlate[len] = ch;
                GetPersonalRegistrationState()->accessQueryPlate[len + 1] = '\0';
            }
        }
    }
}

void ScrollPersonalAccessQuery(int delta)// 滚动个人电动车出入记录查询结果，delta 为滚动的行数，正数向下滚动，负数向上滚动
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (state->accessQueryCount <= 4) {
        state->accessQueryScroll = 0;
        return;
    }
    int maxScroll = state->accessQueryCount - 4;// 计算最大滚动偏移量，确保不会滚动超过查询结果的范围
    state->accessQueryScroll += delta;// 根据 delta 调整滚动偏移量
    if (state->accessQueryScroll < 0) state->accessQueryScroll = 0;
    if (state->accessQueryScroll > maxScroll) state->accessQueryScroll = maxScroll;
}
