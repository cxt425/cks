#ifndef MOUSECTRL_H        // 声明 MouseCtrl.h 头文件，防止重复包含
#define MOUSECTRL_H
#include "public.h"
#include "personalaccess.h"

// 统一处理鼠标点击事件的函数声明
void GlobalMouseCheck(MOUSEMSG m, PageType nowPage);

#endif
