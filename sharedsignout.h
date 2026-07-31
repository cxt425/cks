#ifndef _SHAREDSIGNOUT_H_
#define _SHAREDSIGNOUT_H_

#include "public.h"

void InitSharedSignoutState(void);
void HandleSharedSignoutKey(char key);
void GenerateSharedVerificationCode(void);
void TrySharedLogin(void);
void DrawSharedSignoutPage(void);

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

SharedUserInfo* GetSharedSignoutState(void);

#endif 