#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>      // 包含标准输入输出头文件，用于 getchar()
#include <windows.h>    // 包含 Windows 键盘状态接口
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include "sharedsignout.h"
#include "sharedusevehicle.h"
#include "sharedrepair.h"
#include "personalregistration.h"
#include "personalinspection.h"
#include "personalscrap.h"
#include "personalaccess.h"

int main() {
    initgraph(480, 640);        // 初始化图形窗口，宽 480 像素，高 640 像素, 显示控制台窗口
    setbkcolor(WHITE);          // 设置背景颜色为白色
    BeginBatchDraw();           // 开始批量绘制，防止闪烁
    InitSharedSignoutState();
    InitPersonalInspectionState();
    DWORD loginSuccessTime = 0;
    MOUSEMSG m;
    ExMessage msg;
    while(1)
    {
        while (peekmessage(&msg))
        {
            if (msg.message == WM_CHAR)
            {
                TCHAR ch = msg.ch;
                if (currentPage == PAGE_PERSONAL_REGISTRATION)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandlePersonalRegistrationChar(ch);
                }
                else if (currentPage == PAGE_PERSONAL_INFORMATION)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandlePersonalInformationChar(ch);
                }
                else if (currentPage == PAGE_LOGIN)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandleSharedSignoutChar(ch);
                }
                else if (currentPage == PAGE_SHARED_USE_VEHICLE)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandleSharedUseVehicleChar(ch);
                }
                else if (currentPage == PAGE_SHARED_SETTLEMENT)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandleSharedSettlementChar(ch);
                }
                else if (currentPage == PAGE_SHARED_REPAIR)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandleSharedRepairChar(ch);
                }
                else if (currentPage == PAGE_PERSONAL_INSPECTION)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandlePersonalInspectionChar(ch);
                }
                else if (currentPage == PAGE_PERSONAL_SCRAP)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandlePersonalScrapChar(ch);
                }
                else if (currentPage == PAGE_PERSONAL_ACCESSPAGE1)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandlePersonalAccessChar(ch);
                }
                else if (currentPage == PAGE_PERSONAL_ACCESSPAGE2)
                {
                    if (ch != 8 && ch != 9 && ch != 10 && ch != 13 && ch != 127)
                        HandlePersonalAccessQueryChar(ch);
                }
            }
            else if (msg.message == WM_KEYDOWN)
            {
                if (currentPage == PAGE_PERSONAL_REGISTRATION)
                {
                    if (msg.vkcode == VK_BACK) HandlePersonalRegistrationKey(8);
                    else if (msg.vkcode == VK_RETURN) HandlePersonalRegistrationKey(13);
                    else if (msg.vkcode == VK_TAB) HandlePersonalRegistrationKey(9);
                }
                else if (currentPage == PAGE_PERSONAL_INFORMATION)
                {
                    if (msg.vkcode == VK_BACK) HandlePersonalInformationKey(8);
                    else if (msg.vkcode == VK_RETURN) HandlePersonalInformationKey(13);
                }
                else if (currentPage == PAGE_PERSONAL_INSPECTION)
                {
                    if (msg.vkcode == VK_BACK) HandlePersonalInspectionKey(8);
                    else if (msg.vkcode == VK_RETURN) HandlePersonalInspectionKey(13);
                    else if (msg.vkcode == VK_TAB) HandlePersonalInspectionKey(9);
                }
                else if (currentPage == PAGE_PERSONAL_SCRAP)
                {
                    if (msg.vkcode == VK_BACK) HandlePersonalScrapKey(8);
                    else if (msg.vkcode == VK_RETURN) HandlePersonalScrapKey(13);
                    else if (msg.vkcode == VK_TAB) HandlePersonalScrapKey(9);
                }
                else if (currentPage == PAGE_PERSONAL_ACCESSPAGE1)
                {
                    if (msg.vkcode == VK_BACK) HandlePersonalAccessKey(8);
                    else if (msg.vkcode == VK_RETURN) HandlePersonalAccessKey(13);
                    else if (msg.vkcode == VK_TAB) HandlePersonalAccessKey(9);
                }
                else if (currentPage == PAGE_PERSONAL_ACCESSPAGE2)
                {
                    if (msg.vkcode == VK_BACK) HandlePersonalAccessQueryKey(8);
                    else if (msg.vkcode == VK_RETURN) HandlePersonalAccessQueryKey(13);
                    else if (msg.vkcode == VK_TAB) HandlePersonalAccessQueryKey(9);
                }
                else if (currentPage == PAGE_LOGIN)
                {
                    if (msg.vkcode == VK_BACK) HandleSharedSignoutKey(8);
                    else if (msg.vkcode == VK_RETURN) HandleSharedSignoutKey(13);
                    else if (msg.vkcode == VK_TAB) HandleSharedSignoutKey(9);
                }
                else if (currentPage == PAGE_SHARED_USE_VEHICLE)
                {
                    if (msg.vkcode == VK_BACK) HandleSharedUseVehicleKey(8);
                    else if (msg.vkcode == VK_RETURN) HandleSharedUseVehicleKey(13);
                    else if (msg.vkcode == VK_TAB) HandleSharedUseVehicleKey(9);
                }
                else if (currentPage == PAGE_SHARED_SETTLEMENT)
                {
                    if (msg.vkcode == VK_BACK) HandleSharedSettlementKey(8);
                    else if (msg.vkcode == VK_RETURN) HandleSharedSettlementKey(13);
                    else if (msg.vkcode == VK_TAB) HandleSharedSettlementKey(9);
                }
                else if (currentPage == PAGE_SHARED_REPAIR)
                {
                    if (msg.vkcode == VK_BACK) HandleSharedRepairKey(8);
                    else if (msg.vkcode == VK_RETURN) HandleSharedRepairKey(13);
                    else if (msg.vkcode == VK_TAB) HandleSharedRepairKey(9);
                }
            }
            else if (msg.message == WM_MOUSEWHEEL && currentPage == PAGE_PERSONAL_ACCESSPAGE2)
            {
                short delta = (short)HIWORD(msg.wParam);
                ScrollPersonalAccessQuery(delta > 0 ? -1 : 1);
            }
            else if (msg.message == WM_MOUSEWHEEL && currentPage == PAGE_SHARED_ORDER)
            {
                short delta = (short)HIWORD(msg.wParam);
                ScrollSharedOrder(delta > 0 ? -1 : 1);
            }
            else if (msg.message == WM_MOUSEMOVE && currentPage == PAGE_SHARED_ORDER)
            {
                UpdateSharedOrderDrag(msg.y);
            }
            else if (msg.message == WM_MOUSEMOVE && currentPage == PAGE_PERSONAL_ACCESSPAGE2)
            {
                PersonalUserInfo* st = GetPersonalRegistrationState();
                if (st->accessQueryDragging && st->accessQueryCount > 4)
                {
                    int barTop = 340;
                    int barBottom = 505;
                    int thumbHeight = (barBottom - barTop) * 4 / (st->accessQueryCount > 4 ? st->accessQueryCount : 4);
                    if (thumbHeight < 18) thumbHeight = 18;
                    int maxScroll = st->accessQueryCount - 4;
                    int effective = barBottom - barTop - thumbHeight;
                    if (effective <= 0) effective = 1;
                    int target = msg.y - thumbHeight / 2;
                    if (target < barTop) target = barTop;
                    if (target > barBottom - thumbHeight) target = barBottom - thumbHeight;
                    st->accessQueryScroll = (int)((target - barTop) * maxScroll / effective);
                    if (st->accessQueryScroll < 0) st->accessQueryScroll = 0;
                    if (st->accessQueryScroll > maxScroll) st->accessQueryScroll = maxScroll;
                }
            }
            else if (msg.message == WM_LBUTTONDOWN)
            {
                if (currentPage == PAGE_SHARED_ORDER && msg.x >= 410 && msg.x <= 440 && msg.y >= 145 && msg.y <= 500)
                {
                    BeginSharedOrderDrag(msg.y);
                }
                else if (currentPage == PAGE_PERSONAL_ACCESSPAGE2)
                {
                    PersonalUserInfo* st = GetPersonalRegistrationState();
                    if (st->accessQueryCount > 4 && msg.x >= 448 && msg.x <= 456 && msg.y >= 340 && msg.y <= 505)
                    {
                        st->accessQueryDragging = 1;
                        int barTop = 340;
                        int barBottom = 505;
                        int thumbHeight = (barBottom - barTop) * 4 / (st->accessQueryCount > 4 ? st->accessQueryCount : 4);
                        if (thumbHeight < 18) thumbHeight = 18;
                        int maxScroll = st->accessQueryCount - 4;
                        int effective = barBottom - barTop - thumbHeight;
                        if (effective <= 0) effective = 1;
                        int target = msg.y - thumbHeight / 2;
                        if (target < barTop) target = barTop;
                        if (target > barBottom - thumbHeight) target = barBottom - thumbHeight;
                        st->accessQueryScroll = (int)((target - barTop) * maxScroll / effective);
                        if (st->accessQueryScroll < 0) st->accessQueryScroll = 0;
                        if (st->accessQueryScroll > maxScroll) st->accessQueryScroll = maxScroll;
                    }
                    else
                    {
                        MOUSEMSG tempMouse;
                        tempMouse.x = msg.x;
                        tempMouse.y = msg.y;
                        tempMouse.uMsg = WM_LBUTTONDOWN;
                        GlobalMouseCheck(tempMouse, currentPage);
                    }
                }
                else
                {
                    MOUSEMSG tempMouse;
                    tempMouse.x = msg.x;
                    tempMouse.y = msg.y;
                    tempMouse.uMsg = WM_LBUTTONDOWN;
                    GlobalMouseCheck(tempMouse, currentPage);
                }
            }
            else if (msg.message == WM_LBUTTONUP && currentPage == PAGE_SHARED_ORDER)
            {
                EndSharedOrderDrag();
            }
            else if (msg.message == WM_LBUTTONUP && currentPage == PAGE_PERSONAL_ACCESSPAGE2)
            {
                GetPersonalRegistrationState()->accessQueryDragging = 0;
            }
        }

        if (currentPage == PAGE_PERSONAL_REGISTRATION &&
            GetPersonalRegistrationState()->registered) {
            currentPage = PAGE_PERSONAL_MANAGEMENT;
        }

        cleardevice();
        BeginBatchDraw();
        switch(currentPage)
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
            case PAGE_SHARED_USE_VEHICLE: DrawSharedUseVehiclePage(); break;
            case PAGE_SHARED_SETTLEMENT: DrawSharedSettlementPage(); break;
            case PAGE_SHARED_ORDER: DrawSharedOrderPage(); break;
            case PAGE_SHARED_REPAIR: DrawSharedRepairPage(); break;
        }

        EndBatchDraw();
        SharedUserInfo* sharedState = GetSharedSignoutState();
        if (currentPage == PAGE_LOGIN && sharedState->loginSuccess)
        {
            currentPage = PAGE_SHARED_MANAGEMENT;
            sharedState->loginSuccess = 0;
        }

        Sleep(10);
        BeginBatchDraw();
    }
    closegraph();
    return 0;
}