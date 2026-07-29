#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>       // 包含标准输入输出头文件，用于 getchar()
#include <conio.h>       // 包含 _getch()，用于按任意键继续
#include <string.h>
#include "DrawPage.h"  // 包含自定义的 DrawPage.h 头文件，声明函数
#include "MouseCtrl.h" // 包含自定义的 MouseCtrl.h 头文件，声明函数和变量


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
    settextcolor(WHITE);                   // 设置文字颜色为白色
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

void DrawSharedSignoutPage() {// 在这里实现共享电动车登录首页的绘制
    cleardevice();              // 清空窗口并用背景颜色填充
    settextstyle(30, 0, _T("黑体"));      // 设置文字样式：字号 30、方向 0、字体为“黑体”
    settextcolor(BLACK);                   // 设置文字颜色为黑色
    outtextxy(180, 0, _T("账号登录")); // 在坐标 (180, 0) 位置显示文字
    outtextxy(0, 0, _T(" < ")); // 在坐标 (0, 0) 位置显示文字
    setlinecolor(RGB(87,87,80));   // 设置线条颜色为灰色
    setlinestyle(PS_SOLID, 2); // 线条粗细为2
    line(0, 40, 640, 40);// 绘制一条水平线，起点坐标为 (0, 40)，终点坐标为 (640, 40)

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
    outtextxy(130, 250, _T("共享电动车系统")); // 在坐标 (130, 250) 位置显示文字

    setfillcolor(RGB(200,230,245));  // 设置填充颜色为浅蓝色
    solidroundrect(20,320,460,540,12,12);    // 绘制一个填充的圆角矩形，左上角坐标为 (20, 320)，右下角坐标为 (460, 540)，圆角宽度和高度均为 12

    // 登录按钮
    setfillcolor(RGB(0,75,132));
    fillroundrect(170, 550, 310, 600, 12, 12);
    settextcolor(WHITE);
    settextstyle(24, 0, _T("黑体"));
    outtextxy(200, 560, _T("登录"));

    setlinecolor(RGB(0,75,132));   // 设置线条颜色为深蓝色
    setlinestyle(PS_SOLID,2);      // 设置线条样式为实线，粗细为 2
    roundrect(20,320,460,540,12,12);   // 绘制一个圆角矩形，左上角坐标为 (20, 320)，右下角坐标为 (460, 540)，圆角宽度和高度均为 12
    setlinecolor(RGB(87,87,80));   // 设置线条颜色为灰色
    setlinestyle(PS_SOLID, 2); // 线条粗细为2
    rectangle(40, 360, 440, 510);// 绘制一个矩形，左上角坐标为 (40, 360)，右下角坐标为 (440, 510)
    line(40, 410, 440, 410);// 绘制一条水平线，起点坐标为 (40, 410)，终点坐标为 (440, 410)
    line(40, 460, 440, 460);// 绘制一条水平线，起点坐标为 (40, 460)，终点坐标为 (440, 460)
    line(140, 360, 140, 510);// 绘制一条垂直线，起点坐标为 (140, 360)，终点坐标为 (140, 510)
    setbkmode(TRANSPARENT);       // 设置背景模式为透明
    settextstyle(30, 0, _T("黑体"));      // 设置文字样式：字号 30、方向 0、字体为“黑体”
    settextcolor(RGB(35,35,35));         // 设置文字颜色为灰色
    outtextxy(40, 370, _T("用户名")); // 在坐标 (40, 370) 位置显示文字
    outtextxy(40, 420, _T("手机号")); // 在坐标 (40, 420) 位置显示文字
    outtextxy(40, 470, _T("验证码")); // 在坐标 (40, 470) 位置显示文字
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
    outtextxy(120,615,_T("当前登录:学号123456789|系统版本:V1.0"));//底部版权信息
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
}

