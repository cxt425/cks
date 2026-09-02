#include "sharedusevehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static const char* SHARED_BICYCLE_DATA_FILE = "shared_bicycle_data.txt";
static const char* SHARED_USE_RECORD_FILE = "shared_use_records.txt";
static const char* UTF8_UNPAID_STATUS = "\xE6\x9C\xAA\xE6\x94\xAF\xE4\xBB\x98";
static int sharedOrderScroll = 0;
static int sharedOrderCount = 0;
static int sharedOrderDragging = 0;

static void SetSharedOrderScrollFromBar(int y)
{
    int maxScroll = sharedOrderCount > 8 ? sharedOrderCount - 8 : 0;
    if (maxScroll <= 0) return;

    int barTop = 155;
    int barBottom = 491;
    int thumbHeight = 336 * 8 / sharedOrderCount;
    if (thumbHeight < 24) thumbHeight = 24;
    int effective = barBottom - barTop - thumbHeight;
    int target = y - thumbHeight / 2;
    if (target < barTop) target = barTop;
    if (target > barBottom - thumbHeight) target = barBottom - thumbHeight;
    sharedOrderScroll = (target - barTop) * maxScroll / effective;
}

void ResetSharedOrderScroll(void)
{
    sharedOrderScroll = 0;
    sharedOrderDragging = 0;
}

void SetSharedOrderCount(int count)
{
    sharedOrderCount = count;
    if (sharedOrderScroll > sharedOrderCount - 8)
        sharedOrderScroll = sharedOrderCount > 8 ? sharedOrderCount - 8 : 0;
}

void ScrollSharedOrder(int offset)
{
    sharedOrderScroll += offset;
    if (sharedOrderScroll < 0) sharedOrderScroll = 0;
}

int GetSharedOrderScroll(void)
{
    return sharedOrderScroll;
}
void BeginSharedOrderDrag(int y)
{
    if (sharedOrderCount <= 8 || y < 155 || y > 491) return;
    sharedOrderDragging = 1;
    SetSharedOrderScrollFromBar(y);
}

void UpdateSharedOrderDrag(int y)
{
    if (sharedOrderDragging) SetSharedOrderScrollFromBar(y);
}

void EndSharedOrderDrag(void)
{
    sharedOrderDragging = 0;
}
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

static void AppendSharedUseRecord(const char* phone, const char* plate)
{
    if (!phone || phone[0] == '\0' || !plate || plate[0] == '\0') return;

    FILE* file = fopen(SHARED_USE_RECORD_FILE, "a");
    if (!file) return;

    char line[128];
    snprintf(line, sizeof(line), "%s|%s|--小时|--公里|--元|未支付\n", phone, plate);
    WriteUtf8Text(file, line);
    fclose(file);
}

