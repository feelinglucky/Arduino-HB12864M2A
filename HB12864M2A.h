/****************海比邻液晶串口模式****************
作者：谭涛 武汉大学 				版本：1.1
时间：2008-9-29						功能：定义常用命令及函数
开发工具：keilcV8.05a
***************海比邻液晶串口模式*****************/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifndef HB12862M2A
#define HB12862M2A 1

#define uint  unsigned int
#define uchar unsigned char

// 分辨率（点）
#define MAX_VERT_DOTS 64
#define MAX_HORI_DOTS 128

//初始化指令
/****************字符显示命名，用于单个字符的显示*****************/
//在相应字符位处显示字符命令 命令格式：命令 行位置(0-7) 列位置(0-F) 内容(2字节或1字节)
#define CHAR				0xF0		//显示16*16点阵国标汉字
#define ASC2_8X8		    0xF1		//显示8*8点阵标准ASC2字符
#define ASC2_8X16		    0xF9		//显示8*16点阵半角标准ASC2字符
#define NUM_3X5			    0xE1		//显示3*5点阵数字及大写字母
#define ASC2_5X7		    0xE0		//显示5*7点阵标准ASC2字符
#define NUM_6X12			0xE2		//显示6*12双倍粗细数字
#define NUM_9X16			0xE3		//显示9*16三倍粗细数字

//在任意处显示字符命令      命令格式：命令 行点位置(0-128) 列点位置(0-240) 内容(2字节或1字节)注意不可超过最大
#define CHAR_LAT			0xEA		//显示16*16点阵国标汉字
#define ASC2_8X8_LAT		0xC1		//显示8*8点阵标准ASC2字符
#define ASC2_8X16_LAT		0xC9		//显示8*16点阵半角标准ASC2字符
#define NOM_3X5_LAT			0xC4		//显示3*5点阵数字及大写字母
#define ASC2_5X7_LAT		0xC0		//显示5*7点阵标准ASC2字符
#define NOM_6X12_LAT		0xC2		//显示6*12双倍粗细数字
#define NOM_9X16_LAT		0xC3		//显示9*16三倍粗细数字

//在光标处处显示字符命令 命令格式：命令 内容。使用前先定义光标位置
#define CHAR_CUR			0xEC		//显示16*16点阵国标汉字
#define ASC2_8X8_CUR		0xD1		//显示8*8点阵标准ASC2字符
#define ASC2_8X16_CUR		0xD9		//显示8*16点阵半角标准ASC2字符
#define NUM_3X15_CUR		0xD4		//显示3*5点阵数字及大写字母
#define ASC2_5X7_CUR		0xD0		//显示5*7点阵标准ASC2字符
#define NUM_6X12_CUR		0xD2		//显示6*12双倍粗细数字
#define NUM_9X16_CUR		0xD3		//显示9*16三倍粗细数字

/**************字符串显示命名，用于多个字符的显示*****************///lat表示任意位置，cur表示光标处
//命令格式：命令 行位置   列位置   内容 结束符号0x00
//命令格式：命令 行点位置 列点位置 内容 结束符号0x00
//命令格式：命令 内容 结束符号0x00。记得定义光标位置
#define LCD_MODE_STR_CHAR			0xE9		//混合显示16*16点阵和8*16点阵半角标准ASC2字符串,字符位处
#define LCD_MODE_STR_CHAR_LAT		0xEB		//混合显示16*16点阵和8*16点阵半角标准ASC2字符串，光标处
#define LCD_MODE_STR_ASC2_8X8		0xDF		//显示8*8点阵标准ASC2字符串
#define LCD_MODE_STR_NUM_3X15		0xDE		//显示3*5点阵数字及大写字母字符串
#define LCD_MODE_STR_ASC2_5X7		0xDB		//显示5*7点阵ASC2字符串
#define LCD_MODE_STR_NUM_6X12		0xDC		//显示6*12点阵双倍粗细数字串
#define LCD_MODE_STR_NUM_9X16		0xDD		//显示9*16点阵三倍粗细数字串

