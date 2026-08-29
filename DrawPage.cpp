#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>       // 包含标准输入输出头文件，用于 getchar()
#include <string.h>
#include <stdlib.h>
#include <windows.h>       // 包含 Windows 键盘状态接口
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量
#include "sharedsignout.h"
#include "personalregistration.h"
#include "personalinspection.h"

static void DrawTextAt(int x, int y, const char* text)         // 定义 DrawTextAt 函数，用于在指定位置绘制文本
{
    if (text == NULL || text[0] == '\0')         // 如果文本为空或首字符为空，则直接返回，不进行绘制
        return;

    setbkmode(TRANSPARENT);          // 设置背景模式为透明，以便文本不会覆盖背景
#ifdef UNICODE
    wchar_t wideText[256];
    if (MultiByteToWideChar(CP_ACP, 0, text, -1, wideText, 256) > 0)
        outtextxy(x, y, wideText);
#else
    outtextxy(x, y, text);
#endif
}

void DrawFirstPage() {             // 定义 DrawFirstPage 函数，用于绘制首页界面
    cleardevice();              // 清空窗口并用背景颜色填充
    setfillcolor(RGB(0,146,198));        // 设置填充颜色为浅蓝色
    fillrectangle(40,400,200,550);         // 绘制左边的填充矩形
    setbkmode(TRANSPARENT);
    settextstyle(25, 0, _T("黑体"));      // 设置文字样式：字号 25、方向 0、字体为“黑体”
    settextcolor(WHITE);                   // 设置文字颜色为白色
    outtextxy(50, 420, _T("个人电动车")); // 在坐标 (50, 420) 位置显示文字
    setbkmode(TRANSPARENT);
    settextstyle(15, 0, _T("黑体"));      // 设置文字样式：字号 15、方向 0、字体为“黑体”
    settextcolor(WHITE);                   // 设置文字颜色为白色
    outtextxy(50, 450, _T("注册上牌|年审管理|")); // 在坐标 (50, 450) 位置显示文字
    outtextxy(50, 470, _T("信息变更|报废管理|")); // 在坐标 (50, 470) 位置显示文字
    outtextxy(50, 490, _T("出入校园管理")); // 在坐标 (50, 490) 位置显示文字

    setfillcolor(RGB(0,146,198));        // 再次设置填充颜色为浅蓝色 
    fillrectangle(280,400,440,550);     // 绘制右边的填充矩形
    setbkmode(TRANSPARENT);
    settextstyle(25, 0, _T("黑体"));      // 设置文字样式：字号 25、方向 0、字体为“黑体”
    settextcolor(WHITE);                    // 设置文字颜色为白色
    outtextxy(290, 420, _T("共享电动车")); // 在坐标 (290, 420) 位置显示文字
    setbkmode(TRANSPARENT);     // 设置背景模式为透明
    settextstyle(15, 0, _T("黑体"));      // 设置文字样式：字号 15、方向 0、字体为“黑体”
    settextcolor(WHITE);                   // 设置文字颜色为白色
    outtextxy(290, 450, _T("车辆租借|还车结算|")); // 在坐标 (290, 450) 位置显示文字
    outtextxy(290, 470, _T("费用支付|故障报修")); // 在坐标 (290, 470) 位置显示文字

    IMAGE img;    // 声明一个 IMAGE 类型的对象 img，用于存储图片
    loadimage(&img, _T("xiaohui.png"), 120, 110);  // 加载图片文件 "xiaohui.png"，并将其缩放为 120x110
    putimage(0, 70, &img);               // 在坐标 (0, 70) 位置显示图片

    settextstyle(50, 0, _T("华文行楷"));      // 设置文字样式：字号 50、方向 0、字体为“华文行楷”
    settextcolor(RGB(0,75,132));                   // 设置文字颜色为蓝色
    outtextxy(120, 100, _T("华中科技大学")); // 在坐标 (120, 100) 位置显示文字
     
    settextstyle(15, 0, _T("Spectral"));      // 设置文字样式：字号 15、方向 0、字体为“Spectral”
    settextcolor(RGB(0,75,132));                   // 设置文字颜色为蓝色
    outtextxy(100, 145, _T("HUAZHONG UNIVERSITY OF SCIENCE AND TECHNOLOGY")); // 在坐标 (100, 145) 位置显示文字

    settextstyle(30, 0, _T("黑体"));      // 设置文字样式：字号 30、方向 0、字体为“黑体”
    settextcolor(RGB(0,75,132));                   // 设置文字颜色为蓝色
    outtextxy(130, 250, _T("电动车登记系统")); // 在坐标 (130, 250) 位置显示文字

}

