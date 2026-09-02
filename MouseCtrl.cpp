#include "MouseCtrl.h"    // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include "sharedsignout.h"
#include "sharedusevehicle.h"
#include "sharedrepair.h"
#include "personalregistration.h"
#include "personalinspection.h"
#include "personalscrap.h"
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
                 else if (m.x >= 160 && m.x <= 430 && m.y >= 100 && m.y <= 140)
                    {
                        currentMouseCtrl = PERSON_BTN_YEAR;
                        GetPersonalInspectionState()->focus = 0;
                    }
                 else if (m.x >= 160 && m.x <= 430 && m.y >= 145 && m.y <= 185)
                    {
                        currentMouseCtrl = PERSON_BTN_YEAR;
                        GetPersonalInspectionState()->focus = 1;
                    }
                 else if (m.x >= 30 && m.x <= 450 && m.y >= 500 && m.y <= 535)
                    {
                        currentMouseCtrl = PERSON_BTN_YEAR;
                        QueryPersonalInspection();
                    }
                 else if (m.x >= 30 && m.x <= 450 && m.y >= 245 && m.y <= 315)
                    {
                        currentMouseCtrl = PERSON_BTN_YEAR;
                        RestorePersonalInspectionState();
                    }
                 else if (m.x >= 30 && m.x <= 450 && m.y >= 330 && m.y <= 400)
                    {
                        currentMouseCtrl = PERSON_BTN_YEAR;
                        UpdatePersonalInspectionStatus();
                    }
                 else if (m.x >= 140 && m.x <= 340 && m.y >= 550 && m.y <= 600)
                    {
                        currentMouseCtrl = PERSON_BTN_YEAR;
                        SavePersonalInspectionUpdate();
                    }
            break;
        case PAGE_PERSONAL_INFORMATION:           // 如果当前页面是个人电动车信息变更页面，则判断鼠标点击位置是否在左上角返回按钮的范围内
               if(m.x >=35 && m.x <=320 && m.y >=145 && m.y <=185) {
                   regState->focus = 7;
               }
               else if(m.x >=350 && m.x <=430 && m.y >=145 && m.y <=185) {
                   QueryPersonalVehicleInfo();
               }
                    else if (m.x >=280 && m.x <=440 && m.y >=225 && m.y <=260) regState->focus = 8;
                    else if (m.x >=280 && m.x <=440 && m.y >=270 && m.y <=305) regState->focus = 9;
                    else if (m.x >=280 && m.x <=440 && m.y >=315 && m.y <=350) regState->focus = 10;
                    else if (m.x >=280 && m.x <=440 && m.y >=360 && m.y <=395) regState->focus = 11;
                    else if (m.x >=280 && m.x <=440 && m.y >=405 && m.y <=440) regState->focus = 12;
                    else if (m.x >=30 && m.x <=450 && m.y >=560 && m.y <=610) UpdatePersonalVehicleInfo();
               else if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)// 左上角返回按钮
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
            else if (m.x >= 160 && m.x <= 440 && m.y >= 182 && m.y <= 228) {
                  regState->accessFocus = 0;
            }
            else if (m.x >= 160 && m.x <= 440 && m.y >= 408 && m.y <= 454) {
                  regState->accessFocus = 1;
            }
            else if (m.x >= 160 && m.x <= 280 && m.y >= 300 && m.y <= 350) {
                  regState->accessTypeSelected = 0;
                  strcpy(regState->accessType, "入校");
            }
            else if (m.x >= 300 && m.x <= 420 && m.y >= 300 && m.y <= 350) {
                  regState->accessTypeSelected = 1;
                  strcpy(regState->accessType, "出校");
            }
            else if (m.x >= 20 && m.x <= 460 && m.y >= 520 && m.y <= 590) {
                  SubmitPersonalAccessRecord();
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
                else if (m.x >= 30 && m.x <= 320 && m.y >= 175 && m.y <= 245) {
                    regState->accessQueryFocus = 0;
                    currentPage = PAGE_PERSONAL_ACCESSPAGE2;
                }
                else if (m.x >= 345 && m.x <= 430 && m.y >= 175 && m.y <= 245) {
                    QueryPersonalAccessRecords();
                    currentPage = PAGE_PERSONAL_ACCESSPAGE2;
                }
                break;
        case PAGE_PERSONAL_SCRAP:
             if(m.x >=0 && m.x <=80 && m.y >=0 && m.y <=80)
               {
                  currentMouseCtrl = PERSON_BTN_BACK;
                  currentPage = PAGE_PERSONAL_MANAGEMENT;
                }
             else if (m.x >= 180 && m.x <= 440 && m.y >= 130 && m.y <= 162) {
                  currentMouseCtrl = PERSON_BTN_YEAR;
                  GetPersonalRegistrationState()->focus = 0;
             }
             else if (m.x >= 180 && m.x <= 440 && m.y >= 160 && m.y <= 192) {
                  currentMouseCtrl = PERSON_BTN_YEAR;
                  GetPersonalRegistrationState()->focus = 1;
             }
             else if (m.x >= 350 && m.x <= 440 && m.y >= 220 && m.y <= 253) {
                  currentMouseCtrl = PERSON_BTN_YEAR;
                  QueryPersonalScrapStatus();
             }
             else if (m.x >= 40 && m.x <= 125 && m.y >= 315 && m.y <= 355) {
                  GetPersonalRegistrationState()->scrapReasonType = 0;
                  GetPersonalRegistrationState()->focus = 0;
             }
             else if (m.x >= 135 && m.x <= 275 && m.y >= 315 && m.y <= 355) {
                  GetPersonalRegistrationState()->scrapReasonType = 1;
                  GetPersonalRegistrationState()->focus = 0;
             }
             else if (m.x >= 285 && m.x <= 415 && m.y >= 315 && m.y <= 355) {
                  GetPersonalRegistrationState()->scrapReasonType = 2;
                  GetPersonalRegistrationState()->focus = 0;
             }
             else if ((m.x >= 40 && m.x <= 60 && m.y >= 370 && m.y <= 390) ||
                      (m.x >= 180 && m.x <= 430 && m.y >= 365 && m.y <= 400)) {
                  GetPersonalRegistrationState()->scrapReasonType = 3;
                  GetPersonalRegistrationState()->focus = 2;
             }
             else if (m.x >= 40 && m.x <= 440 && m.y >= 520 && m.y <= 575) {
                  currentMouseCtrl = PERSON_BTN_YEAR;
                  SavePersonalScrapUpdate();
             }
                break;
        case PAGE_SHARED_MANAGEMENT:
            if (m.x >= 60 && m.x <= 420 && m.y >= 270 && m.y <= 330) {
                currentMouseCtrl = SHARED_BTN_USE_VEHICLE;
                currentPage = PAGE_SHARED_USE_VEHICLE;
            }
            else if (m.x >= 60 && m.x <= 420 && m.y >= 390 && m.y <= 450) {
                currentMouseCtrl = SHARED_BTN_ORDER;
                currentPage = PAGE_SHARED_ORDER;
            }
            else if (m.x >= 60 && m.x <= 420 && m.y >= 510 && m.y <= 570) {
                currentMouseCtrl = MOUSE_NONE;
                LogoutSharedUser();
                currentPage = PAGE_LOGIN;
            }
            break;
        case PAGE_SHARED_ORDER:
            if (m.x >= 0 && m.x <= 80 && m.y >= 0 && m.y <= 80) {
                currentMouseCtrl = SHARED_BTN_BACK;
                currentPage = PAGE_SHARED_MANAGEMENT;
            }
            break;
        case PAGE_SHARED_USE_VEHICLE:
            if (m.x >= 0 && m.x <= 80 && m.y >= 0 && m.y <= 80) {
                currentMouseCtrl = SHARED_BTN_BACK;
                currentPage = PAGE_SHARED_MANAGEMENT;
            }
            else if (m.x >= 80 && m.x <= 280 && m.y >= 160 && m.y <= 220) {
                currentMouseCtrl = MOUSE_NONE;
            }
            else if (m.x >= 305 && m.x <= 405 && m.y >= 170 && m.y <= 220) {
                TryUnlockSharedVehicle();
                if (strncmp(GetSharedSignoutState()->sharedUseMessage, "开锁成功", 9) == 0) {
                    currentPage = PAGE_SHARED_SETTLEMENT;
                }
            }
            else if (m.x >= 60 && m.x <= 420 && m.y >= 470 && m.y <= 530) {
                SharedUserInfo* sharedState = GetSharedSignoutState();
                if (strcmp(sharedState->sharedUseStatus, "骑行中") == 0 && sharedState->settlementPlate[0] != '\0') {
                    currentMouseCtrl = SHARED_BTN_SETTLEMENT;
                    currentPage = PAGE_SHARED_SETTLEMENT;
                }
                else {
                    currentMouseCtrl = MOUSE_NONE;
                    strcpy(sharedState->sharedUseMessage, "请先成功解锁车辆，再进入结算");
                }
            }
            break;
        case PAGE_SHARED_SETTLEMENT:
            if (m.x >= 0 && m.x <= 80 && m.y >= 0 && m.y <= 80) {
                currentMouseCtrl = SHARED_BTN_BACK;
                currentPage = PAGE_SHARED_USE_VEHICLE;
            }
            else if (m.x >= 170 && m.x <= 300 && m.y >= 150 && m.y <= 185) {
                currentMouseCtrl = MOUSE_NONE;
                GetSharedSignoutState()->settlementFocus = 0;
            }
            else if (m.x >= 170 && m.x <= 300 && m.y >= 190 && m.y <= 225) {
                currentMouseCtrl = MOUSE_NONE;
                GetSharedSignoutState()->settlementFocus = 1;
            }
            else if (m.x >= 70 && m.x <= 410 && m.y >= 340 && m.y <= 400) {
                currentMouseCtrl = MOUSE_NONE;
                ConfirmSharedSettlementPayment();
                currentPage = PAGE_SHARED_MANAGEMENT;
            }
            else if (m.x >= 70 && m.x <= 410 && m.y >= 420 && m.y <= 480) {
                currentMouseCtrl = MOUSE_NONE;
                currentPage = PAGE_SHARED_USE_VEHICLE;
            }
            else if (m.x >= 70 && m.x <= 410 && m.y >= 540 && m.y <= 600) {
                currentMouseCtrl = MOUSE_NONE;
                currentPage = PAGE_SHARED_REPAIR;
            }
            break;
        case PAGE_SHARED_REPAIR:
            if (m.x >= 0 && m.x <= 80 && m.y >= 0 && m.y <= 80) {
                currentMouseCtrl = SHARED_BTN_BACK;
                currentPage = PAGE_SHARED_SETTLEMENT;
            }
            else if (m.x >= 45 && m.x <= 135 && m.y >= 240 && m.y <= 280) {
                currentMouseCtrl = MOUSE_NONE;
                strcpy(state->repairType, "无法开锁");
            }
            else if (m.x >= 145 && m.x <= 235 && m.y >= 240 && m.y <= 280) {
                currentMouseCtrl = MOUSE_NONE;
                strcpy(state->repairType, "刹车异常");
            }
            else if (m.x >= 245 && m.x <= 335 && m.y >= 240 && m.y <= 280) {
                currentMouseCtrl = MOUSE_NONE;
                strcpy(state->repairType, "电量异常");
            }
            else if (m.x >= 345 && m.x <= 435 && m.y >= 240 && m.y <= 280) {
                currentMouseCtrl = MOUSE_NONE;
                strcpy(state->repairType, "车身损坏");
            }
            else if (m.x >= 30 && m.x <= 450 && m.y >= 320 && m.y <= 450) {
                currentMouseCtrl = MOUSE_NONE;
            }
            else if (m.x >= 70 && m.x <= 410 && m.y >= 470 && m.y <= 530) {
                currentMouseCtrl = MOUSE_NONE;
                SubmitSharedRepair();
            }
            else if (m.x >= 70 && m.x <= 410 && m.y >= 550 && m.y <= 610) {
                currentMouseCtrl = MOUSE_NONE;
                ResetSharedRepairState();
                currentPage = PAGE_SHARED_SETTLEMENT;
            }
            break;
    }
}