#define LCD_MODE_STR_CHAR_CUR		0xED		//混合显示16*16点阵和8*16点阵半角标准ASC2字符串，光标处
#define LCD_MODE_STR_ASC2_8X8_CUR	0xDA		//显示8*8点阵标准ASC2字符串
#define LCD_MODE_STR_NUM_3X15_CUR	0xD6		//显示3*5点阵数字及大写字母字符串
#define LCD_MODE_STR_ASC2_5X7_CUR	0xD5		//显示5*7点阵ASC2字符串
#define LCD_MODE_STR_NUM_6X12_CUR	0xD7		//显示6*12点阵双倍粗细数字串
#define LCD_MODE_STR_NUM_9X16_CUR	0xD8		//显示9*16点阵三倍粗细数字串
/*240128针脚定义*/ 

/****************图形显示命名，用于图形的显示*****************/
//在相应字符位处显示字符命令 命令格式：命令 行位置(0-7) 列位置(0-F) 内容(2字节或1字节)
#define LCD_MODE_BIT_LAT			0xF2		//显示位点阵
#define LCD_MODE_CHAR_LAT		    0xF3		//显示字节点阵
#define LCD_MODE_LINE		    	0xC5		//画线
#define LCD_MODE_BMP			    0xFD		//画图片			  

/****************屏幕控制命令，用于图形的控制*****************/
#define LCD_CMD_SCREEN_CLS			0xF4		//全屏幕清除
#define LCD_CMD_SCREEN_UP			0xF5		//全屏幕上移动
#define LCD_CMD_SCREEN_DOWN		    0xF6		//全屏幕下移动
#define LCD_CMD_SCREEN_LEFT		    0xF7		//全屏幕左移动
#define LCD_CMD_SCREEN_RIGHT		0xF8		//全屏幕右移动
#define LCD_CMD_INVERSE				0xFA		//反白
#define LCD_CMD_CUR_SET				0xFB		//光标显示设置
#define LCD_CMD_RATE_SET			0xFC		//移动速度调整
#define LCD_CMD_SCREEN_ON			0xFE		//屏幕开关及显示模式
#define LCD_CMD_CUR_LOC_SET			0xE4		//光标定位设置
#define LCD_CMD_LIGHT_SET			0xE5		//背光开关
#define LCD_CMD_CONTRAST_SET		0xE6		//对比度调节

/****************模块设置命令，用于模块的控制*****************/
#define LCD_CMD_POWER_DOWN			0xFF		//省电模式
#define LCD_CMD_RESET				0xEF		//复位命令
#define LCD_CMD_BAND_SET			0xE8		//UART波特率设置

/*********************块状图形操作命令************************/
#define LCD_CMD_BLOCK_MOVE			0xC6		//任意指定区域移位
#define LCD_CMD_BLOCK_OPRT			0xC7		//任意指定区域操作
#define LCD_CMD_BLOCK_RAY			0xC8		//任意指定区域闪烁

#ifdef _OPEN_WRT_
    #define DEFAULT_LED_PORT "/dev/ttyS0"
#endif

// 默认端口
#ifndef DEFAULT_LED_PORT
    #define DEFAULT_LED_PORT "/dev/ttyUSB0"
#endif

#define FALSE  -1
#define TRUE   0

int lcd_open_port(int pin);
void lcd_connect(int pin);
void lcd_disconnect();
void lcd_delay();
int lcd_send_byte(uchar byte);
int lcd_send_data(uchar *data, int len);
void lcd_screen_con(uchar commands);
void lcd_clear();
void lcd_reset();
void lcd_toggle_backlight();
void lcd_set_backlight(int open);
void lcd_draw_string(int x, int y, char *str);
void lcd_draw_image(int x, int y, int width, int height, uchar *data);

#endif