void DrawSharedSignoutPage(void)
{
    cleardevice();
    settextstyle(30, 0, _T("黑体"));
    settextcolor(BLACK);
    outtextxy(180, 0, _T("账号登录"));
    outtextxy(0, 0, _T(" < "));
    setlinecolor(RGB(87,87,80));
    setlinestyle(PS_SOLID, 2);
    line(0, 40, 640, 40);

    IMAGE img;
    loadimage(&img, _T("xiaohui.png"), 120, 110);
    putimage(0, 70, &img);

    settextstyle(50, 0, _T("华文行楷"));
    settextcolor(RGB(0,75,132));
    outtextxy(120, 100, _T("华中科技大学"));

    settextstyle(15, 0, _T("Spectral"));
    settextcolor(RGB(0,75,132));
    outtextxy(100, 145, _T("HUAZHONG UNIVERSITY OF SCIENCE AND TECHNOLOGY"));

    settextstyle(30, 0, _T("黑体"));
    settextcolor(RGB(0,75,132));
    outtextxy(130, 250, _T("共享电动车系统"));

    setfillcolor(RGB(200,230,245));
    solidroundrect(20,320,460,540,12,12);
    setfillcolor(WHITE);
    solidroundrect(145,365,435,395,4,4);
    solidroundrect(145,415,435,445,4,4);
    solidroundrect(145,465,435,495,4,4);

    setfillcolor(RGB(0,75,132));
    fillroundrect(70, 550, 190, 600, 12, 12);
    settextcolor(WHITE);
    settextstyle(24, 0, _T("黑体"));
    outtextxy(105, 560, _T("登录"));

    setfillcolor(RGB(0,75,132));
    fillroundrect(290, 550, 410, 600, 12, 12);
    settextcolor(WHITE);
    settextstyle(20, 0, _T("黑体"));
    outtextxy(305, 560, _T("获取验证码"));

    setlinecolor(RGB(0,75,132));
    setlinestyle(PS_SOLID,2);
    roundrect(20,320,460,540,12,12);
    setlinecolor(RGB(87,87,80));
    setlinestyle(PS_SOLID, 2);
    rectangle(40, 360, 440, 510);
    line(40, 410, 440, 410);
    line(40, 460, 440, 460);
    line(140, 360, 140, 510);

    setbkmode(TRANSPARENT);
    settextstyle(18, 0, _T("黑体"));
    settextcolor(RGB(35,35,35));
    outtextxy(40, 370, _T("用户名"));
    outtextxy(40, 420, _T("手机号"));
    outtextxy(40, 470, _T("验证码"));

    const SharedUserInfo* state = GetSharedSignoutState();        // 获取共享登录状态信息的指针
    // 根据共享登录状态信息的内容，绘制输入框中的文本或提示信息
    settextstyle(18, 0, _T("黑体"));
    if (state->username[0] != '\0') {
        settextcolor(BLACK);
        DrawTextAt(150, 372, state->username);  // 绘制用户名输入框中的文本
    } else {
        settextcolor(RGB(140, 140, 140));
        outtextxy(150, 372, _T("请输入用户名"));
    }

    if (state->phone[0] != '\0') {
        settextcolor(BLACK);
        DrawTextAt(150, 422, state->phone);
    } else {
        settextcolor(RGB(140, 140, 140));
        outtextxy(150, 422, _T("请输入手机号"));
    }

    if (state->code[0] != '\0') {
        settextcolor(BLACK);
        DrawTextAt(150, 472, state->code);
    } else {
        settextcolor(RGB(140, 140, 140));
        outtextxy(150, 472, _T("请输入验证码"));
    }
    settextstyle(12, 0, _T("黑体"));
    // 根据登录状态信息的 loginSuccess 字段，设置状态提示信息的颜色
    if (state->loginSuccess)
        settextcolor(GREEN);
    else
        settextcolor(RGB(180, 60, 60));
    DrawTextAt(40, 610, state->message);
}