static void UpdateSharedUseRecordPaid(const char* phone, const char* plate, const char* duration, const char* distance, const char* amount)
{
    if (!phone || phone[0] == '\0' || !plate || plate[0] == '\0') return;

    FILE* in = fopen(SHARED_USE_RECORD_FILE, "r");
    if (!in) {
        return;
    }

    FILE* out = fopen("shared_use_records.tmp", "w");
    if (!out) {
        fclose(in);
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        char buffer[256];
        strncpy(buffer, line, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        char* parts[6] = {0};
        int count = 0;
        char* token = strtok(buffer, "|\r\n");
        while (token && count < 6) {
            parts[count++] = token;
            token = strtok(NULL, "|\r\n");
        }

        if (count >= 6 && parts[0] && strcmp(parts[0], phone) == 0 &&
            parts[1] && strcmp(parts[1], plate) == 0 && parts[5] && strcmp(parts[5], UTF8_UNPAID_STATUS) == 0) {
            char newLine[256];
            snprintf(newLine, sizeof(newLine), "%s|%s|%s分钟|%s公里|%s元|已支付\n",
                     phone, plate,
                     duration[0] ? duration : "0",
                     distance[0] ? distance : "0",
                     amount);
            WriteUtf8Text(out, newLine);
            found = 1;
        }
        else {
            fputs(line, out);
        }
    }

    fclose(in);
    fclose(out);

    if (found) {
        remove(SHARED_USE_RECORD_FILE);
        rename("shared_use_records.tmp", SHARED_USE_RECORD_FILE);
    }
    else {
        remove("shared_use_records.tmp");
    }
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

static int ConvertLocalToUtf8(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;

    int wideLen = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
    if (wideLen > 0) {
        wchar_t* wide = (wchar_t*)malloc((size_t)wideLen * sizeof(wchar_t));
        if (wide) {
            MultiByteToWideChar(CP_ACP, 0, source, -1, wide, wideLen);
            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
            if (utf8Len > 0) {
                char* utf8 = (char*)malloc((size_t)utf8Len);
                if (utf8) {
                    WideCharToMultiByte(CP_UTF8, 0, wide, -1, utf8, utf8Len, NULL, NULL);
                    strncpy(destination, utf8, destinationSize - 1);
                    destination[destinationSize - 1] = '\0';
                    free(utf8);
                    free(wide);
                    return 1;
                }
            }
            free(wide);
        }
    }

    strncpy(destination, source, destinationSize - 1);
    destination[destinationSize - 1] = '\0';
    return 1;
}

void UpdateSharedBicycleRecordInFile(const char* plate, const char* status, const char* battery)
{
    if (!plate || plate[0] == '\0' || !status || !battery) return;

    FILE* in = fopen(SHARED_BICYCLE_DATA_FILE, "r");
    if (!in) return;

    FILE* out = fopen("shared_bicycle_data.tmp", "w");
    if (!out) {
        fclose(in);
        return;
    }

    char plateUtf8[32], statusUtf8[64], batteryUtf8[16];
    ConvertLocalToUtf8(plate, plateUtf8, sizeof(plateUtf8));
    ConvertLocalToUtf8(status, statusUtf8, sizeof(statusUtf8));
    ConvertLocalToUtf8(battery, batteryUtf8, sizeof(batteryUtf8));

    char line[128];
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        char buffer[128];
        strncpy(buffer, line, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        char* field = strtok(buffer, "|\r\n");
        if (field && strcmp(field, plate) == 0) {
            fprintf(out, "%s|%s|%s\n", plateUtf8, statusUtf8, batteryUtf8);
            found = 1;
        }
        else {
            fputs(line, out);
        }
    }

    if (!found) {
        fprintf(out, "%s|%s|%s\n", plateUtf8, statusUtf8, batteryUtf8);
    }

    fclose(in);
    fclose(out);

    remove(SHARED_BICYCLE_DATA_FILE);
    rename("shared_bicycle_data.tmp", SHARED_BICYCLE_DATA_FILE);
}

static void ResetSharedRideState(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    state->sharedUsePlate[0] = '\0';
    strcpy(state->sharedUseStatus, "未查询");
    strcpy(state->sharedUseBattery, "--");
    strcpy(state->sharedUseMessage, "请输入车辆编号");

    state->settlementPlate[0] = '\0';
    state->settlementDuration[0] = '\0';
    state->settlementDistance[0] = '\0';
    strcpy(state->settlementAmount, "0.8");
    strcpy(state->settlementStatus, "未支付");
    state->settlementFocus = 0;
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
        char* firstPipe = strchr(line, '|');
        if (!firstPipe) continue;

        *firstPipe = '\0';
        if (strcmp(line, state->sharedUsePlate) != 0) continue;

        char* secondPipe = strrchr(firstPipe + 1, '|');
        if (!secondPipe) continue;

        *secondPipe = '\0';
        char statusText[64];
        char batteryText[16];
        strncpy(statusText, firstPipe + 1, sizeof(statusText) - 1);
        statusText[sizeof(statusText) - 1] = '\0';
        strncpy(batteryText, secondPipe + 1, sizeof(batteryText) - 1);
        batteryText[sizeof(batteryText) - 1] = '\0';

        char* newline = strpbrk(batteryText, "\r\n");
        if (newline) *newline = '\0';

        NormalizeLocalText(state->sharedUseStatus, sizeof(state->sharedUseStatus), statusText);
        NormalizeLocalText(state->sharedUseBattery, sizeof(state->sharedUseBattery), batteryText);
        snprintf(state->sharedUseMessage, sizeof(state->sharedUseMessage), "车辆状态：%s", state->sharedUseStatus);
        found = 1;
        break;
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

    const char* finalStatus = "空闲中";
    if (strncmp(state->sharedUseStatus, "报修中", 3) == 0 || strncmp(state->sharedUseStatus, "保修中", 3) == 0) {
        finalStatus = state->sharedUseStatus;
    }

    UpdateSharedBicycleRecordInFile(state->settlementPlate, finalStatus, batteryText);

    UpdateSharedUseRecordPaid(state->phone, state->settlementPlate,
                             state->settlementDuration[0] ? state->settlementDuration : "0",
                             state->settlementDistance[0] ? state->settlementDistance : "0",
                             state->settlementAmount);

    strcpy(state->settlementStatus, "已支付");
    strcpy(state->sharedUseBattery, batteryText);
    if (strncmp(state->sharedUseStatus, "报修中", 3) == 0 || strncmp(state->sharedUseStatus, "保修中", 3) == 0) {
        strcpy(state->sharedUseStatus, finalStatus);
        strcpy(state->sharedUseMessage, "还车结算成功：报修状态已保留");
    }
    else {
        strcpy(state->sharedUseStatus, "空闲中");
        strcpy(state->sharedUseMessage, "还车结算成功：已支付");
    }

    ResetSharedRideState();
}

void TryUnlockSharedVehicle(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    if (state->sharedUsePlate[0] == '\0') {
        strcpy(state->sharedUseMessage, "请输入车辆编号");
        return;
    }

    QuerySharedVehicleInfo();
    if (strncmp(state->sharedUseStatus, "报修中", 3) == 0 || strncmp(state->sharedUseStatus, "保修中", 3) == 0) {
        strcpy(state->sharedUseMessage, "开锁失败：车辆正在报修中，无法解锁");
        return;
    }
    if (strcmp(state->sharedUseStatus, "空闲中") != 0) {
        if (strncmp(state->sharedUseStatus, "故障中", 3) == 0)
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
    state->settlementDuration[0] = '\0';
    state->settlementDistance[0] = '\0';
    strcpy(state->settlementAmount, "0.8");
    strcpy(state->settlementStatus, "未支付");
    state->settlementFocus = 0;

    strcpy(state->sharedUseStatus, "骑行中");
    UpdateSharedBicycleRecordInFile(state->sharedUsePlate, "骑行中", state->sharedUseBattery);
    AppendSharedUseRecord(state->phone, state->sharedUsePlate);
    strcpy(state->sharedUseMessage, "开锁成功：车辆已解锁");
}
