#include "MouseCtrl.h"    // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include "sharedsignout.h"
#include "personalregistration.h"
#include <graphics.h>      // 包含 EasyX 图形库头文件

MouseTarget currentMouseCtrl = MOUSE_NONE;      // 声明全局变量 currentMouseCtrl，表示当前选中的控件
PageType currentPage = PAGE_HOME;            // 声明全局变量 currentPage，表示当前页面类型

void GlobalMouseCheck(MOUSEMSG m, PageType nowPage)     // 声明 GlobalMouseCheck 函数，用于统一处理鼠标事件，根据当前页面和鼠标消息判断用户操作
{
    if(m.uMsg != WM_LBUTTONDOWN) return;           // 如果鼠标消息不是左键按下事件，则直接返回，不进行处理
    currentMouseCtrl = MOUSE_NONE;                 // 将当前选中的控件设置为 MOUSE_NONE，表示没有选中任何控件

    SharedUserInfo* state = GetSharedSignoutState();    // 获取共享登录状态信息的指针，用于在登录页面处理输入框和按钮的操作
    PersonalUserInfo* regState = GetPersonalRegistrationState(); // 获取个人注册状态信息的指针，用于在注册页面处理输入框和按钮的操作
    switch(nowPage)                                 // 根据当前页面类型进行不同的鼠标事件处理
    {
        case PAGE_HOME:                             // 如果当前页面是首页，则判断鼠标点击位置是否在个人电动车管理系统按钮或共享电动车登录按钮的范围内
            if(m.x >=40 && m.x <=200 && m.y >=400 && m.y <=550)             // 如果鼠标点击位置在个人电动车管理系统按钮的范围内，则将当前选中的控件设置为 HOME_BTN_PERSON
                {
                    currentMouseCtrl = HOME_BTN_PERSON;
                    currentPage = PAGE_PERSONAL_MANAGEMENT; // 切换到个人电动车管理系统页面
                }
            else if(m.x >=280 && m.x <=440 && m.y >=400 && m.y <=550)        // 如果鼠标点击位置在共享电动车登录按钮的范围内，则将当前选中的控件设置为 HOME_BTN_SHARE
                {
                    currentMouseCtrl = HOME_BTN_SHARE;
                    currentPage = PAGE_LOGIN; // 切换到共享电动车登录页面
                }
            break;
        case PAGE_LOGIN:                      // 如果当前页面是登录页面，则判断鼠标点击位置是否在返回按钮、用户名输入框、手机号输入框、验证码输入框或登录按钮的范围内
            if(m.x >=0 && m.x <=50 && m.y >=0 && m.y <=40)          // 如果鼠标点击位置在返回按钮的范围内，则将当前选中的控件设置为 LOGIN_BACK
                {
                    currentMouseCtrl = LOGIN_BACK;
                    currentPage = PAGE_HOME; // 切换回首页
                }
            else if(m.x >=140 && m.x <=440 && m.y >=360 && m.y <=410)        // 如果鼠标点击位置在用户名输入框的范围内，则将当前选中的控件设置为 LOGIN_INPUT_USER
                {
                    currentMouseCtrl = LOGIN_INPUT_USER;
                    state->focus = 0;
                    currentPage = PAGE_LOGIN; // 保持在登录页面
                }
            else if(m.x >=140 && m.x <=440 && m.y >=410 && m.y <=460)        // 如果鼠标点击位置在手机号输入框的范围内，则将当前选中的控件设置为 LOGIN_INPUT_PHONE
                {
                    currentMouseCtrl = LOGIN_INPUT_PHONE;
                    state->focus = 1;
                    currentPage = PAGE_LOGIN; // 保持在登录页面
                }
            else if(m.x >=140 && m.x <=440 && m.y >=460 && m.y <=510)        // 如果鼠标点击位置在验证码输入框的范围内，则将当前选中的控件设置为 LOGIN_INPUT_CODE
                {
                    currentMouseCtrl = LOGIN_INPUT_CODE;
                    state->focus = 2;
                    currentPage = PAGE_LOGIN; // 保持在登录页面
                }
            else if(m.x >=70 && m.x <=190 && m.y >=550 && m.y <=600)        // 如果鼠标点击位置在登录按钮的范围内，则执行登录按钮操作
                {
                    currentMouseCtrl = LOGIN_SUBMIT;
                    TrySharedLogin();
                    currentPage = PAGE_LOGIN; // 保持在登录页面
                }
            else if(m.x >=290 && m.x <=410 && m.y >=550 && m.y <=600)        // 如果鼠标点击位置在获取验证码按钮的范围内，则执行获取验证码操作
                {
                    currentMouseCtrl = LOGIN_GET_CODE;
                    GenerateSharedVerificationCode();
                    currentPage = PAGE_LOGIN; // 保持在登录页面
                }
            break;
        case PAGE_PERSONAL_MANAGEMENT:
            if(m.x >=30 && m.x <=210 && m.y >=335 && m.y <=420)
               {
                  currentMouseCtrl = PERSON_BTN_REG;
                  currentPage = PAGE_PERSONAL_REGISTRATION; // 切换到个人电动车注册页面
               }
            else if(m.x >=270 && m.x <=450 && m.y >=335 && m.y <=420)
               {
                  currentMouseCtrl = PERSON_BTN_YEAR;
                  currentPage = PAGE_PERSONAL_INSPECTION; // 保持在个人电动车年审管理系统页面
               }
            else if(m.x >=30 && m.x <=210 && m.y >=440 && m.y <=530)
               {
                  currentMouseCtrl = PERSON_BTN_CHANGE;
                  currentPage = PAGE_PERSONAL_INFORMATION; // 保持在个人电动车信息管理系统页面
               }
            else if(m.x >=270 && m.x <=450 && m.y >=440 && m.y <=530)
               {
                  currentMouseCtrl = PERSON_BTN_REPAIR; 
                  currentPage = PAGE_PERSONAL_SCRAP; // 保持在个人电动车报废管理系统页面
               }
            else if(m.x >=0 && m.x <=50 && m.y >=0 && m.y <=40)
               {
                  currentMouseCtrl = PERSON_BTN_BACK;
                  currentPage = PAGE_HOME; // 切换回首页
                }
            else if(m.x >=30 && m.x <=450 && m.y >=545 && m.y <=605)
               {
                  currentMouseCtrl = PERSON_BTN_ENTRY_EXIT;
                  currentPage = PAGE_PERSONAL_ACCESSPAGE1; // 切换到个人电动车出入校园管理1页面
             }
            break;
        case PAGE_PERSONAL_REGISTRATION:     // 如果当前页面是个人电动车注册页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
             if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮（坐标和你DrawPersonalRegistrationPage里返回框一致）
                {
                  currentMouseCtrl = PERSON_BTN_BACK;// 点击返回，切回个人管理页面
                  currentPage = PAGE_PERSONAL_MANAGEMENT;
                }
            else if (m.x >=240 && m.x <=420 && m.y >=170 && m.y <=200) {
                currentMouseCtrl = PERSON_REG_INPUT_LICENSE;
                regState->focus = 0;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=240 && m.x <=420 && m.y >=210 && m.y <=240) {
                currentMouseCtrl = PERSON_REG_INPUT_OWNER;
                regState->focus = 1;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=240 && m.x <=420 && m.y >=250 && m.y <=280) {
                currentMouseCtrl = PERSON_REG_INPUT_COLLEGE;
                regState->focus = 2;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=240 && m.x <=420 && m.y >=290 && m.y <=320) {
                currentMouseCtrl = PERSON_REG_INPUT_ID;
                regState->focus = 3;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=240 && m.x <=420 && m.y >=330 && m.y <=360) {
                currentMouseCtrl = PERSON_REG_INPUT_PHONE;
                regState->focus = 4;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=240 && m.x <=420 && m.y >=370 && m.y <=400) {
                currentMouseCtrl = PERSON_REG_INPUT_TYPE;
                regState->focus = 5;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=240 && m.x <=420 && m.y >=410 && m.y <=440) {
                currentMouseCtrl = PERSON_REG_INPUT_DATE;
                regState->focus = 6;
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            else if (m.x >=40 && m.x <=440 && m.y >=560 && m.y <=610) {
                currentMouseCtrl = PERSON_REG_SUBMIT;
                TryPersonalRegistration();
                currentPage = PAGE_PERSONAL_REGISTRATION;
            }
            break;
        case PAGE_PERSONAL_INSPECTION:        // 如果当前页面是个人电动车年审管理页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
             if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮（坐标和你DrawPersonalInspectionPage里返回框一致）
               {
                  currentMouseCtrl = PERSON_BTN_BACK;// 点击返回，切回个人管理页面
                  currentPage = PAGE_PERSONAL_MANAGEMENT;
               }
            break;
        case PAGE_PERSONAL_INFORMATION:           // 如果当前页面是个人电动车信息变更页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
             if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮（坐标和你DrawPersonalInformationPage里返回框一致）
               {
                  currentMouseCtrl = PERSON_BTN_BACK;// 点击返回，切回个人管理页面
                  currentPage = PAGE_PERSONAL_MANAGEMENT;
                }
            break;
        case PAGE_PERSONAL_ACCESSPAGE1:         // 如果当前页面是个人电动车出入校园管理1页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
             if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮（坐标和你DrawPersonalAccessPage1里返回框一致）
               {
                  currentMouseCtrl = PERSON_BTN_BACK;// 点击返回，切回个人管理页面
                  currentPage = PAGE_PERSONAL_MANAGEMENT;
                } 
            else if(m.x >=270 && m.x <=380 && m.y >=85 && m.y <=125)
                {
                  currentMouseCtrl = PERSON_BTN_ENTRY_EXIT2;
                  currentPage = PAGE_PERSONAL_ACCESSPAGE2; // 保持在个人电动车出入校园管理2页面
                }
             break;
        case PAGE_PERSONAL_ACCESSPAGE2:         // 如果当前页面是个人电动车出入校园管理2页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
                if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮（坐标和你DrawPersonalAccessPage2里返回框一致）
                {
                    currentMouseCtrl = PERSON_BTN_BACK;// 点击返回，切回个人管理页面
                    currentPage = PAGE_PERSONAL_MANAGEMENT;
                    } 
                else if(m.x >=55 && m.x <=165 && m.y >=85 && m.y <=125)
                {
                    currentMouseCtrl = PERSON_BTN_ENTRY_EXIT1;
                    currentPage = PAGE_PERSONAL_ACCESSPAGE1; // 保持在个人电动车出入校园管理1页面
                }
                break;
        case PAGE_PERSONAL_SCRAP:         // 如果当前页面是个人电动车报废管理页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
             if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮（坐标和你DrawPersonalScrapPage里返回框一致）
               {
                  currentMouseCtrl = PERSON_BTN_BACK;// 点击返回，切回个人管理页面
                  currentPage = PAGE_PERSONAL_MANAGEMENT;
                } 
                break;
    }
}