void DrawPersonalInspectionPage()// 声明 DrawPersonalInspectionPage 函数，用于绘制个人电动车年审管理系统界面
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
    outtextxy(150,32,_T("车辆年审管理"));//顶部居中标题

    setfillcolor(WHITE);
    setlinecolor(RGB(220,220,220));
    fillroundrect(30,95,450,230,18,18);
    settextcolor(BLACK);
    settextstyle(20,0,_T("黑体"));//顶部信息卡片:车牌号、车主、状态

    outtextxy(50,110,_T("车牌号:鄂A12345"));//车牌号
    line(40, 140, 440, 140);//分割线

    outtextxy(50,155,_T("车主姓名:张三"));//车主姓名
    line(40, 185, 440, 185);//分割线

    outtextxy(50,200,_T("当前状态:正常"));//当前年审状态

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
    outtextxy(110,290,_T("设置下次年审时间"));//更新年审日期说明文字

    fillroundrect(30,330,450,400,18,18);//更新年审状态

    setfillcolor(RGB(220,238,250));
    fillcircle(70, 365, 25);
    setlinecolor(RGB(0,120,200));
    circle(70, 365, 25);//左侧浅蓝色圆形图标

    settextcolor(BLACK);
    settextstyle(20,0,_T("黑体"));
    outtextxy(110,345,_T("更新年审状态"));//更新年审状态文字
    settextstyle(14,0,_T("黑体"));
    settextcolor(RGB(90,90,90));
    outtextxy(110,375,_T("年审通过或标记待审核"));//更新年审状态说明文字

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
    setfillcolor(RGB(0,130,220));
    fillrectangle(0,0,480,120);//顶部蓝色标题栏

    setlinecolor(WHITE);
    setlinestyle(PS_SOLID,3);
    line(30,55,60,35);
    line(30,50,60,75);//返回左箭头

    settextcolor(WHITE);
    settextstyle(32,0,_T("黑体"));
    outtextxy(130,45,_T("车辆信息变更"));//顶部居中标题
    
    setfillcolor(WHITE);
    fillroundrect(20,140,460,270,12,12);//车辆查询白色卡片

    settextcolor(BLACK);
    settextstyle(26,0,_T("黑体"));
    outtextxy(35,155,_T("车辆查询"));//卡片标题

    setlinecolor(RGB(220,220,220));
    rectangle(35,190,320,240);//车牌号输入框

    settextstyle(20,0,_T("黑体"));
    settextcolor(RGB(160,160,160));
    outtextxy(50,202,_T("请输入车牌号"));//输入框提示文字

    setfillcolor(RGB(0,130,220));
    fillroundrect(350,190,430,240,8,8);//查询按钮
    settextcolor(WHITE);
    outtextxy(370,202,_T("查询"));//查询按钮文字

    setfillcolor(WHITE);
    fillroundrect(20,290,460,600,12,12);//车辆信息
    settextcolor(BLACK);
    settextstyle(26,0,_T("黑体"));
    outtextxy(35,305,_T("车辆信息"));//车主信息大卡片

    setlinecolor(RGB(230,230,230));
    line(35,345,440,345);//分割线

    settextstyle(22,0,_T("黑体"));
    outtextxy(35,360,_T("院系"));
    rectangle(280,350,440,385);//院系输入框
    line(35,400,440,400);//院系行

    outtextxy(35,415,_T("学号/工号"));
    rectangle(280,405,440,440);//学号输入框
    line(35,445,440,445);//学号行

    outtextxy(35,460,_T("联系方式"));
    rectangle(280,450,440,485);//联系方式输入框
    line(35,490,440,490);//联系方式行

    outtextxy(35,505,_T("车型"));
    rectangle(280,495,440,530);//车型输入框
    line(35,535,440,535);//车型行

    outtextxy(35,550,_T("备注信息行"));
    rectangle(280,540,440,575);//备注信息行

    settextcolor(RGB(100,100,100));
    settextstyle(18,0,_T("黑体"));
    outtextxy(35,580,_T("修改后请确认信息是否完整")); // 底部小字提醒

    setfillcolor(RGB(0,130,220));
    fillroundrect(30,625,450,690,35,35);
    settextcolor(WHITE);
    settextstyle(28,0,_T("黑体"));
    outtextxy(175,645,_T("保存变更"));//底部【确认修改】按钮

    _getch();
    closegraph();
}
