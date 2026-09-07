#ifndef PERSONALINSPECTION_H
#define PERSONALINSPECTION_H

#include "public.h"
#include <windows.h>

typedef struct {
    char licensePlate[10];// 车牌号
    char ownerName[20];// 车主姓名
    char vehicleStatus[16];// 车辆状态
    char nextInspectionDate[11];// 下次年审日期
    char registrationDate[11];// 注册日期
    char originalVehicleStatus[16];// 原始车辆状态，用于比较是否有更改
    char message[64];// 用于显示查询或更新操作的提示信息
    int queryFound;// 标记查询是否成功找到车辆信息
    int statusUpdated;// 标记车辆状态是否已更新
    int focus;// 标记当前焦点所在的输入框或按钮
} PersonalInspectionState;// 个人年审管理界面状态结构体

void InitPersonalInspectionState(void);// 初始化个人年审管理界面状态
PersonalInspectionState* GetPersonalInspectionState(void);// 获取个人年审管理界面状态的指针
void HandlePersonalInspectionKey(char key);// 处理个人年审管理界面的键盘输入
void HandlePersonalInspectionChar(TCHAR key);// 处理个人年审管理界面的字符输入
void QueryPersonalInspection(void);// 查询个人年审信息
void UpdatePersonalInspectionStatus(void);// 更新个人年审状态
void SavePersonalInspectionUpdate(void);// 保存个人年审状态更新
void RestorePersonalInspectionState(void);// 恢复个人年审管理界面状态

#endif
