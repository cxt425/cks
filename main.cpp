#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>      // 包含标准输入输出头文件，用于 getchar()
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
int main() {
    initgraph(480, 640);        // 初始化图形窗口，宽 480 像素，高 640 像素
    setbkcolor(WHITE);          // 设置背景颜色为白色
    cleardevice();              // 清空窗口并用背景颜色填充
    //DrawFirstPage();            // 调用 DrawFirstPage 函数绘制首页界面
    DrawSharedSignoutPage();    // 调用 DrawSharedSignoutPage 函数绘制共享电动车登录首页
    return 0;                   // 返回 0 表示程序正常结束
}