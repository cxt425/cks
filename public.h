#ifndef PUBLIC_H
#define PUBLIC_H

#include <graphics.h>

// 页面类型枚举：定义整个程序中有哪些页面
typedef enum {
    PAGE_HOME,
    PAGE_LOGIN,
    PAGE_PERSONAL_MANAGEMENT,
    PAGE_PERSONAL_REGISTRATION,
    PAGE_PERSONAL_INSPECTION,
    PAGE_PERSONAL_INFORMATION,
    PAGE_PERSONAL_ACCESSPAGE1
} PageType;

// 鼠标目标枚举：定义界面中可点击的控件
typedef enum {
    MOUSE_NONE,
    HOME_BTN_PERSON,
    HOME_BTN_SHARE,
    LOGIN_BACK,
    LOGIN_INPUT_USER,
    LOGIN_INPUT_PHONE,
    LOGIN_INPUT_CODE,
    LOGIN_SUBMIT,
    PERSON_BTN_BACK,
    PERSON_BTN_REG,
    PERSON_BTN_YEAR,
    PERSON_BTN_CHANGE,
    PERSON_BTN_REPAIR,
    PERSON_BTN_ENTRY_EXIT
} MouseTarget;

// 全局变量声明：由 MouseCtrl.cpp 中定义
extern PageType currentPage;
extern MouseTarget currentMouseCtrl;

#endif