void DrawPersonalManagementPage()    // 定义 DrawPersonalManagementPage 函数，用于绘制个人电动车管理系统界面
{
    cleardevice();              // 清空窗口并用背景颜色填充
    settextstyle(30, 0, _T("黑体"));      // 设置文字样式：字号 30、方向 0、字体为“黑体”
    settextcolor(BLACK);                   // 设置文字颜色为黑色
    outtextxy(0, 0, _T(" < ")); // 在坐标 (0, 0) 位置显示文字

    setfillcolor(RGB(0,146,198)); // 设置填充颜色为蓝色
    setlinecolor(RGB(0,146,198)); // 新增统一边框颜色
    fillrectangle(30,335,210,425); //左上卡片【车辆注册/上牌】
    fillrectangle(270,335,450,425); //右上卡片【车辆年审管理】
    fillrectangle(30,440,210,530);//左下框【车辆信息变更】
    fillrectangle(270,440,450,530);//右下框【车辆报废管理】
    fillrectangle(30,545,450,605);//底部通栏长框【出入校园管理】

    IMAGE img;    // 声明一个 IMAGE 类型的对象 img，用于存储图片
    loadimage(&img, _T("xiaohui.png"), 120, 110);  // 加载图片文件 "xiaohui.png"，并将其缩放为 120x110
    putimage(0, 70, &img);// 在坐标 (0, 70) 位置显示图片

    settextstyle(50, 0, _T("华文行楷"));//设置文字样式：字号 50、方向 0、字体为“华文行楷”
    settextcolor(RGB(0,75,132));// 设置文字颜色为蓝色
    outtextxy(120, 100, _T("华中科技大学"));//在坐标 (120, 100) 位置显示文字
     
    settextstyle(15, 0, _T("Spectral"));//设置文字样式：字号 15、方向 0、字体为“Spectral”
    settextcolor(RGB(0,75,132));//设置文字颜色为蓝色
    outtextxy(100, 145, _T("HUAZHONG UNIVERSITY OF SCIENCE AND TECHNOLOGY"));//在坐标 (100, 145) 位置显示文字

    settextstyle(20,0,_T("黑体"));
    outtextxy(135,200,_T("个人电动车登记管理系统"));//界面说明：个人电动车管理系统
    settextcolor(WHITE);//卡片内白色文字
    
    setbkmode(TRANSPARENT);//设置文字背景为透明
    
    settextstyle(20,0,_T("黑体"));
    outtextxy(60,355,_T("车辆注册/上牌"));//左上卡片文字
    settextstyle(14,0,_T("黑体"));
    outtextxy(35,382,_T("新增车辆录入/车牌查重校验"));//左上卡片小文字
   
    settextstyle(20,0,_T("黑体"));
    outtextxy(300,355,_T("车辆年审管理"));//右上卡片文字
    settextstyle(14,0,_T("黑体"));
    outtextxy(275,382,_T("修改年审日期/更新年审状态"));//右上卡片小文字
    
    settextstyle(20,0,_T("黑体"));
    outtextxy(60,460,_T("车辆信息变更"));//左下卡片文字
    settextstyle(14,0,_T("黑体"));
    outtextxy(35,486,_T("车主信息修改/车辆备注修改"));//左下卡片小文字
    
    settextstyle(20,0,_T("黑体"));
    outtextxy(300,460,_T("车辆报废管理"));//右下卡片文字
    settextstyle(14,0,_T("黑体"));
    outtextxy(275,486,_T("报废车辆处理/更新报废状态"));//右下卡片小文字
    
    settextstyle(22,0,_T("黑体"));
    outtextxy(175,560,_T("出入校园管理"));//底部通栏长框文字
    
    settextstyle(14,0,_T("黑体"));
    outtextxy(160,590,_T("记录出入时间/更新校园状态"));//底部通栏长框小文字

    settextcolor(BLACK);
    settextstyle(12,0,_T("黑体"));
    outtextxy(120,615,_T("系统版本:V1.0"));//底部版权信息
}

void DrawPersonalRegistrationPage()//定义 DrawPersonalregistrationPage 函数，用于绘制个人电动车注册界面
{
    cleardevice();              // 清空窗口并用背景颜色填充
    setfillcolor(RGB(0,146,198)); // 设置填充颜色为青蓝色
    fillrectangle(0,0,480,80); //顶部通栏色块
    
    settextstyle(25,0,_T("黑体"));
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    outtextxy(150,30,_T("车辆注册上牌"));//顶部居中标题
    
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3); // 线条粗细为3
    line(30, 40, 40, 30);
    line(30, 40, 40, 50);//左上角返回箭头
    
    setfillcolor(WHITE);
    setlinecolor(RGB(220,220,220));
    fillroundrect(30,140,450,460,12,12);//表单大圆角矩形框

    settextstyle(18,0,_T("黑体"));
    settextcolor(BLACK);
    outtextxy(50,175,_T("车牌号:"));//车牌号
    outtextxy(50,215,_T("车主姓名:"));//车主姓名
    outtextxy(50,255,_T("院系:"));//院系
    outtextxy(50,295,_T("学号/工号:"));//学号/工号
    outtextxy(50,335,_T("联系方式:"));//电话
    outtextxy(50,375,_T("车型:"));//车型
    outtextxy(50,415,_T("注册日期:"));//注册日期
    
    setfillcolor(RGB(245,245,245));
    fillroundrect(240,170,420,200,6,6);//车牌号输入框
    fillroundrect(240,210,420,240,6,6);//车主姓名输入框
    fillroundrect(240,250,420,280,6,6);//院系输入框
    fillroundrect(240,290,420,320,6,6);//学号输入框
    fillroundrect(240,330,420,360,6,6);//电话输入框
    fillroundrect(240,370,420,400,6,6);//车型输入框
    fillroundrect(240,410,420,440,6,6);//注册日期输入框

    setlinecolor(RGB(230,230,230));
    line(30, 205, 450, 205);//车牌号输入框下划线
    line(30, 245, 450, 245);//车主姓名输入框下划线
    line(30, 285, 450, 285);//院系输入框下划线
    line(30, 325, 450, 325);//学号输入框下划线
    line(30, 365, 450, 365);//电话输入框下划线
    line(30, 405, 450, 405);//车型输入框下划线

    circle(120, 508, 8);
    settextcolor(RGB(80,80,80));
    settextstyle(20,0,_T("黑体"));
    outtextxy(135,500,_T("车牌号将进行唯一性校验"));//电动车单选圆圈
    
    setfillcolor(RGB(0,146,198));
    fillroundrect(40,560,440,610,25,25);//提交按钮
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(190,570,_T("确认注册"));//提交按钮文字

    const PersonalUserInfo* regState = GetPersonalRegistrationState(); // 获取个人注册状态信息的指针
    // 根据注册状态绘制输入框中的文本或提示信息
    settextstyle(16, 0, _T("黑体"));
    if (regState->licensePlate[0] != '\0') {
        settextcolor(BLACK);
        DrawTextAt(250, 178, regState->licensePlate);
    } else {
        settextcolor(RGB(140,140,140)); outtextxy(250, 178, _T("请输入车牌号"));
    }

    if (regState->ownerName[0] != '\0') { settextcolor(BLACK); DrawTextAt(250, 212, regState->ownerName); }
    else { settextcolor(RGB(140,140,140)); outtextxy(250, 212, _T("请输入车主姓名")); }

    if (regState->college[0] != '\0') { settextcolor(BLACK); DrawTextAt(250, 252, regState->college); }
    else { settextcolor(RGB(140,140,140)); outtextxy(250, 252, _T("请输入院系")); }

    if (regState->personalID[0] != '\0') { settextcolor(BLACK); DrawTextAt(250, 292, regState->personalID); }
    else { settextcolor(RGB(140,140,140)); outtextxy(250, 292, _T("请输入学号/工号")); }

    if (regState->ownerPhone[0] != '\0') { settextcolor(BLACK); DrawTextAt(250, 332, regState->ownerPhone); }
    else { settextcolor(RGB(140,140,140)); outtextxy(250, 332, _T("请输入手机号")); }

    if (regState->vehicleType[0] != '\0') { settextcolor(BLACK); DrawTextAt(250, 372, regState->vehicleType); }
    else { settextcolor(RGB(140,140,140)); outtextxy(250, 372, _T("请输入车型")); }

    if (regState->registrationDate[0] != '\0') { settextcolor(BLACK); DrawTextAt(250, 412, regState->registrationDate); }
    else { settextcolor(RGB(140,140,140)); outtextxy(250, 412, _T("YYYY-MM-DD")); }

    // 消息提示
    settextstyle(14,0,_T("黑体"));
    if (regState->message[0] != '\0') {
        if (regState->registered) settextcolor(GREEN); else settextcolor(RGB(180,60,60));
        DrawTextAt(40, 520, regState->message);
    }
}

