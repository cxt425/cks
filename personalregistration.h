#ifndef _PERSONALREGISTRATION_H_  // 声明 personalregistration.h 头文件，防止重复包含
#define _PERSONALREGISTRATION_H_

#include "public.h"
#include <windows.h> 
#define INPUT_NOTHING      0
#define INPUT_OWNER_NAME   1
extern int g_nowInput;

void RegisterPageKeyHandle(TCHAR ch);
void ClearRegInput(void);

void InitPersonalRegistrationState(void);     // 初始化个人注册页面的状态信息
void HandlePersonalRegistrationKey(char key);      // 处理注册页面的键盘输入
void HandlePersonalRegistrationChar(TCHAR key);   // 处理注册页面的字符输入
void TryPersonalRegistration(void); // 尝试提交注册并校验输入
void RegisterPageKeyHandle(TCHAR ch); // Updated function signature

typedef struct {
    char licensePlate[10]; // 车牌号
    char ownerName[20];    // 车主姓名
    char college[20];      // 所属学院
    char personalID[20];    // 人员编号
    char ownerPhone[12];   // 车主手机号
    char vehicleType[20];  // 车辆类型
    char registrationDate[11]; // 注册日期，格式为 YYYY-MM-DD
    int focus;             // 当前输入框焦点，0-6
    int registered;        // 提交是否成功
    char message[64];      // 状态提示信息
} PersonalUserInfo;

int SavePersonalVehicleData(const PersonalUserInfo* info); // 保存个人电动车数据

PersonalUserInfo* GetPersonalRegistrationState(void); // 获取个人注册状态信息的指针

#endif