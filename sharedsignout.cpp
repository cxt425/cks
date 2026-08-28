#include "sharedsignout.h"
#include <ctype.h>     // 包含字符处理函数的头文件
#include <stdio.h>
#include <stdlib.h>       // 包含标准库头文件，用于 rand() 和 srand() 函数
#include <string.h>        // 包含字符串处理函数的头文件
#include <time.h>         // 包含时间函数的头文件，用于获取当前时间作为随机数种子
#include <limits.h>

static SharedUserInfo gSharedUserInfo;// 定义全局共享用户信息结构体，用于存储登录状态和输入信息
static const char* SHARED_DATA_FILE = "shared_vehicle_data.txt";

static int ConvertAnsiToUtf8(const char* source, char* destination, int destinationSize)
{
    if (!source || !destination || destinationSize <= 0) return 0;

    wchar_t wideText[128];
    int wideLength = MultiByteToWideChar(CP_ACP, 0, source, -1, wideText, sizeof(wideText) / sizeof(wideText[0]));
    if (wideLength <= 0) return 0;

    return WideCharToMultiByte(CP_UTF8, 0, wideText, -1, destination, destinationSize, NULL, NULL) > 0;
}

int SaveSharedVehicleData(const SharedUserInfo* info)
{
    if (!info) return 0;

    char username[128];
    char phone[128];
    if (!ConvertAnsiToUtf8(info->username, username, sizeof(username)) ||
        !ConvertAnsiToUtf8(info->phone, phone, sizeof(phone))) return 0;

    FILE* file = fopen(SHARED_DATA_FILE, "a");
    if (!file) return 0;
    int success = fprintf(file, "%s|%s\n", username, phone) >= 0;
    fclose(file);
    return success;
}
SharedUserInfo* GetSharedSignoutState(void)      // 获取共享登录状态信息的指针
{
    return &gSharedUserInfo;// 返回全局共享用户信息结构体的地址
}

// 检查手机号格式是否为 11 位纯数字
static int IsPhoneValid(const char* phone)
{
    if (phone == NULL || strlen(phone) != 11)       // 检查手机号是否为 NULL 或长度不为 11
        return 0;
    if (phone[0] != '1')         // 检查手机号首位是否为 '1'
        return 0;
    for (int i = 0; i < 11; ++i) {
        if (!isdigit((unsigned char)phone[i]))         // 检查每个字符是否为数字
            return 0;
    }
    return 1;
}

// 判断当前输入框是否允许输入该字符
static int IsValidInputChar(int focus, char key)
{
    if (focus == 1 || focus == 2) // 手机号和验证码只允许数字
        return (key >= '0' && key <= '9');

    // 用户名允许中文（GBK 双字节）、字母、数字、空格和常用符号
    return (unsigned char)key >= 0x80 || (key >= 32 && key <= 126);
}

// 向当前输入框追加字符，限制输入长度
static void AppendCharToField(char* dest, int maxLen, int focus, char key)
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1)
        return;

    if (IsValidInputChar(focus, key)) {
        dest[len] = key;
        dest[len + 1] = '\0';
    }
}

// 删除当前输入框最后一个字符
static void DeleteCharFromField(char* dest)
{
    int len = (int)strlen(dest);
    if (len <= 0) return;
    if ((unsigned char)dest[len - 1] >= 0x80 && len >= 2) len -= 2;
    else --len;
    dest[len] = '\0';
}

void HandleSharedSignoutChar(TCHAR key)
{
    if (key == 8 || key == 127 || key == 13 || key == 10 || key == 9) {
        HandleSharedSignoutKey((char)key);
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
        if (state->focus == 0)
            AppendCharToField(state->username, sizeof(state->username), state->focus, converted[i]);
        else if (state->focus == 1)
            AppendCharToField(state->phone, sizeof(state->phone), state->focus, converted[i]);
        else
            AppendCharToField(state->code, sizeof(state->code), state->focus, converted[i]);
    }
}