void DrawPersonalInspectionPage()// 声明 DrawPersonalInspectionPage 函数，用于绘制个人电动车年审管理系统界面
{
    const PersonalInspectionState* state = GetPersonalInspectionState();
    cleardevice();
    cleardevice();              // 清空窗口并用背景颜色填充
    setfillcolor(RGB(0,146,198)); // 设置填充颜色为蓝色
    fillrectangle(0,0,480,80); //顶部蓝色标题栏

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3); // 线条粗细为3
    line(30, 40, 40, 30);
    line(30, 40, 40, 50);//左上角返回箭头白色
    
    setbkmode(TRANSPARENT);//设置文字背景为透明
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(150,32,_T("车辆年审管理"));//顶部居中标题

    setfillcolor(WHITE);
    setlinecolor(RGB(220,220,220));
    fillroundrect(30,95,450,230,18,18);
    settextcolor(BLACK);
    settextstyle(20,0,_T("黑体"));//顶部信息卡片:车牌号、车主、状态

    outtextxy(50,110,_T("车牌号:"));//车牌号
    line(40, 140, 440, 140);//分割线

    outtextxy(50,155,_T("车主姓名:"));//车主姓名
    line(40, 185, 440, 185);//分割线

    outtextxy(50,200,_T("当前状态:"));//当前年审状态

    settextstyle(16,0,_T("黑体"));
    settextcolor(state->licensePlate[0] ? BLACK : RGB(160,160,160));
    if (state->licensePlate[0]) DrawTextAt(180, 112, state->licensePlate);
    else outtextxy(180, 112, _T("请输入车牌号"));
    settextcolor(state->ownerName[0] ? BLACK : RGB(160,160,160));
    if (state->ownerName[0]) DrawTextAt(180, 157, state->ownerName);
    else outtextxy(180, 157, _T("请输入车主姓名"));
    settextcolor(state->queryFound ? RGB(0, 140, 80) : RGB(180, 60, 60));
    if (state->queryFound) DrawTextAt(180, 202, state->vehicleStatus);
    else outtextxy(180, 202, _T("未查询"));

    setfillcolor(RGB(0, 130, 220));
    fillroundrect(30, 500, 450, 535, 12, 12);
    settextcolor(WHITE);
    settextstyle(18, 0, _T("黑体"));
    outtextxy(205, 508, _T("查询状态"));
    settextcolor(RGB(180, 60, 60));
    settextstyle(13, 0, _T("黑体"));
    DrawTextAt(35, 615, state->message);

    fillroundrect(30,245,450,315,18,18);//更新年审日期

    setfillcolor(RGB(220,245,235));
    fillcircle(70, 280, 25);//圆形图标
    setlinecolor(RGB(0,160,110));
    circle(70, 280, 25);//左侧浅绿色圆形图标

    settextcolor(BLACK);
    settextstyle(20,0,_T("黑体"));
    outtextxy(110,260,_T("更新年审日期"));//更新年审日期文字
    settextstyle(14,0,_T("黑体"));
    settextcolor(RGB(90,90,90));
    outtextxy(110,290,_T("下次年审时间:"));//更新年审日期说明文字
    settextcolor(BLACK);
    if (state->queryFound)
        DrawTextAt(235, 290, state->nextInspectionDate);
    else
        outtextxy(235, 290, _T("未查询"));

    if (state->statusUpdated)
        setfillcolor(RGB(120, 200, 150));
    else
        setfillcolor(WHITE);
    fillroundrect(30,330,450,400,18,18);//更新年审状态

    setfillcolor(RGB(220,238,250));
    fillcircle(70, 365, 25);
    setlinecolor(RGB(0,120,200));
    circle(70, 365, 25);//左侧浅蓝色圆形图标

    settextcolor(state->statusUpdated ? RGB(0, 100, 50) : BLACK);
    settextstyle(20,0,_T("黑体"));
    outtextxy(110,345,state->statusUpdated ? _T("年审状态已更新") : _T("点击更新年审状态"));//更新年审状态文字
    settextstyle(14,0,_T("黑体"));
    settextcolor(RGB(90,90,90));
    outtextxy(110,375,_T("年审通过后状态更新为正常"));//更新年审状态说明文字

    fillroundrect(30,415,450,485,18,18);
    setfillcolor(WHITE);
    settextstyle(18,0,_T("黑体"));
    outtextxy(70,430,_T("年审通过后状态更新为正常"));//提交更新按钮文字
    settextstyle(15,0,_T("黑体"));
    outtextxy(70,460,_T("过期车辆将被标记为待审核"));//底部提示文字框

    setfillcolor(RGB(0,185,175));
    fillroundrect(140,550,340,600,25,25);
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(165,560,_T("保存年审信息"));//底部【保存年审信息】按钮
}

