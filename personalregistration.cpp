#include "personalregistration.h"
#include <ctype.h>     // 包含字符处理函数的头文件
#include <stdio.h>     // 包含标准输入输出头文件
#include <stdlib.h>       // 包含标准库头文件，用于 rand() 和 srand() 函数
#include <string.h>        // 包含字符串处理函数的头文件

static PersonalUserInfo gPersonalUserInfo; // 定义全局个人用户信息结构体，用于存储注册状态和输入信息
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
// Helper: 判断当前输入框是否允许输入该字符
static int IsValidInputChar(int focus, char key)
{
    if (focus == 3 || focus == 4) // personalID, phone -> digits
        return (key >= '0' && key <= '9');
    if (focus == 6) // date -> digits and -
        return (key >= '0' && key <= '9') || key == '-';
    // 其他允许字母、数字、空格
    return (key >= '0' && key <= '9') || (key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || key == ' ';
}
// Helper: 向当前输入框追加字符，限制输入长度
static void AppendCharToField(char* dest, int maxLen, int focus, char key)
{
    int len = (int)strlen(dest);
    if (len >= maxLen - 1) return;
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
    if (strlen(s->licensePlate) < 2) { strcpy(s->message, "请输入有效车牌号"); s->registered = 0; return; }
    if (strlen(s->ownerName) < 2) { strcpy(s->message, "请输入车主姓名"); s->registered = 0; return; }
    if (strlen(s->college) < 2) { strcpy(s->message, "请输入院系信息"); s->registered = 0; return; }
    if (strlen(s->personalID) < 4) { strcpy(s->message, "请输入有效学号/工号"); s->registered = 0; return; }
    if (!IsPhoneValid(s->ownerPhone)) { strcpy(s->message, "手机号必须为11位数字"); s->registered = 0; return; }
    if (strlen(s->vehicleType) < 1) { strcpy(s->message, "请输入车辆类型"); s->registered = 0; return; }
    if (!IsDateValid(s->registrationDate)) { strcpy(s->message, "注册日期格式应为 YYYY-MM-DD"); s->registered = 0; return; }
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