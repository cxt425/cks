#include "sharedusevehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static const char* SHARED_BICYCLE_DATA_FILE = "shared_bicycle_data.txt";
static const char* SHARED_USE_RECORD_FILE = "shared_use_records.txt";

static void WriteUtf8Text(FILE* file, const char* text)
{
    if (!file || !text) return;

    int wideLen = MultiByteToWideChar(CP_ACP, 0, text, -1, NULL, 0);
    if (wideLen > 0) {
        wchar_t* wide = (wchar_t*)malloc((size_t)wideLen * sizeof(wchar_t));
        if (wide) {
            MultiByteToWideChar(CP_ACP, 0, text, -1, wide, wideLen);
            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
            if (utf8Len > 0) {
                char* utf8 = (char*)malloc((size_t)utf8Len);
                if (utf8) {
                    WideCharToMultiByte(CP_UTF8, 0, wide, -1, utf8, utf8Len, NULL, NULL);
                    fwrite(utf8, 1, (size_t)utf8Len - 1, file);
                    free(utf8);
                }
            }
            free(wide);
            return;
        }
    }

    fwrite(text, 1, strlen(text), file);
}

static void AppendSharedUseRecord(const char* plate)
{
    if (!plate || plate[0] == '\0') return;

    FILE* file = fopen(SHARED_USE_RECORD_FILE, "a");
    if (!file) return;

    char line[128];
    snprintf(line, sizeof(line), "%s|--小时|--公里|--元|未支付\n", plate);
    WriteUtf8Text(file, line);
    fclose(file);
}

static const char* SHARED_SETTLEMENT_RECORD_FILE = "shared_use_records.txt";

static void AppendSharedUsePlateChar(char* dest, int maxLen, char key)
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
    if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') || (key >= '0' && key <= '9')) {
        dest[len] = key;
        dest[len + 1] = '\0';
    }
}

static void AppendSharedNumericChar(char* dest, int maxLen, char key)
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
    if ((key >= '0' && key <= '9') || key == '.') {
        dest[len] = key;
        dest[len + 1] = '\0';
    }
}

static void DeleteSharedNumericChar(char* dest)
{
    int len = (int)strlen(dest);
    if (len <= 0) return;
    dest[len - 1] = '\0';
}

static void DeleteSharedUsePlateChar(char* dest)
{
    int len = (int)strlen(dest);
    if (len <= 0) return;
    dest[len - 1] = '\0';
}

void HandleSharedUseVehicleKey(char key)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (key == 8 || key == 127) {
        DeleteSharedUsePlateChar(state->sharedUsePlate);
        strcpy(state->sharedUseStatus, "未查询");
        strcpy(state->sharedUseBattery, "--");
        return;
    }
    if (key == 13 || key == 10 || key == 9) {
        QuerySharedVehicleInfo();
        return;
    }
    if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') || (key >= '0' && key <= '9')) {
        AppendSharedUsePlateChar(state->sharedUsePlate, sizeof(state->sharedUsePlate), key);
    }
}

void HandleSharedUseVehicleChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandleSharedUseVehicleKey((char)key);
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
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
            AppendSharedUsePlateChar(state->sharedUsePlate, sizeof(state->sharedUsePlate), ch);
        }
    }
}

static int ParseBatteryPercent(const char* batteryText)
{
    if (!batteryText || batteryText[0] == '\0') return -1;

    char tmp[16] = {0};
    int j = 0;
    for (int i = 0; batteryText[i] != '\0' && j < 15; ++i) {
        if (batteryText[i] >= '0' && batteryText[i] <= '9') {
            tmp[j++] = batteryText[i];
        }
    }
    tmp[j] = '\0';
    if (tmp[0] == '\0') return -1;
    return atoi(tmp);
}

