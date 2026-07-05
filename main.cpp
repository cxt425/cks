#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>      // 包含标准输入输出头文件，用于 getchar()
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include <conio.h>      // 包含控制台输入输出头文件，用于 _kbhit() 和 _getch() 函数

extern IMAGE img; // 声明全局变量 img，用于存储图片对象

int main() {
    initgraph(480, 640);        // 初始化图形窗口，宽 480 像素，高 640 像素, 显示控制台窗口
    setbkcolor(WHITE);          // 设置背景颜色为白色
    BeginBatchDraw();           // 开始批量绘制，防止闪烁
    loadimage(&img, _T("xiaohui.png"), 120, 110);  // 加载图片文件 "xiaohui.png"，并将其缩放为 120x110
    MOUSEMSG m;
    while(1)
    {
        while(MouseHit())          // 检测是否有鼠标消息，如果有则进入循环
        {
            m = GetMouseMsg();
            GlobalMouseCheck(m, currentPage);
        }

        switch(currentPage)               // 根据当前页面类型调用相应的绘制函数
        {
            case PAGE_HOME: DrawFirstPage(); break;
            case PAGE_LOGIN: DrawSharedSignoutPage(); break;
            case PAGE_PERSONAL_MANAGEMENT: DrawPersonalManagementPage(); break;
        }

        EndBatchDraw();             // 提交一帧绘制内容
        Sleep(10);                  // 暂停一小段时间，降低 CPU 占用率
        BeginBatchDraw();           // 开始下一帧批量绘制

        if(_kbhit() && _getch()==27) break;      // 如果检测到键盘按下事件且按下的键是 ESC 键，则跳出循环，结束程序
    }
    EndBatchDraw();               // 结束批量绘制
    closegraph();                 // 关闭图形窗口并释放资源
    return 0;                     // 返回 0 表示程序正常结束
}