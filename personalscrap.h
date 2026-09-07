#ifndef _PERSONALSCRAP_H_
#define _PERSONALSCRAP_H_

#include "personalregistration.h"

void QueryPersonalScrapStatus(void);// 查询个人报废车辆状态
void HandlePersonalScrapKey(char key);// 处理个人报废车辆管理界面的键盘输入
void HandlePersonalScrapChar(TCHAR key);// 处理个人报废车辆管理界面的字符输入
void SavePersonalScrapUpdate(void);// 保存个人报废车辆状态更新

#endif