static void UpdateSharedBicycleRecordInFile(const char* plate, const char* status, const char* battery)
{
    if (!plate || plate[0] == '\0' || !status || !battery) return;

    FILE* in = fopen(SHARED_BICYCLE_DATA_FILE, "r");
    if (!in) return;

    FILE* out = fopen("shared_bicycle_data.tmp", "w");
    if (!out) {
        fclose(in);
        return;
    }

    char line[128];
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        char buffer[128];
        strncpy(buffer, line, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        char* field = strtok(buffer, "|\r\n");
        if (field && strcmp(field, plate) == 0) {
            fprintf(out, "%s|%s|%s\n", plate, status, battery);
            found = 1;
        }
        else {
            fputs(line, out);
        }
    }

    if (!found) {
        fprintf(out, "%s|%s|%s\n", plate, status, battery);
    }

    fclose(in);
    fclose(out);

    remove(SHARED_BICYCLE_DATA_FILE);
    rename("shared_bicycle_data.tmp", SHARED_BICYCLE_DATA_FILE);
}

static void NormalizeLocalText(char* dest, size_t destSize, const char* src)
{
    if (!dest || destSize == 0) return;
    dest[0] = '\0';
    if (!src) return;

    int utf8Len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    if (utf8Len > 0) {
        wchar_t* wide = (wchar_t*)malloc((size_t)utf8Len * sizeof(wchar_t));
        if (wide) {
            MultiByteToWideChar(CP_UTF8, 0, src, -1, wide, utf8Len);
            int localLen = WideCharToMultiByte(CP_ACP, 0, wide, -1, dest, (int)destSize, NULL, NULL);
            free(wide);
            if (localLen > 0) return;
        }
    }

    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
}

void QuerySharedVehicleInfo(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (state->sharedUsePlate[0] == '\0') {
        strcpy(state->sharedUseStatus, "未查询");
        strcpy(state->sharedUseBattery, "--");
        strcpy(state->sharedUseMessage, "请输入车辆编号");
        return;
    }

    FILE* file = fopen(SHARED_BICYCLE_DATA_FILE, "r");
    if (!file) {
        strcpy(state->sharedUseStatus, "数据文件不存在");
        strcpy(state->sharedUseBattery, "--");
        strcpy(state->sharedUseMessage, "数据文件不存在");
        return;
    }

    char line[128];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char* field[3] = {0};
        char* ptr = strtok(line, "|\r\n");
        int index = 0;
        while (ptr && index < 3) {
            field[index++] = ptr;
            ptr = strtok(NULL, "|\r\n");
        }
        if (index >= 3 && field[0] && field[1] && field[2]) {
            if (strcmp(field[0], state->sharedUsePlate) == 0) {
                NormalizeLocalText(state->sharedUseStatus, sizeof(state->sharedUseStatus), field[1]);
                NormalizeLocalText(state->sharedUseBattery, sizeof(state->sharedUseBattery), field[2]);
                snprintf(state->sharedUseMessage, sizeof(state->sharedUseMessage), "车辆状态：%s", state->sharedUseStatus);
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    if (!found) {
        strcpy(state->sharedUseStatus, "未找到");
        strcpy(state->sharedUseBattery, "--");
        strcpy(state->sharedUseMessage, "未找到该车辆");
    }
}

static void UpdateSettlementAmount(SharedUserInfo* state)
{
    if (!state) return;

    int minutes = atoi(state->settlementDuration);
    double distance = atof(state->settlementDistance);
    (void)distance;

    double fee = 0.8;
    if (minutes > 20) {
        int extraBlocks = (minutes - 20 + 9) / 10;
        fee = 0.8 + extraBlocks;
        if (fee > 5.8) fee = 5.8;
    }

    snprintf(state->settlementAmount, sizeof(state->settlementAmount), "%.1f", fee);
}

void HandleSharedSettlementKey(char key)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (key == 8 || key == 127) {
        if (state->settlementFocus == 0) DeleteSharedNumericChar(state->settlementDuration);
        else DeleteSharedNumericChar(state->settlementDistance);
        UpdateSettlementAmount(state);
        return;
    }
    if (key == 9) {
        state->settlementFocus = (state->settlementFocus + 1) % 2;
        return;
    }
    if (key == 13 || key == 10) {
        UpdateSettlementAmount(state);
        return;
    }

    if (state->settlementFocus == 0) {
        if ((key >= '0' && key <= '9')) {
            AppendSharedNumericChar(state->settlementDuration, sizeof(state->settlementDuration), key);
            UpdateSettlementAmount(state);
        }
    }
    else {
        if ((key >= '0' && key <= '9') || key == '.') {
            AppendSharedNumericChar(state->settlementDistance, sizeof(state->settlementDistance), key);
            UpdateSettlementAmount(state);
        }
    }
}

void HandleSharedSettlementChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandleSharedSettlementKey((char)key);
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
        if (state->settlementFocus == 0 && ch >= '0' && ch <= '9') {
            AppendSharedNumericChar(state->settlementDuration, sizeof(state->settlementDuration), ch);
            UpdateSettlementAmount(state);
        }
        else if (state->settlementFocus == 1 && ((ch >= '0' && ch <= '9') || ch == '.')) {
            AppendSharedNumericChar(state->settlementDistance, sizeof(state->settlementDistance), ch);
            UpdateSettlementAmount(state);
        }
    }
}

