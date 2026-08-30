#ifndef _SHAREDSIGNOUT_H_  // 声明 sharedsignout.h 头文件，防止重复包含
#define _SHAREDSIGNOUT_H_

#include "public.h"

void InitSharedSignoutState(void);     // 初始化共享登录页面的状态信息
void HandleSharedSignoutKey(char key);      // 处理登录页面的键盘输入
void HandleSharedSignoutChar(TCHAR key);    // 处理登录页面的中文字符输入
void GenerateSharedVerificationCode(void);    // 生成一次性验证码，并校验输入信息是否合法
void TrySharedLogin(void);       // 尝试登录共享电动车系统，检查输入信息是否合法，并更新登录状态
void LogoutSharedUser(void);     // 退出共享电动车登录状态并清空登录表单

typedef struct {
    char username[20]; // 用户名
    char phone[12];    // 手机号
    char code[7];      // 验证码，留出结尾的 '\0'
    int focus;         // 当前选中的输入框，0表示用户名，1表示手机号，2表示验证码
    int codeSent;      // 是否已经获取验证码
    int loginSuccess;  // 是否登录成功
    char generatedCode[7]; // 本次生成的验证码
    char message[64];  // 状态提示信息
    char sharedUsePlate[16];  // 输入用车页面的车辆编号
    char sharedUseStatus[32]; // 查询到的车辆状态
    char sharedUseBattery[16]; // 查询到的可用电量
    char sharedUseMessage[64]; // 开锁结果提示信息
    char settlementPlate[16]; // 还车结算页面车辆编号
    char settlementDuration[16]; // 用车时长（分钟）
    char settlementDistance[16]; // 骑行里程（公里）
    char settlementAmount[16]; // 应付金额
    char settlementStatus[16]; // 支付状态
    int settlementFocus; // 0=时长，1=里程
} SharedUserInfo;

int SaveSharedVehicleData(const SharedUserInfo* info); // 保存共享电动车数据
SharedUserInfo* GetSharedSignoutState(void);// 获取共享登录状态信息的指针

#endif