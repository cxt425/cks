#ifndef DRAWPAGE_H    // 如果未定义 DRAWPAGE_H，则开始包含保护
#define DRAWPAGE_H    // 定义 DRAWPAGE_H，防止头文件被重复包含
#include <graphics.h>   // 包含 EasyX 图形库头文件

typedef enum {
    PAGE_HOME,
    PAGE_LOGIN,
    PAGE_PERSONAL_MANAGEMENT,
    PAGE_PERSONAL_REGISTRATION
} PageType;
extern PageType currentPage;   // 声明全局变量 currentPage，表示当前页面类型

void DrawFirstPage(void);    // 声明 DrawFirstPage 函数，用于绘制首页界面
void DrawSharedSignoutPage(void);  // 声明 DrawSharedSignoutPage 函数，用于绘制共享电动车登录首页
void DrawPersonalManagementPage(void);    // 声明 DrawPersonalManagementPage 函数，用于绘制个人电动车管理系统界面
void DrawPersonalRegistrationPage(void);    // 声明 DrawPersonalPage 函数，用于绘制个人电动车管理界面
#endif                       // 结束包含保护