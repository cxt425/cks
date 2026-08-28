#ifndef _PERSONALREGISTRATION_H_  // 声明 personalregistration.h 头文件，防止重复包含
#define _PERSONALREGISTRATION_H_

#include "public.h"
#include <windows.h> 

void InitPersonalRegistrationState(void);     // 初始化个人注册页面的状态信息
void HandlePersonalRegistrationKey(char key);      // 处理注册页面的键盘输入
void HandlePersonalRegistrationChar(TCHAR key);   // 处理注册页面的字符输入
void HandlePersonalInformationKey(char key);      // 处理车辆信息查询输入
void HandlePersonalInformationChar(TCHAR key);    // 处理车辆信息查询字符
void QueryPersonalVehicleInfo(void);              // 按车牌号查询车辆信息
void UpdatePersonalVehicleInfo(void);             // 更新 TXT 中的个人车辆信息
void TryPersonalRegistration(void); // 尝试提交注册并校验输入

typedef struct {
    char licensePlate[10];
    char ownerName[20];
    char college[20];
    char personalID[20];
    char ownerPhone[12];
    char vehicleType[20];
    char registrationDate[11];
    char vehicleStatus[16];
} PersonalVehicleRecord;

typedef struct {
    char licensePlate[10]; // 车牌号
    char ownerName[20];    // 车主姓名
    char college[20];      // 所属学院
    char personalID[20];    // 人员编号
    char ownerPhone[12];   // 车主手机号
    char vehicleType[20];  // 车辆类型
    char registrationDate[11]; // 注册日期，格式为 YYYY-MM-DD
    char vehicleStatus[16]; // 车辆状态：正常、待审核、报废
    char queryLicensePlate[10]; // 信息变更页面查询车牌号
    int queryFound;             // 是否查询到车辆
    PersonalVehicleRecord queryResult; // 查询到的车辆信息
    char queryMessage[64];      // 车辆信息变更页面提示信息
    int focus;             // 当前输入框焦点，0-6
    int registered;        // 提交是否成功
    char message[64];      // 状态提示信息
} PersonalUserInfo;

int SavePersonalVehicleData(const PersonalUserInfo* info); // 保存个人电动车数据

PersonalUserInfo* GetPersonalRegistrationState(void); // 获取个人注册状态信息的指针

#endif