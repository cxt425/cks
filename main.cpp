#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>      // 包含标准输入输出头文件，用于 getchar()
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include <conio.h>      // 包含控制台输入输出头文件，用于 _kbhit() 和 _getch() 函数


int main() {
    initgraph(480, 640);        // 初始化图形窗口，宽 480 像素，高 640 像素, 显示控制台窗口
    setbkcolor(WHITE);          // 设置背景颜色为白色
    BeginBatchDraw();           // 开始批量绘制，防止闪烁
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
            case PAGE_PERSONAL_REGISTRATION: DrawPersonalRegistrationPage(); break;
            case PAGE_PERSONAL_INSPECTION: DrawPersonalInspectionPage(); break;
            case PAGE_PERSONAL_INFORMATION: DrawPersonalInformationPage(); break;
            case PAGE_PERSONAL_ACCESSPAGE1: DrawPersonalAccessPage1(); break;
        }

        EndBatchDraw();             // 提交一帧绘制内容
        Sleep(10);                  // 暂停一小段时间，降低 CPU 占用率
        BeginBatchDraw();           // 开始下一帧批量绘制
        if(_kbhit())
        {
        if(_getch() == 27)
            break;
        }
    }
    closegraph();                 // 关闭图形窗口并释放资源
    return 0;                     // 返回 0 表示程序正常结束
}