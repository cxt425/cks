#include <graphics.h>
#include <stdio.h>

int main() {
    initgraph(640, 480);        // 创建 640x480 窗口
    setbkcolor(WHITE); 
    cleardevice();          // 设置背景颜色为白色
    setcolor(RED);              // 设置颜色为红色
    circle(320, 240, 100);      // 在中心画半径为100的圆
    getchar();                  // 按任意键继续
    closegraph();               // 关闭图形窗口
    return 0;
}