void DrawPersonalInformationPage() // 声明，用于绘制个人电动车信息变更页面界面
{ 
    const PersonalUserInfo* state = GetPersonalRegistrationState();
    setfillcolor(RGB(0,130,220));
    fillrectangle(0,0,480,80);//顶部蓝色标题栏

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID,3);
    line(30,40,40,30);
    line(30,40,40,50);//返回左箭头

    settextcolor(WHITE);
    settextstyle(30,0,_T("黑体"));
    outtextxy(138,25,_T("车辆信息变更"));//顶部居中标题
    
    setfillcolor(WHITE);
    fillroundrect(0,80,480,640,8,8);//车辆查询白色卡片

    settextcolor(BLACK);
    settextstyle(24,0,_T("黑体"));
    outtextxy(35,100,_T("车辆查询"));//卡片标题

    setlinecolor(RGB(220,220,220));
    rectangle(35,145,320,185);//车牌号输入框

    settextstyle(18,0,_T("黑体"));
    settextcolor(state->queryLicensePlate[0] ? BLACK : RGB(160,160,160));
    if (state->queryLicensePlate[0]) DrawTextAt(50, 155, state->queryLicensePlate);
    else outtextxy(50,155,_T("请输入车牌号"));//输入框提示文字

    setfillcolor(RGB(0,130,220));
    fillroundrect(350,145,430,185,8,8);//查询按钮
    settextcolor(WHITE);
    outtextxy(370,155,_T("查询"));//查询按钮文字

    setfillcolor(WHITE);
    fillroundrect(20,210,460,520,12,12);//车辆信息
    settextcolor(BLACK);
    settextstyle(20,0,_T("黑体"));
    outtextxy(35,230,_T("车主姓名"));//车主信息大卡片
    rectangle(280,225,440,260);//车主姓名输入框
    if (state->queryFound) {
        settextstyle(16,0,_T("黑体"));
        DrawTextAt(290, 232, state->queryResult.ownerName);
    }

    setlinecolor(RGB(230,230,230));
    line(20,265,460,265);//分割线

    settextstyle(18,0,_T("黑体"));
    outtextxy(35,280,_T("院系"));
    rectangle(280,270,440,305);//院系输入框
    if (state->queryFound) DrawTextAt(290, 280, state->queryResult.college);
    line(20,310,460,310);//院系行

    outtextxy(35,325,_T("学号/工号"));
    rectangle(280,315,440,350);//学号输入框
    if (state->queryFound) DrawTextAt(290, 325, state->queryResult.personalID);
    line(20,355,460,355);//学号行

    outtextxy(35,370,_T("联系方式"));
    rectangle(280,360,440,395);//联系方式输入框
    if (state->queryFound) DrawTextAt(290, 370, state->queryResult.ownerPhone);
    line(20,400,460,400);//联系方式行

    outtextxy(35,415,_T("车型"));
    rectangle(280,405,440,440);//车型输入框
    if (state->queryFound) DrawTextAt(290, 415, state->queryResult.vehicleType);
    line(20,445,460,445);//车型行

    outtextxy(35,460,_T("备注信息"));
    if (state->queryFound) DrawTextAt(290, 460, state->queryResult.vehicleStatus);

    settextcolor(RGB(100,100,100));
    settextstyle(18,0,_T("黑体"));
    if (state->queryMessage[0]) DrawTextAt(35, 525, state->queryMessage);

    setfillcolor(RGB(0,130,220));
    fillroundrect(30,560,450,610,35,35);
    settextcolor(WHITE);
    settextstyle(28,0,_T("黑体"));
    outtextxy(175,570,_T("保存变更"));//底部【确认修改】按钮

}
void DrawPersonalAccessPage1() // 声明，用于绘制个人电动车出入校园管理出入记录查询
{
    setfillcolor(RGB(0,130,220));
    fillrectangle(0,0,480,80);//顶部蓝色标题栏
    settextcolor(WHITE);
    settextstyle(30,0,_T("黑体"));
    outtextxy(138,25,_T("出入校园管理"));//顶部居中标题

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID,2);
    line(30,40,40,30);
    line(30,40,40,50);//返回左箭头

    setfillcolor(WHITE);
    fillroundrect(0,80,480,640,8,8);//白色背景

    setfillcolor(WHITE);
    fillroundrect(20,90,460,150,12,12);
    roundrect(20,90,460,150,12,12);
    settextstyle(20,0,_T("黑体"));//Tab栏

    settextcolor(RGB(0,130,220));
    outtextxy(55,105,_T("出入记录添加"));
    setlinecolor(RGB(0,130,220));
    line(50,138,175,138);//激活Tab：出入记录添加

    settextcolor(RGB(110,110,110));
    outtextxy(270,105,_T("出入记录查询"));//未激活Tab：出入记录查询

    setlinecolor(RGB(215,215,215));
    line(240,98,240,132);//中间分隔竖线

    setfillcolor(RGB(255,255,255));
    fillroundrect(20,170,460,240,12,12);
    roundrect(20,170,460,240,12,12);
    settextcolor(RGB(0,0,0));
    outtextxy(35,192,_T("车牌号:"));//车牌号卡片

    setlinecolor(RGB(190,190,190));
    roundrect(160,182,440,228,20,20);
    settextcolor(RGB(110,110,110));
    outtextxy(180,194,_T("请输入车牌号"));//车牌号输入框

    setfillcolor(WHITE);
    fillroundrect(20,260,460,470,12,12);
    roundrect(20,260,460,470,12,12);
    settextcolor(RGB(0,0,0));
    outtextxy(35,282,_T("出入类型:"));//出入类型+时间卡片

    setfillcolor(RGB(0,130,220));
    fillroundrect(160,300,280,350,22,22);
    settextcolor(WHITE);
    outtextxy(198,312,_T("入校"));//入校按钮（默认选中）

    setfillcolor(RGB(230,230,230));
    fillroundrect(300,300,420,350,22,22);
    settextcolor(RGB(110,110,110));
    outtextxy(338,312,_T("出校"));//出校按钮

    outtextxy(35,390,_T("出入时间:"));//记录时间

    setlinecolor(RGB(160,160,160));
    rectangle(65,418,95,448);//左侧复选框

    setlinecolor(RGB(190,190,190));
    roundrect(160,408,440,454,20,20);
    settextcolor(RGB(110,110,110));
    outtextxy(180,420,_T("请输入时间"));//时间输入框

    setfillcolor(RGB(0,130,220));
    fillroundrect(20,520,460,590,32,32);
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(190,542,_T("提交记录"));//底部提交按钮
}
void DrawPersonalAccessPage2()
{
   setfillcolor(RGB(0,130,220));
    fillrectangle(0,0,480,80);//顶部蓝色标题栏
    settextcolor(WHITE);
    settextstyle(30,0,_T("黑体"));
    outtextxy(138,25,_T("出入校园管理"));//顶部居中标题

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID,2);
    line(30,40,40,30);
    line(30,40,40,50);//返回左箭头

    setfillcolor(WHITE);
    fillroundrect(0,80,480,640,8,8);//白色背景

    setfillcolor(WHITE);
    fillroundrect(20,90,460,150,12,12);
    roundrect(20,90,460,150,12,12);
    settextstyle(20,0,_T("黑体"));//Tab栏

    settextcolor(RGB(110,110,110));
    outtextxy(55,105,_T("出入记录添加"));//未激活Tab：出入记录添加

    settextcolor(RGB(0,130,220));
    outtextxy(270,105,_T("出入记录查询"));
    setlinecolor(RGB(0,130,220));
    line(265,138,390,138);//激活Tab：出入记录查询

    setlinecolor(RGB(215,215,215));
    line(240,98,240,132);//中间分隔竖线

    fillroundrect(30,185,450,265,16,16);//车牌号输入框
    setlinecolor(RGB(130,130,130));
    setfillcolor(RGB(255,255,255));
    fillroundrect(50,200,320,245,22,22);//查询结果显示框
    roundrect(50,200,320,245,22,22);
    settextstyle(18,0,_T("黑体"));
    settextcolor(RGB(110,110,110));
    outtextxy(60,215,_T("请输入车牌号"));//查询结果显示框提示文字

    setfillcolor(RGB(0,130,220));
    fillroundrect(345,200,430,245,22,22);//查询按钮
    settextstyle(20,0,_T("黑体"));
    settextcolor(WHITE);
    outtextxy(368,211,_T("查询"));//查询按钮文字

    setfillcolor(RGB(255,255,255));
    fillroundrect(30,280,450,525,16,16);
    settextstyle(20,0,_T("黑体"));
    settextcolor(BLACK);
    outtextxy(45,302,_T("出入记录列表"));//出入记录列表

    setfillcolor(RGB(225,242,255));
    fillroundrect(45,340,435,375,12,12);//出入记录显示框
    settextstyle(18,0,_T("黑体"));
    settextcolor(RGB(110,110,110));
    outtextxy(60,346,_T("车牌号"));
    outtextxy(145,346,_T("车主姓名"));
    outtextxy(245,346,_T("出入类型"));
    outtextxy(335,346,_T("出入时间"));//表头底色

    line(130,340,130,495);
    line(240,340,240,495);
    line(325,340,325,495);//表头竖线

    rectangle(45,375,435,405);//出入记录显示框边框
    settextstyle(12,0,_T("宋体"));
    outtextxy(52,380,_T("鄂A12345"));
    outtextxy(147,380,_T("张三"));
    outtextxy(257,380,_T("入校"));
    outtextxy(334,380,_T("2024-06-01 08:30"));//第一条出入记录

    rectangle(45,405,435,435);//出入记录显示框边框
    outtextxy(52,410,_T("鄂A67890"));
    outtextxy(147,410,_T("李四"));
    outtextxy(257,410,_T("离校"));
    outtextxy(334,410,_T("2024-06-01 17:22"));//第二条出入记录

    rectangle(45,435,435,465);//出入记录显示框边框
    outtextxy(52,440,_T("鄂A54321"));
    outtextxy(147,440,_T("李四"));
    outtextxy(257,440,_T("入校"));
    outtextxy(334,440,_T("2024-06-02 09:15"));//第三条出入记录

    rectangle(45,465,435,495);//出入记录显示框边框
    outtextxy(52,470,_T("鄂A98765"));
    outtextxy(147,470,_T("张三"));
    outtextxy(257,470,_T("离校"));
    outtextxy(334,470,_T("2024-06-02 18:05"));//第四条出入记录

}
void DrawPersonalScrapPage()// 声明 DrawPersonalScrapPage 函数，用于绘制个人电动车报废管理系统界面
{
    cleardevice();              // 清空窗口并用背景颜色填充
    setfillcolor(RGB(0,146,198)); // 设置填充颜色为蓝色
    fillrectangle(0,0,480,80); //顶部蓝色标题栏

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3); // 线条粗细为3
    line(30, 40, 40, 30);
    line(30, 40, 40, 50);//左上角返回箭头白色
    
    setbkmode(TRANSPARENT);//设置文字背景为透明
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(150,32,_T("车辆报废管理"));//顶部居中标题
    
    setfillcolor(WHITE);
    fillroundrect(0,80,480,640,8,8);//白色背景

    setlinecolor(RGB(200,200,200));
    fillroundrect(30,90,450,210,22,22);//车辆报废信息显示框
    settextstyle(18,0,_T("宋体"));
    settextcolor(BLACK);
    outtextxy(50,108,_T("车辆信息"));//小标题
    outtextxy(50,138,_T("车牌号:"));
    outtextxy(50,168,_T("车主姓名:"));//车辆信息卡片

    setfillcolor(RGB(255,255,255));
    fillroundrect(30,225,450,460,18,18);//报废信息显示框

    settextstyle(18,0,_T("黑体"));
    outtextxy(50,240,_T("报废车辆处理"));//小标题

    fillroundrect(40,270,125,310,22,22);
    settextstyle(16,0,_T("黑体"));
    settextcolor(BLACK);
    outtextxy(50,280,_T("车体损坏"));//报废车辆处理按钮

    setfillcolor(RGB(0,130,220));
    fillroundrect(135,270,275,310,22,22);
    settextcolor(WHITE);
    outtextxy(160,280,_T("达到使用年限"));//达到使用年限按钮（选中蓝色按钮）

    setfillcolor(RGB(255,255,255));
    fillroundrect(285,270,415,310,22,22);
    settextcolor(BLACK);
    outtextxy(300,280,_T("丢失无法找回"));//丢失无法找回按钮

    settextstyle(20,0,_T("宋体"));
    settextcolor(BLACK);
    rectangle(40,325,60,345);
    outtextxy(75,325,_T("其他原因"));
    fillroundrect(180,320,430,355,18,18);
    settextcolor(RGB(110,110,110));
    outtextxy(190,327,_T("请输入其他原因"));//其他原因复选栏+输入框
 
    setfillcolor(RGB(255,210,0));
    fillrectangle(30,365,450,400);
    settextcolor(BLACK);
    settextstyle(18,0,_T("宋体"));
    outtextxy(80,373,_T("注意：标记报废后将禁止该车后续操作"));//黄色警告提示栏

    settextcolor(BLACK);
    settextstyle(18,0,_T("宋体"));
    outtextxy(60,420,_T("报废时间"));
    setfillcolor(RGB(245,245,245));
    fillroundrect(180,410,430,445,20,20);
    settextcolor(RGB(110,110,110));
    settextstyle(14,0,_T("宋体"));
    outtextxy(190,420,_T("请输入报废时间：XXXX-XX-XX"));//报废时间输入框

    setfillcolor(RGB(230,30,30));
    fillroundrect(40,475,440,530,30,30);
    settextstyle(28,0,_T("黑体"));
    settextcolor(WHITE);
    outtextxy(175,490,_T("确认报废"));//底部红色【确认报废】按钮
}
void DrawSharedManagementPage()//定义 DrawSharedManagementPage 函数，用于绘制共享电动车管理系统界面
{
   setfillcolor(WHITE);
   fillrectangle(0,0,480,640);//白色背景

   setbkmode(TRANSPARENT);//设置文字背景为透明
   settextcolor(BLACK);
   settextstyle(25,0,_T("黑体"));
   outtextxy(150,80  ,_T("共享电动车管理"));//顶部居中标题

   setlinecolor(BLACK);
   setlinestyle(PS_SOLID,2);
   line(30,40,40,30);
   line(30,40,40,50);//返回左箭头

   setlinecolor(RGB(200,200,200));
   fillroundrect(30,120,450,240,22,22);//登录状态显示框

   settextstyle(18,0,_T("宋体"));
   settextcolor(BLACK);
   outtextxy(50,138,_T("姓名：张三"));
   outtextxy(50,168,_T("学号:U202501001"));
   outtextxy(50,198,_T("当前状态:已登录"));//登录状态信息卡片
   
   setfillcolor(RGB(0,130,220));
   fillroundrect(60,270,420,330,18,18);
   settextcolor(WHITE);
   settextstyle(24,0,_T("宋体"));
   int btn1_w=textwidth(_T("输入用车"));
   outtextxy(35+(420-btn1_w)/2,285,_T("输入用车"));//输入用车按钮

    setfillcolor(RGB(0,130,220));
    fillroundrect(60,350,420,410,18,18);
    settextcolor(WHITE);
    settextstyle(24,0,_T("宋体"));
    int btn2_w=textwidth(_T("换车结算"));
    outtextxy(35+(420-btn2_w)/2,365,_T("换车结算"));//换车结算按钮

    setfillcolor(RGB(0,130,220));
    fillroundrect(60,430,420,490,18,18);
    settextcolor(WHITE);
    settextstyle(24,0,_T("宋体"));
    int btn3_w=textwidth(_T("我的订单"));
    outtextxy(35+(420-btn3_w)/2,445,_T("我的订单"));//我的订单按钮

    setfillcolor(RGB(0,130,220));
    fillroundrect(60,510,420,570,18,18);
    settextcolor(WHITE);
    settextstyle(24,0,_T("宋体"));
    int btn4_w=textwidth(_T("退出登录"));
    outtextxy(35+(420-btn4_w)/2,525,_T("退出登录"));//退出登录按钮

    settextcolor(BLACK);
    settextstyle(12,0,_T("黑体"));
    int tip_w=textwidth(_T("请选择您的操作"));
    outtextxy(240-tip_w/2, 580, _T("请选择您的操作"));//底部提示文字 
}

