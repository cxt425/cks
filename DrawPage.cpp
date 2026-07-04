#include <graphics.h>   // 包含 EasyX 图形库头文件
#include <stdio.h>       // 包含标准输入输出头文件，用于 getchar()

void DrawFirstPage() {
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

    IMAGE img;
    loadimage(&img, _T("xiaohui.png"), 120, 110);  // 加载图片文件 "xiaohui.png"，并将其缩放为 120x110 像素
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

    getchar();                  // 等待用户按键，防止窗口立即关闭
    closegraph();               // 关闭图形窗口并释放资源
}