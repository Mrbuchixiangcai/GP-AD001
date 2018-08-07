//头文件header file//
#include "app_main.h"

//宏定义macro definition//

//类型定义byte definition//
COLOR_MODE color_mode; //颜色切换枚举变量类型
COLOR_MODE alarm_color_mode;//闹钟专用

//变量定义variable definition//
uint8_t  R_val;
uint8_t  G_val;
uint8_t  B_val;

uint8_t  curR_val;
uint8_t  curG_val;
uint8_t  curB_val;
uint8_t  userR_val;
uint8_t  userG_val;
uint8_t  userB_val;
uint8_t  sumRGB_val;
uint8_t  Light_Brightness; //Light_Brightness光线亮度
uint8_t  play_rgb_index; //颜色索引标记
uint16_t cnt_rgb_pause; //计数_颜色_暂停
uint8_t  play_rgb_speed; //速度

//标志位定义flags definetion//
uint8_t  tickRGB; //tick标记

//函数定义function definetion//
/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：RGB亮度调节函数
*******************************************************************/
void led_rgb_drive(void)
{
	if(--R_val==0) 
		LED_RED(0); //端口复位宏定义
	if(--G_val==0)
		LED_GREEN(0);
	if(--B_val==0)
		LED_BLUE(0);
	if(--sumRGB_val==0) //亮度调节，三个等级1/2/3
	{
		if(Light_Brightness==0)
		{
			R_val=0;
			G_val=0;
			B_val=0;
		}
		else if(Light_Brightness==1)	
		{
			R_val=curR_val/4;
			G_val=curG_val/4;
			B_val=curB_val/4;
		}
		else if(Light_Brightness==2)	
		{
			R_val=curR_val/2;
			G_val=curG_val/2;
			B_val=curB_val/2;
		}
		else  //if(Light_Brightness==3)
		{
			R_val=curR_val;
			G_val=curG_val;
			B_val=curB_val;
		}
		if((PlayMode==PLAY_BT) && (BT_LED_DET()))
			SET_BT_LED();
		else
	   		CLR_BT_LED();
		if(R_val) 
			LED_RED(1); //端口置位宏定义
		if(G_val)
			LED_GREEN(1);
		if(B_val)
			LED_BLUE(1);
		tickRGB=1;
	}
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：在RGB_Handle(void)函数中，单独选中某个颜色时，把重复性的语句，打包成函数调用
*******************************************************************/
void RGBColorSet(uint8_t R,uint8_t G,uint8_t B)
{
	curR_val=R;
	curG_val=G;
	curB_val=B;
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：RGB颜色渐变函数
*******************************************************************/
void led_rgb_play(void)
{  
	if(++play_rgb_speed<4) //自加，到10就执行if后面的语句
		return;
	play_rgb_speed=0; //play_rgb_speed到10清零
	if(play_rgb_index == 0) //红
	{
		if(curR_val < 255) //到255就不加了
			curR_val++;
		if(curG_val > 0) 
			curG_val--;
		if(curB_val > 0) 
			curB_val--;				
		if((curR_val==255) && (curG_val==0) && (curB_val==0) && (++cnt_rgb_pause>30)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index++; //颜色自加时颜色索引play_reg_index自加
		}
	}
	else if(play_rgb_index == 1) //橙
	{
		if(curR_val < 255) 
		 	curR_val++;
		if(curR_val > 255) 
		 	curR_val--;				
		if(curG_val < 128) 
			 curG_val++;
		if(curG_val > 128) 
		 	curG_val--;
		if(curB_val > 0) 
		 	curB_val--;
		if((curR_val==255) && (curG_val==128) && (curB_val==0) && (++cnt_rgb_pause>30)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index++;
		}
	}
	else if(play_rgb_index == 2)//黄 
	{
		if(curR_val < 255) 
			curR_val++;
		if(curG_val < 255) 
			curG_val++;				
		if(curB_val > 0) 
			curB_val--;
		if((curR_val==255) && (curG_val==255) && (curB_val==0) && (++cnt_rgb_pause>30)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index++;
		}
	}		
	else if(play_rgb_index == 3)//绿
	{
		if(curR_val > 0) 
		 	curR_val--;
		if(curG_val < 255) 
		 	curG_val++;				
		if(curB_val > 0) 
		 	curB_val--;
		if((curR_val==0) && (curG_val==255) && (curB_val==0) && (++cnt_rgb_pause>30)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index++;
		}
	}		
	else if(play_rgb_index == 4)//蓝
	{
		if(curR_val > 0) 
		 	curR_val--;
		if(curG_val > 0) 
		 	curG_val--;
		if(curB_val < 255) 
		 	curB_val++;
		if((curR_val==0) && (curG_val==0) && (curB_val==255) && (++cnt_rgb_pause>30)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index++;
		}
	}	
	else if(play_rgb_index == 5)//青
	{
		if(curR_val > 0) 
		 	curR_val--;
		if(curG_val < 255) 
		 	curG_val++;
		if(curB_val < 255) 
		 	curB_val++;
		if((curR_val==0) && (curG_val==255) && (curB_val==255) && (++cnt_rgb_pause>30)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index++;
		}
	}		
	else //紫
	{
		if(curR_val < 255)
			curR_val++;
		if(curG_val > 0) 
			curG_val--;				
		if(curB_val < 255) 
			curB_val++;
		if((curR_val==255) && (curG_val==0) && (curB_val==255) && (++cnt_rgb_pause>50)) 
		{
			cnt_rgb_pause=0;
			play_rgb_index=0;
		}
    }	
}

/*******************************************************************
函数原型：
输入参数：
输出参数：
函数功能：RGB颜色
*******************************************************************/
void RGB_Handle(void)
{
	if(tickRGB==0)
		return;
	tickRGB=0;
	if((color_mode==COLOR_CHANGE) && (color_mode!=color_mode_bk))//color_mode_bk在开关机时用作备份，不冲突
	{
		color_mode_bk=color_mode;
		play_rgb_index = 0;
		curR_val=127;
		curG_val=0;
		curB_val=0;	
	}
	if((PlayMode==PLAY_OFF) || (color_mode==COLOR_OFF))
	{
		RGBColorSet(0,0,0);
	}
	else if(color_mode == COLOR_WHITE)
	{
		RGBColorSet(255,255,255);
	}
	else if(color_mode == COLOR_RED)
	{
		RGBColorSet(255,0,0);
	}
	else if(color_mode == COLOR_ORANGE)
	{
		RGBColorSet(255,128,0);
	}
	else if(color_mode == COLOR_YELLOW)
	{
		RGBColorSet(255,255,0);
	}
	else if(color_mode == COLOR_GREEN)
	{
		RGBColorSet(0,255,0);
	}
	else if(color_mode == COLOR_CYAN)
	{
		RGBColorSet(0,255,255);
	}
	else if(color_mode == COLOR_BLUE)
	{
		RGBColorSet(0,0,255);
	}
	else if(color_mode == COLOR_PURPLE)
	{
		RGBColorSet(128,0,255);
	}
	else if(color_mode == COLOR_USER)
	{
		curR_val=userR_val;
		curG_val=userG_val;
		curB_val=userB_val;
		if((userR_val==255)&&(userG_val==255)&&(userB_val==255))
		{
			color_mode=COLOR_WHITE;
		}
		else if((userR_val==255)&&(userG_val==0)&&(userB_val==0))
		{
			color_mode=COLOR_RED;
		}
		else if((userR_val==255)&&(userG_val==128)&&(userB_val==0))
		{
			color_mode=COLOR_ORANGE;
		}
		else if((userR_val==255)&&(userG_val==255)&&(userB_val==0))
		{
			color_mode=COLOR_YELLOW;
		}
		else if((userR_val==0)&&(userG_val==255)&&(userB_val==0))
		{
			color_mode=COLOR_GREEN;
		}
		else if((userR_val==0)&&(userG_val==255)&&(userB_val==255))
		{
			color_mode=COLOR_CYAN;
		}
		else if((userR_val==0)&&(userG_val==0)&&(userB_val==255))
		{
			color_mode=COLOR_BLUE;
		}
		else if((userR_val==128)&&(userG_val==0)&&(userB_val==255))
		{
			color_mode=COLOR_PURPLE;
		}
	}
	else if(color_mode == COLOR_CHANGE)
	{
		led_rgb_play(); //颜色自加模式
	}
	if((color_mode==COLOR_OFF)&&(color_mode == COLOR_USER))
	{//按键控制灯光时，把颜色值赋给userR_val
		userR_val=curR_val;
		userG_val=curG_val;
		userB_val=curB_val;
	}
	if(color_mode!=COLOR_OFF)
	{
		color_mode_bk=color_mode;	
	}
}


    


