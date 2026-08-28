#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>      // 包含标准输入输出头文件，用于 getchar()
#include <windows.h>    // 包含 Windows 键盘状态接口
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include "sharedsignout.h"
#include "personalregistration.h"

int main() {
    initgraph(480, 640);        // 初始化图形窗口，宽 480 像素，高 640 像素, 显示控制台窗口
    setbkcolor(WHITE);          // 设置背景颜色为白色
    BeginBatchDraw();           // 开始批量绘制，防止闪烁
    InitSharedSignoutState();
    DWORD loginSuccessTime = 0;
    MOUSEMSG m;
    ExMessage msg;
    while(1)
    {
        while (peekmessage(&msg))
    {
        // 接收输入法输入（中文、英文、数字、退格）
        if (msg.message == WM_CHAR)
        {
            TCHAR ch = msg.ch;
            if (currentPage == PAGE_PERSONAL_REGISTRATION)
            {
                 if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                    HandlePersonalRegistrationChar(ch);
            }
            else if (currentPage == PAGE_LOGIN)
            {
                 if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                    HandleSharedSignoutChar(ch);
            }
        }
        else if (msg.message == WM_KEYDOWN && currentPage == PAGE_PERSONAL_REGISTRATION)
        {
            if (msg.vkcode == VK_BACK) HandlePersonalRegistrationKey(8);
            else if (msg.vkcode == VK_RETURN) HandlePersonalRegistrationKey(13);
            else if (msg.vkcode == VK_TAB) HandlePersonalRegistrationKey(9);
        }
        else if (msg.message == WM_KEYDOWN && currentPage == PAGE_LOGIN)
        {
            if (msg.vkcode == VK_BACK) HandleSharedSignoutKey(8);
            else if (msg.vkcode == VK_RETURN) HandleSharedSignoutKey(13);
            else if (msg.vkcode == VK_TAB) HandleSharedSignoutKey(9);
        }
        // 鼠标左键，保留你原有逻辑
        else if (msg.message == WM_LBUTTONDOWN)
        {
            MOUSEMSG tempMouse;
            tempMouse.x = msg.x;
            tempMouse.y = msg.y;
            tempMouse.uMsg = WM_LBUTTONDOWN;
            GlobalMouseCheck(tempMouse, currentPage);
        }
    }

    cleardevice();
    BeginBatchDraw();
        switch(currentPage)               // 根据当前页面类型调用相应的绘制函数
        {
            case PAGE_HOME: DrawFirstPage(); break;
            case PAGE_LOGIN: DrawSharedSignoutPage(); break;
            case PAGE_PERSONAL_MANAGEMENT: DrawPersonalManagementPage(); break;
            case PAGE_PERSONAL_REGISTRATION: DrawPersonalRegistrationPage(); break;
            case PAGE_PERSONAL_INSPECTION: DrawPersonalInspectionPage(); break;
            case PAGE_PERSONAL_INFORMATION: DrawPersonalInformationPage(); break;
            case PAGE_PERSONAL_ACCESSPAGE1: DrawPersonalAccessPage1(); break;
            case PAGE_PERSONAL_ACCESSPAGE2: DrawPersonalAccessPage2(); break;
            case PAGE_PERSONAL_SCRAP: DrawPersonalScrapPage(); break;
            case PAGE_SHARED_MANAGEMENT: DrawSharedManagementPage(); break;
        }

        EndBatchDraw();             // 提交一帧绘制内容
// 检查共享登录状态，如果登录成功且当前页面是登录页面，则在 3 秒后自动切换到个人管理页面
        SharedUserInfo* sharedState = GetSharedSignoutState();
        if (currentPage == PAGE_LOGIN && sharedState->loginSuccess) {
            if (loginSuccessTime == 0)
                loginSuccessTime = GetTickCount();
            else if (GetTickCount() - loginSuccessTime >= 3000) {
                currentPage = PAGE_SHARED_MANAGEMENT;
                loginSuccessTime = 0;
            }
        } else {
            loginSuccessTime = 0;
        }

        Sleep(10);                  // 暂停一小段时间，降低 CPU 占用率
        BeginBatchDraw();           // 开始下一帧批量绘制
    }
    closegraph();                 // 关闭图形窗口并释放资源
    return 0;                     // 返回 0 表示程序正常结束
}