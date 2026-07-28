#include "sharedsignout.h"
#include <ctype.h>     // 包含字符处理函数的头文件
#include <stdio.h>
#include <stdlib.h>       // 包含标准库头文件，用于 rand() 和 srand() 函数
#include <string.h>        // 包含字符串处理函数的头文件
#include <time.h>         // 包含时间函数的头文件，用于获取当前时间作为随机数种子

static SharedUserInfo gSharedUserInfo;

SharedUserInfo* GetSharedSignoutState(void)      // 获取共享登录状态信息的指针
{
    return &gSharedUserInfo;
}

// 检查手机号格式是否为 11 位纯数字
static int IsPhoneValid(const char* phone)
{
    if (phone == NULL || strlen(phone) != 11)
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

    return (key >= '0' && key <= '9') || (key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z');
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
    if (len > 0)
        dest[len - 1] = '\0';
}

// 初始化共享登录页面的状态信息
void InitSharedSignoutState(void)
{
    SharedUserInfo* state = GetSharedSignoutState();
    memset(state, 0, sizeof(*state));
    state->focus = 0;
    state->codeSent = 0;
    state->loginSuccess = 0;
    strcpy(state->message, "请先输入用户名和手机号，再获取验证码");
}

// 处理登录页面的键盘输入
void HandleSharedSignoutKey(char key)
{
    SharedUserInfo* state = GetSharedSignoutState();

    if (key == 8 || key == 127) {
        if (state->focus == 0)
            DeleteCharFromField(state->username);
        else if (state->focus == 1)
            DeleteCharFromField(state->phone);
        else
            DeleteCharFromField(state->code);
        return;
    }

    if (key == 13 || key == 10) {
        TrySharedLogin();
        return;
    }

    if (key == 9) {
        state->focus = (state->focus + 1) % 3;
        return;
    }

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

    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    int codeValue = rand() % 1000000;
    sprintf(state->generatedCode, "%06d", codeValue);
    state->codeSent = 1;
    state->loginSuccess = 0;
    sprintf(state->message, "验证码已发送，当前验证码：%s", state->generatedCode);
}

// 执行登录校验，只有验证码正确且信息完整时才能成功
void TrySharedLogin(void)
{
    SharedUserInfo* state = GetSharedSignoutState();

    if (strlen(state->username) < 2) {
        strcpy(state->message, "请输入用户名");
        state->loginSuccess = 0;
        return;
    }

    if (!IsPhoneValid(state->phone)) {
        strcpy(state->message, "手机号必须为11位数字");
        state->loginSuccess = 0;
        return;
    }

    if (!state->codeSent) {
        strcpy(state->message, "请先获取验证码");
        state->loginSuccess = 0;
        return;
    }

    if (strlen(state->code) != 6) {
        strcpy(state->message, "请输入6位验证码");
        state->loginSuccess = 0;
        return;
    }

    if (strcmp(state->code, state->generatedCode) != 0) {
        strcpy(state->message, "验证码错误，请重新输入");
        state->loginSuccess = 0;
        return;
    }

    state->loginSuccess = 1;
    strcpy(state->message, "登录成功，欢迎使用共享电动车系统");
}

