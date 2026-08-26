#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>      // 包含标准输入输出头文件，用于 getchar()
#include <windows.h>    // 包含 Windows 键盘状态接口
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include "sharedsignout.h"
#include "personalregistration.h"

static void PollKeyboardInput(void)
{
    static unsigned char keyDown[256] = {0};
    static int initialized = 0;

    if (!initialized) {
        memset(keyDown, 0, sizeof(keyDown));
        initialized = 1;
    }

    for (int vk = 0; vk < 256; ++vk) {
        short state = GetAsyncKeyState(vk);
        int isDown = (state & 0x8000) != 0;

        if (isDown && !keyDown[vk]) {
            keyDown[vk] = 1;
            char ch = 0;

            if (vk >= '0' && vk <= '9') {
                ch = (char)vk;
            } else if (vk >= 'A' && vk <= 'Z') {
                ch = (char)(vk + 32);
            } else if (vk == VK_BACK) {
                ch = 8;
            } else if (vk == VK_RETURN) {
                ch = 13;
            } else if (vk == VK_TAB) {
                ch = 9;
            } else if (vk == VK_SPACE) {
                ch = ' ';
            }

            if (ch != 0) {
                if (currentPage == PAGE_LOGIN) HandleSharedSignoutKey(ch);
                else if (currentPage == PAGE_PERSONAL_REGISTRATION) HandlePersonalRegistrationKey(ch);
            }
        } else if (!isDown) {
            keyDown[vk] = 0;
        }
    }
}

int main() {
    initgraph(480, 640);        // 初始化图形窗口，宽 480 像素，高 640 像素, 显示控制台窗口
    setbkcolor(WHITE);          // 设置背景颜色为白色
    BeginBatchDraw();           // 开始批量绘制，防止闪烁
    InitSharedSignoutState();
    DWORD loginSuccessTime = 0;
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
            case PAGE_PERSONAL_ACCESSPAGE2: DrawPersonalAccessPage2(); break;
            case PAGE_PERSONAL_SCRAP: DrawPersonalScrapPage(); break;
            case PAGE_SHARED_MANAGEMENT: DrawSharedManagementPage(); break;
        }

        EndBatchDraw();             // 提交一帧绘制内容
        PollKeyboardInput();        // 检测当前按键并转发给登录页面
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