void ConfirmSharedSettlementPayment(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (state->settlementPlate[0] == '\0') return;

    UpdateSettlementAmount(state);

    double distance = atof(state->settlementDistance);
    int currentBattery = ParseBatteryPercent(state->sharedUseBattery);
    int batteryAfterRide = currentBattery;
    if (currentBattery >= 0) {
        int consume = (int)(distance + 0.5f);
        if (consume < 0) consume = 0;
        batteryAfterRide = currentBattery - consume;
        if (batteryAfterRide < 0) batteryAfterRide = 0;
    }

    char batteryText[16];
    snprintf(batteryText, sizeof(batteryText), "%d%%", batteryAfterRide >= 0 ? batteryAfterRide : 0);
    UpdateSharedBicycleRecordInFile(state->settlementPlate, "空闲中", batteryText);

    FILE* file = fopen(SHARED_SETTLEMENT_RECORD_FILE, "a");
    if (!file) return;

    char line[256];
    snprintf(line, sizeof(line), "%s|%s分钟|%s公里|%s元|已支付\n",
             state->settlementPlate,
             state->settlementDuration[0] ? state->settlementDuration : "0",
             state->settlementDistance[0] ? state->settlementDistance : "0",
             state->settlementAmount);
    WriteUtf8Text(file, line);
    fclose(file);

    strcpy(state->sharedUseStatus, "空闲中");
    strcpy(state->sharedUseBattery, batteryText);
    strcpy(state->settlementStatus, "已支付");
    strcpy(state->sharedUseMessage, "还车结算成功：已支付");
}

void TryUnlockSharedVehicle(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (state->sharedUsePlate[0] == '\0') {
        strcpy(state->sharedUseMessage, "请输入车辆编号");
        return;
    }

    QuerySharedVehicleInfo();
    if (strcmp(state->sharedUseStatus, "空闲中") != 0) {
        if (strcmp(state->sharedUseStatus, "故障中") == 0)
            strcpy(state->sharedUseMessage, "开锁失败：车辆状态为故障中");
        else if (strcmp(state->sharedUseStatus, "骑行中") == 0)
            strcpy(state->sharedUseMessage, "开锁失败：车辆正在骑行中");
        else
            strcpy(state->sharedUseMessage, "开锁失败：车辆状态不可用");
        return;
    }

    int battery = ParseBatteryPercent(state->sharedUseBattery);
    if (battery >= 0 && battery < 20) {
        strcpy(state->sharedUseMessage, "开锁失败：电量不足（少于20%）");
        return;
    }

    strcpy(state->settlementPlate, state->sharedUsePlate);
    strcpy(state->settlementDuration, "0");
    strcpy(state->settlementDistance, "0");
    strcpy(state->settlementAmount, "0.8");
    strcpy(state->settlementStatus, "未支付");
    state->settlementFocus = 0;

    strcpy(state->sharedUseStatus, "骑行中");
    UpdateSharedBicycleRecordInFile(state->sharedUsePlate, "骑行中", state->sharedUseBattery);
    AppendSharedUseRecord(state->sharedUsePlate);
    strcpy(state->sharedUseMessage, "开锁成功：车辆已解锁");
}
