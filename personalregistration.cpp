#include "personalregistration.h"
#include <ctype.h>     // 包含字符处理函数的头文件
#include <stdio.h>     // 包含标准输入输出头文件
#include <stdlib.h>       // 包含标准库头文件，用于 rand() 和 srand() 函数
#include <string.h>        // 包含字符串处理函数的头文件
#include <windows.h>    
int g_nowInput = INPUT_NOTHING;
static PersonalUserInfo gPersonalUserInfo; // 定义全局个人用户信息结构体，用于存储注册状态和输入信息
static const char* PERSONAL_DATA_FILE = "personal_vehicle_data.txt";

static int ConvertAnsiToUtf8(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;

    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_ACP, 0, source, -1, wideText, sizeof(wideText) / sizeof(wideText[0]));
    if (wideLength <= 0) return 0;

    return WideCharToMultiByte(CP_UTF8, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

// 保存个人电动车数据到文件中，返回保存是否成功
int SavePersonalVehicleData(const PersonalUserInfo* info)
{
    if (!info) return 0;

    char licensePlate[128];
    char ownerName[128];
    char college[128];
    char personalID[128];
    char ownerPhone[128];
    char vehicleType[128];
    char registrationDate[128];
    if (!ConvertAnsiToUtf8(info->licensePlate, licensePlate, sizeof(licensePlate)) ||
        !ConvertAnsiToUtf8(info->ownerName, ownerName, sizeof(ownerName)) ||
        !ConvertAnsiToUtf8(info->college, college, sizeof(college)) ||
        !ConvertAnsiToUtf8(info->personalID, personalID, sizeof(personalID)) ||
        !ConvertAnsiToUtf8(info->ownerPhone, ownerPhone, sizeof(ownerPhone)) ||
        !ConvertAnsiToUtf8(info->vehicleType, vehicleType, sizeof(vehicleType)) ||
        !ConvertAnsiToUtf8(info->registrationDate, registrationDate, sizeof(registrationDate))) return 0;

    FILE* file = fopen(PERSONAL_DATA_FILE, "a");
    if (!file) return 0;
    int success = fprintf(file, "%s|%s|%s|%s|%s|%s|%s\n",
        licensePlate, ownerName, college, personalID,
        ownerPhone, vehicleType, registrationDate) >= 0;
    fclose(file);
    return success;
}
 PersonalUserInfo* GetPersonalRegistrationState(void) // 获取个人注册状态信息的指针
{
    return &gPersonalUserInfo; // 返回全局个人用户信息结构体的地址
}

// Helper: 检查手机号是否为 11 位数字
static int IsPhoneValid(const char* phone)
{
    if (!phone || strlen(phone) != 11) return 0;
    if (phone[0] != '1') return 0;
    for (int i = 0; i < 11; ++i) if (!isdigit((unsigned char)phone[i])) return 0;
    return 1;
}

// Helper: 检查日期格式 YYYY-MM-DD
static int IsDateValid(const char* d)
{
    if (!d || strlen(d) != 10) return 0;
    if (d[4] != '-' || d[7] != '-') return 0;
    for (int i = 0; i < 10; ++i) {
        if (i==4 || i==7) continue;
        if (!isdigit((unsigned char)d[i])) return 0;
    }
    return 1;
}
// Helper: 检查学号/工号格式，1位大写字母 + 9位数字
static int IsPersonalIDValid(const char* personalID)
{
    if (!personalID || strlen(personalID) != 10) return 0;
    if (personalID[0] < 'A' || personalID[0] > 'Z') return 0;
    for (int i = 1; i < 10; ++i) {
        if (personalID[i] < '0' || personalID[i] > '9') return 0;
    }
    return 1;
}
// Helper: 检查车牌号格式，首位为大写字母，后四位为大写字母或数字
static int IsLicensePlateValid(const char* licensePlate)
{
    if (!licensePlate || strlen(licensePlate) != 5) return 0;
    if (licensePlate[0] < 'A' || licensePlate[0] > 'Z') return 0;
    for (int i = 1; i < 5; ++i) {
        if (!((licensePlate[i] >= 'A' && licensePlate[i] <= 'Z') ||
              (licensePlate[i] >= '0' && licensePlate[i] <= '9'))) return 0;
    }
    return 1;
}
// Helper: 判断当前输入框是否允许输入该字符
static int IsValidInputChar(int focus, char key)
{
    if (focus == 4) // phone -> digits
        return (key >= '0' && key <= '9');
    if (focus == 6) // date -> digits and -
        return (key >= '0' && key <= '9') || key == '-';
    // 其他字段允许中文、字母、数字、空格和常用符号
    return (unsigned char)key >= 0x80 || (key >= 32 && key <= 126);
}
// Helper: 向当前输入框追加字符，限制输入长度
static void AppendCharToField(char* dest, int maxLen, int focus, char key)
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
    if (!IsValidInputChar(focus, key)) return;// 检查输入字符是否合法
    dest[len] = key;
    dest[len+1] = '\0';
}
// Helper: 删除当前输入框最后一个字符
static void DeleteCharFromField(char* dest)
{
    int len = (int)strlen(dest);
    if (len > 0) dest[len-1] = '\0';
}
// 初始化个人注册页面的状态信息
void InitPersonalRegistrationState(void)
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    memset(state, 0, sizeof(*state));// 将结构体清零，初始化所有字段为默认值
    state->focus = 0;
    state->registered = 0;
    strcpy(state->message, "请填写注册信息，按 Tab 切换输入框");
}
// 提交注册并做基本校验
void TryPersonalRegistration(void)
{
    PersonalUserInfo* s = GetPersonalRegistrationState();
    if (!IsLicensePlateValid(s->licensePlate)) { strcpy(s->message, "车牌号须为1位大写字母加4位大写字母或数字"); s->registered = 0; return; }
    if (strlen(s->ownerName) < 2) { strcpy(s->message, "请输入车主姓名"); s->registered = 0; return; }
    if (strlen(s->college) < 2) { strcpy(s->message, "请输入院系信息"); s->registered = 0; return; }
    if (!IsPersonalIDValid(s->personalID)) { strcpy(s->message, "学号格式应为1位大写字母加9位数字"); s->registered = 0; return; }
    if (!IsPhoneValid(s->ownerPhone)) { strcpy(s->message, "手机号必须为11位数字"); s->registered = 0; return; }
    if (strlen(s->vehicleType) < 1) { strcpy(s->message, "请输入车辆类型"); s->registered = 0; return; }
    if (!IsDateValid(s->registrationDate)) { strcpy(s->message, "注册日期格式应为 YYYY-MM-DD"); s->registered = 0; return; }
    if (!SavePersonalVehicleData(s)) {
        strcpy(s->message, "个人电动车数据保存失败");
        s->registered = 0;
        return;
    }
    s->registered = 1;
    strcpy(s->message, "注册成功！");
}
// 处理注册页面的键盘输入
void HandlePersonalRegistrationKey(char key)
{
    PersonalUserInfo* state = GetPersonalRegistrationState();
    if (key == 8 || key == 127) {
        // 退格
        switch (state->focus) {
            case 0: DeleteCharFromField(state->licensePlate); break;
            case 1: DeleteCharFromField(state->ownerName); break;
            case 2: DeleteCharFromField(state->college); break;
            case 3: DeleteCharFromField(state->personalID); break;
            case 4: DeleteCharFromField(state->ownerPhone); break;
            case 5: DeleteCharFromField(state->vehicleType); break;
            case 6: DeleteCharFromField(state->registrationDate); break;
        }
        return;
    }
    if (key == 13 || key == 10) { // 回车提交
        TryPersonalRegistration();
        return;
    }
    if (key == 9) { // Tab 切换焦点
        state->focus = (state->focus + 1) % 7;
        return;
    }
    // 普通字符追加
    switch (state->focus) {
        case 0: AppendCharToField(state->licensePlate, sizeof(state->licensePlate), state->focus, key); break;
        case 1: AppendCharToField(state->ownerName, sizeof(state->ownerName), state->focus, key); break;
        case 2: AppendCharToField(state->college, sizeof(state->college), state->focus, key); break;
        case 3: AppendCharToField(state->personalID, sizeof(state->personalID), state->focus, key); break;
        case 4: AppendCharToField(state->ownerPhone, sizeof(state->ownerPhone), state->focus, key); break;
        case 5: AppendCharToField(state->vehicleType, sizeof(state->vehicleType), state->focus, key); break;
        case 6: AppendCharToField(state->registrationDate, sizeof(state->registrationDate), state->focus, key); break;
    }
}

void HandlePersonalRegistrationChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandlePersonalRegistrationKey((char)key);
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

    PersonalUserInfo* state = GetPersonalRegistrationState();
    for (int i = 0; i < byteCount; ++i) {
        switch (state->focus) {
            case 0: AppendCharToField(state->licensePlate, sizeof(state->licensePlate), state->focus, converted[i]); break;
            case 1: AppendCharToField(state->ownerName, sizeof(state->ownerName), state->focus, converted[i]); break;
            case 2: AppendCharToField(state->college, sizeof(state->college), state->focus, converted[i]); break;
            case 3: AppendCharToField(state->personalID, sizeof(state->personalID), state->focus, converted[i]); break;
            case 4: AppendCharToField(state->ownerPhone, sizeof(state->ownerPhone), state->focus, converted[i]); break;
            case 5: AppendCharToField(state->vehicleType, sizeof(state->vehicleType), state->focus, converted[i]); break;
            case 6: AppendCharToField(state->registrationDate, sizeof(state->registrationDate), state->focus, converted[i]); break;
        }
    }
}

// 接收键盘字符，转换为当前程序使用的 GBK 字节并写入车主姓名
void RegisterPageKeyHandle(TCHAR ch)
{
    PersonalUserInfo* regState = GetPersonalRegistrationState();

    //没有选中任何输入框，直接退出，不处理按键
    if(g_nowInput == INPUT_NOTHING)
        return;

    // 退格键
    if (ch == 8)
    {
        if(g_nowInput == INPUT_OWNER_NAME)
        {
            int len = (int)strlen(regState->ownerName);
            if(len > 0)
            {
                regState->ownerName[len - 1] = '\0';
            }
        }
        return;
    }

    // 回车交给原有提交逻辑
    if(ch == 13)
    {
        return;
    }

    char buf[MB_LEN_MAX] = {0};
    int byteCount = 0;
#ifdef UNICODE
    byteCount = WideCharToMultiByte(CP_ACP, 0, &ch, 1, buf, sizeof(buf), NULL, NULL);
#else
    buf[0] = (char)ch;
    byteCount = 1;
#endif
    if (byteCount <= 0) return;

    // 把转换后的中文或普通字符追加到车主姓名
    if(g_nowInput == INPUT_OWNER_NAME)
    {
        int curLen = (int)strlen(regState->ownerName);
        if(curLen + byteCount < (int)sizeof(regState->ownerName))
        {
            memcpy(regState->ownerName + curLen, buf, byteCount);
            regState->ownerName[curLen + byteCount] = '\0';
        }
    }
}

//切换到注册页面清空输入
void ClearRegInput(void)
{
    PersonalUserInfo* regState = GetPersonalRegistrationState();
    memset(regState->ownerName, 0, sizeof(regState->ownerName));
    g_nowInput = INPUT_NOTHING;
}