void DrawSharedUseVehiclePage()//定义共享电动车输入用车界面绘制函数
{
    cleardevice();              // 清空窗口并用背景颜色填充
    setfillcolor(RGB(0,146,198)); // 设置填充颜色为蓝色
    fillrectangle(0,0,480,80); //顶部蓝色标题栏

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3); // 线条粗细为3
    line(30, 40, 40, 30);
    line(30, 40, 40, 50);//左上角返回箭头白色
    
    setbkmode(TRANSPARENT);//设置文字背景为透明
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(185,32,_T("输入用车"));//顶部居中标题
    
    setfillcolor(WHITE);
    fillroundrect(60,100,420,240,22,22);//车辆编号输入框
    setlinecolor(RGB(200,200,200));
    fillroundrect(60,100,420,240,22,22);//车辆编号输入框

    setfillcolor(RGB(220,220,220));
    fillroundrect(80,160,280,220,22,22);//输入框

    settextstyle(18,0,_T("黑体"));
    settextcolor(BLACK);
    outtextxy(80,120,_T("请输入车辆编号"));//提示文字：请输入车辆编号

    settextcolor(BLACK);
    settextstyle(30,0,_T("黑体"));
    outtextxy(90,180,_T("E2001"));//车辆编号输入框默认文字

    setfillcolor(RGB(0,146,198));
    fillroundrect(305,170,405,220,32,32);
    settextcolor(WHITE);
    settextstyle(18,0,_T("黑体"));
    int lock_w = textwidth(_T("确认开锁"));
    outtextxy(355-lock_w/2, 190, _T("确认开锁"));//确认开锁按钮

    setfillcolor(WHITE);
    fillroundrect(60,270,420,400,32,32);
    setlinecolor(RGB(200,200,200));
    fillroundrect(60,270,420,400,32,32);//车辆状态显示输入框

    settextcolor(BLACK);
    settextstyle(20,0,_T("黑体"));
    outtextxy(80,300,_T("车辆状态:空闲"));
    outtextxy(80,330,_T("预计费用:起步价2元"));
    outtextxy(80,360,_T("可用电量:80%"));

   setfillcolor(RGB(0,146,198));
   fillroundrect(60,500,420,560,32,32);
   settextcolor(WHITE);
   settextstyle(20,0,_T("黑体"));
   int btn_w = textwidth(_T("返回主菜单"));
   outtextxy(240-btn_w/2, 520, _T("返回主菜单"));//返回主菜单按钮

}

void DrawSharedSettlementPage() {
    cleardevice();
    setfillcolor(RGB(0,146,198));
    fillrectangle(0,0,480,80);

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 3);
    line(30, 40, 40, 30);
    line(30, 40, 40, 50);

    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(25,0,_T("黑体"));
    outtextxy(165,32,_T("换车结算"));

   
    
    setfillcolor(RGB(0,146,198));
    fillroundrect(70,520,410,570,28,28);
    settextcolor(WHITE);
    settextstyle(22,0,_T("黑体"));
    int settle_w = textwidth(_T("确认结算"));
    outtextxy(240 - settle_w / 2, 535, _T("确认结算"));
}
