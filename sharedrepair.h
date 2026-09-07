#ifndef SHAREDREPAIR_H
#define SHAREDREPAIR_H

#include "sharedsignout.h"

void ResetSharedRepairState(void);// 重置共享车辆报修界面状态
void HandleSharedRepairKey(char key);// 处理共享车辆报修界面的键盘输入
void HandleSharedRepairChar(TCHAR key);// 处理共享车辆报修界面的字符输入
void SubmitSharedRepair(void);// 提交共享车辆报修信息

#endif
