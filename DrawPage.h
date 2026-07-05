#ifndef DRAWPAGE_H    // 如果未定义 DRAWPAGE_H，则开始包含保护
#define DRAWPAGE_H    // 定义 DRAWPAGE_H，防止头文件被重复包含
void DrawFirstPage(void);    // 声明 DrawFirstPage 函数，用于绘制首页界面
void DrawSharedSignoutPage(void);  // 声明 DrawSharedSignoutPage 函数，用于绘制共享电动车登录首页
void DrawPersonalManagementPage(void);    // 声明 DrawPersonalManagementPage 函数，用于绘制个人电动车管理系统界面
void DrawPersonalRegistrationPage(void);    // 声明 DrawPersonalPage 函数，用于绘制个人电动车管理界面
void DrawPersonalInspectionPage(void);    // 声明 DrawPersonalInspectionPage 函数，用于绘制个人电动车年审管理系统界面
#endif                       // 结束包含保护