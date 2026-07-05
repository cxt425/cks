#ifndef MOUSECTRL_H         // 如果未定义 MOUSECTRL_H，则开始包含保护
#define MOUSECTRL_H         // 定义 MOUSECTRL_H，防止头文件被重复包含
#include <graphics.h>      // 包含 EasyX 图形库头文件
#include "DrawPage.h"      // 包含自定义的 DrawPage.h 头文件，声明函数

typedef enum {       // 定义 MouseTarget 枚举类型，表示所有可点击控件
    MOUSE_NONE,       // 表示没有选中任何控件
    HOME_BTN_PERSON,   // 表示首页个人电动车管理系统按钮
    HOME_BTN_SHARE,    // 表示首页共享电动车登录按钮

    LOGIN_BACK,        // 表示登录页面返回按钮
    LOGIN_INPUT_USER,  // 表示登录页面用户名输入框
    LOGIN_INPUT_PHONE,  // 表示登录页面手机号输入框
    LOGIN_INPUT_CODE,    // 表示登录页面验证码输入框

    PERSON_BTN_BACK,     // 表示个人电动车管理系统页面返回按钮
    PERSON_BTN_REG,     // 表示个人电动车管理系统页面车辆注册/上牌按钮
    PERSON_BTN_YEAR,      // 表示个人电动车管理系统页面年审管理按钮
    PERSON_BTN_CHANGE,    // 表示个人电动车管理系统页面信息变更按钮
    PERSON_BTN_REPAIR,   // 表示个人电动车管理系统页面报废管理按钮
    PERSON_BTN_ENTRY_EXIT    // 表示个人电动车管理系统页面出入校园管理按钮

    
} MouseTarget;         // 定义 MouseTarget 枚举类型，表示所有可点击控件

extern MouseTarget currentMouseCtrl;     // 声明全局变量 currentMouseCtrl，表示当前选中的控件
extern PageType currentPage;            // 声明全局变量 currentPage，表示当前页面类型
void GlobalMouseCheck(MOUSEMSG m, PageType nowPage);     // 声明 GlobalMouseCheck 函数，用于统一处理鼠标事件，根据当前页面和鼠标消息判断用户操作
#endif