// 初始化共享登录页面的状态信息
void InitSharedSignoutState(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    memset(state, 0, sizeof(*state));       // 将结构体清零，初始化所有字段为默认值
    state->focus = 0;
    state->codeSent = 0;
    state->loginSuccess = 0;
    strcpy(state->message, "请先输入用户名和手机号，再获取验证码");
}

// 处理登录页面的键盘输入
void HandleSharedSignoutKey(char key)
{
    SharedUserInfo* state = GetSharedSignoutState();
// 处理退格键和删除键
    if (key == 8 || key == 127) {
        if (state->focus == 0)
            DeleteCharFromField(state->username);
        else if (state->focus == 1)
            DeleteCharFromField(state->phone);
        else
            DeleteCharFromField(state->code);
        return;
    }
// 处理回车键和换行键，尝试登录
    if (key == 13 || key == 10) {
        TrySharedLogin();
        return;
    }
// 处理 Tab 键，切换输入框焦点
    if (key == 9) {
        state->focus = (state->focus + 1) % 3;         // 切换焦点到下一个输入框
        return;
    }
// 处理其他字符输入，根据当前焦点追加到对应输入框
    if (state->focus == 0)
        AppendCharToField(state->username, sizeof(state->username), state->focus, key);
    else if (state->focus == 1)
        AppendCharToField(state->phone, sizeof(state->phone), state->focus, key);
    else
        AppendCharToField(state->code, sizeof(state->code), state->focus, key);
}
// 生成一次性验证码，并校验输入信息是否合法
void GenerateSharedVerificationCode(void)
{
    SharedUserInfo* state = GetSharedSignoutState();

    if (strlen(state->username) < 2 || !IsPhoneValid(state->phone)) {
        strcpy(state->message, "请输入有效用户名和11位手机号");
        state->codeSent = 0;
        state->loginSuccess = 0;
        return;
    }
// 生成随机验证码，确保每次生成的验证码不同
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));    // 使用当前时间作为随机数种子
        seeded = 1;
    }
// 生成一个 6 位的随机验证码
    int codeValue = rand() % 1000000;
    sprintf(state->generatedCode, "%06d", codeValue);// 将验证码格式化为 6 位字符串，前面补零
    state->codeSent = 1;
    state->loginSuccess = 0;
    sprintf(state->message, "验证码已发送，当前验证码：%s", state->generatedCode);// 显示生成的验证码，实际应用中应通过短信发送给用户
}

// 执行登录校验，只有验证码正确且信息完整时才能成功
void TrySharedLogin(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
// 检查用户名长度是否至少为 2 个字符
    if (strlen(state->username) < 2) {
        strcpy(state->message, "请输入用户名");
        state->loginSuccess = 0;
        return;
    }
// 检查手机号格式是否为 11 位纯数字
    if (!IsPhoneValid(state->phone)) {
        strcpy(state->message, "手机号必须为11位数字");
        state->loginSuccess = 0;
        return;
    }
// 检查验证码是否已经发送
    if (!state->codeSent) {
        strcpy(state->message, "请先获取验证码");
        state->loginSuccess = 0;
        return;
    }
// 检查验证码长度是否为 6 位
    if (strlen(state->code) != 6) {
        strcpy(state->message, "请输入6位验证码");
        state->loginSuccess = 0;
        return;
    }
// 检查输入的验证码是否与生成的验证码匹配
    if (strcmp(state->code, state->generatedCode) != 0) {
        strcpy(state->message, "验证码错误，请重新输入");
        state->loginSuccess = 0;
        return;
    }
// 如果所有检查通过，登录成功
    state->loginSuccess = 1;
    // 尝试保存共享电动车数据，如果保存失败则更新状态信息
    if (!SaveSharedVehicleData(state)) {
        state->loginSuccess = 0;
        strcpy(state->message, "共享电动车数据保存失败");
        return;
    }
    strcpy(state->message, "登录成功，欢迎使用共享电动车系统");
}