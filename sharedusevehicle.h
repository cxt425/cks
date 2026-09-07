#ifndef SHAREDUSEVEHICLE_H
#define SHAREDUSEVEHICLE_H

#include "sharedsignout.h"

void HandleSharedUseVehicleChar(TCHAR key);// 处理共享车辆使用界面的字符输入
void HandleSharedUseVehicleKey(char key);// 处理共享车辆使用界面的键盘输入
void QuerySharedVehicleInfo(void);// 查询共享车辆信息
void TryUnlockSharedVehicle(void);// 尝试开锁共享车辆
void HandleSharedSettlementChar(TCHAR key);// 处理共享车辆还车结算界面的字符输入
void HandleSharedSettlementKey(char key);// 处理共享车辆还车结算界面的键盘输入
void ConfirmSharedSettlementPayment(void);// 确认共享车辆还车结算支付
void UpdateSharedBicycleRecordInFile(const char* plate, const char* status, const char* battery);// 更新共享电动车信息到文件中
void ResetSharedOrderScroll(void);// 重置共享车辆我的订单界面滚动状态
void ScrollSharedOrder(int offset);// 滚动共享车辆我的订单界面
void BeginSharedOrderDrag(int y);// 开始拖动共享车辆我的订单界面滚动条
void UpdateSharedOrderDrag(int y);// 更新共享车辆我的订单界面拖动滚动条
void EndSharedOrderDrag(void);// 结束拖动共享车辆我的订单界面滚动条
void SetSharedOrderCount(int count);// 设置共享车辆我的订单界面订单数量

#endif
