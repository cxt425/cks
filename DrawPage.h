#ifndef DRAWPAGE_H
#define DRAWPAGE_H

#include "public.h"

// 页面绘制函数声明：负责各个界面的绘制
void DrawFirstPage(void);// 绘制首页
void DrawSharedSignoutPage(void);// 绘制共享车辆签退界面
void DrawPersonalManagementPage(void);// 绘制个人管理界面
void DrawPersonalRegistrationPage(void);// 绘制个人注册界面
void DrawPersonalInspectionPage(void);// 绘制个人年审管理界面
void DrawPersonalInformationPage(void);// 绘制个人信息变更界面
void DrawPersonalAccessPage1(void);// 绘制个人出入记录添加界面
void DrawPersonalAccessPage2(void);// 绘制个人出入记录查询界面
void DrawPersonalScrapPage(void);// 绘制个人报废车辆管理界面
void DrawSharedManagementPage(void);// 绘制共享车辆管理界面
void DrawSharedUseVehiclePage(void);// 绘制共享车辆使用界面
void DrawSharedSettlementPage(void);// 绘制共享车辆还车结算界面
void DrawSharedOrderPage(void);// 绘制共享车辆我的订单界面
void DrawSharedRepairPage(void);// 绘制共享车辆报修界面
#endif