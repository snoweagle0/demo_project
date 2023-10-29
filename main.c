#include "lcd.h"
#include "bmp.h"
//#include "gy39_mq2.h"
//#include <pthread.h>
#include <linux/input.h>
#include <stdlib.h>

typedef struct
{
    int x;
    int y;
}Point;



int get_touch()
{
    //1.打开触摸屏
    int tc_fd = open("/dev/input/event0",O_RDONLY);
    if (tc_fd == -1)
    {
        printf("open fail!\n");
        return -1;
    }
    
    struct input_event ev;
    Point start = {-1,-1};

    //一直循环获取触摸事件
    system("madplay /home/wll/project/hoyomix.mp3 &");
    while (1)
    {
        read(tc_fd,&ev,sizeof(ev));
        if(ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (start.x == -1)
            {
                start.x = ev.value;
            }
            
        }
        if(ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (start.y == -1)
            {
                start.y = ev.value;
            }
        }
        printf("start(%d,%d)\n",start.x,start.y);
        if(ev.type = EV_KEY && ev.code == BTN_TOUCH && ev.value
        == 0)
        {
            if (start.x >= 0 && start.x < 400 && start.y >=0 && start.y < 480)
            {
                return 1;
            }
            if (start.x >= 400 && start.x < 1024 && start.y >=0 && start.y < 600)//800 480
            {
                return 2;
            }
            
        }
    }
    system("killall -KILL madplay");
    close(tc_fd);
}

int main(void)
{
	ssize_t rsize = 0;
	lcd_init();
	lcd_draw_bmp( 0,0, "8.bmp");

	  //定义了一个字符串数组
    char *path[20] = {"8.bmp","glx.bmp","t.bmp","p.bmp"};
    int rec;//获取你tc的返回值
    int i = 0;
    
    while (1)
    {
       lcd_draw_bmp(0,0,path[i]);
       rec = get_touch();//获取你tc的返回值
       if (rec == 1)
       {
            //切换上一张图片，如果你到数组的末尾，怎么办
			if(i>0)
			{
				i--;
			}
            else {i = 3;}
       }
       else if(rec == 2)
       {
            //切换下一张图片
			if(i<3)
			{
				i++;
			}
            else{ i = 0;}
       }
    }
    return 0;
}