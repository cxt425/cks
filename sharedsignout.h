#ifndef _SHAREDSIGNOUT_H_  // 声明 sharedsignout.h 头文件，防止重复包含
#define _SHAREDSIGNOUT_H_

#include "public.h"

void InitSharedSignoutState(void);     // 初始化共享登录页面的状态信息
void HandleSharedSignoutKey(char key);      // 处理登录页面的键盘输入
void HandleSharedSignoutChar(TCHAR key);    // 处理登录页面的中文字符输入
void GenerateSharedVerificationCode(void);    // 生成一次性验证码，并校验输入信息是否合法
void TrySharedLogin(void);       // 尝试登录共享电动车系统，检查输入信息是否合法，并更新登录状态

typedef struct {
    char username[20]; // 用户名
    char phone[12];    // 手机号
    char code[7];      // 验证码，留出结尾的 '\0'
    int focus;         // 当前选中的输入框，0表示用户名，1表示手机号，2表示验证码
    int codeSent;      // 是否已经获取验证码
    int loginSuccess;  // 是否登录成功
    char generatedCode[7]; // 本次生成的验证码
    char message[64];  // 状态提示信息
} SharedUserInfo;

int SaveSharedVehicleData(const SharedUserInfo* info); // 保存共享电动车数据

SharedUserInfo* GetSharedSignoutState(void);// 获取共享登录状态信息的指针

#endif