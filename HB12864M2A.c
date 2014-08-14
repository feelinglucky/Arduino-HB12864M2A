/**
 * HB12864M2A Operating Lib
 *
 * @author  ohdarling88 <ohdarling88@gmail.com>
 * @date    2011.07.19
 *
 * @author feelinglucky<lucky@gracecode.com>
 * @date   2012-05-21
 */

#include <SoftwareSerial.h>
#include "HB12864M2A.h"

//int lcd_file_port = -1;
struct termios options;

/*
void set_termios(struct termios *pNewtio, unsigned short uBaudRate)
{
    bzero(pNewtio,sizeof(struct termios));
    pNewtio->c_cflag = uBaudRate|CS8|CREAD|CLOCAL;
    pNewtio->c_iflag = IXOFF;
    pNewtio->c_oflag = 0;
    pNewtio->c_lflag = 0;
    pNewtio->c_cc[VINTR] = 0;
    pNewtio->c_cc[VQUIT] = 0;
    pNewtio->c_cc[VERASE] = 0;
    pNewtio->c_cc[VKILL] = 0;
    pNewtio->c_cc[VEOF] = 0;
    pNewtio->c_cc[VTIME] = 0;
    pNewtio->c_cc[VMIN] = 0;
    //pNewtio->c_cc[VSWTC] = 0;
    pNewtio->c_cc[VSTART] = 0;
    pNewtio->c_cc[VSTOP] = 0;
    pNewtio->c_cc[VSUSP] = 0;
    pNewtio->c_cc[VEOL] = 0;
    pNewtio->c_cc[VREPRINT] = 0;
    pNewtio->c_cc[VDISCARD] = 0;
    pNewtio->c_cc[VWERASE] = 0;
    pNewtio->c_cc[VLNEXT] = 0;
    pNewtio->c_cc[VEOL2] = 0;
}
*/


int lcd_open_port(int pin) {
    /*
    printf("Open Port: %s.\n", port_file);
    lcd_file_port = open(port_file, O_RDWR | O_NOCTTY | O_NDELAY);
    if (lcd_file_port == -1) {
        perror("lcd_open_port: Open Port Error!");
    } else {
        fcntl(lcd_file_port, F_SETFL, FNDELAY);
    }
        
    return lcd_file_port;

    */
}


void lcd_connect(int pin) {
    pinMode(pin, OUTPUT);
    begin(9600);
    delay(4);
}


void lcd_disconnect() {
    /*
    if (lcd_file_port > 0) {
        close(lcd_file_port);
    }
    lcd_file_port = -1;
    */
}


/**
 * 等待数据和液晶处理
 */
void lcd_delay() {
    usleep(100000);
}


/**
 * 向端口发送个字节
 */
int lcd_send_byte(uchar byte)
{
    lcd_connect();
    uchar buf[1];
    buf[0] = byte;
    write(buf);
    //return write(lcd_file_port, buf, 1);
}


/**
 * 向端口发送字符
 */
int lcd_send_data(uchar *data, int len) {
    write(data);
    //return write(data, len);
}


/**
 * 屏幕控制命令
 */
void lcd_screen_con(uchar commands) {
	unsigned char cmd[] = { commands, 0xBB, 0xBB, 0xBB };
	lcd_send_data(cmd, sizeof(cmd));
	lcd_delay();
}


/**
 * 清除屏幕
 */
void lcd_clear() {
    lcd_screen_con(LCD_CMD_SCREEN_CLS);
}


/**
 * 重置液晶屏
 */
void lcd_reset() {
    lcd_screen_con(LCD_CMD_RESET);
	sleep(1); // 等待一秒重置完成
}


/**
 * 打开、关闭背光
 */
void lcd_toggle_backlight() {
    lcd_screen_con(LCD_CMD_LIGHT_SET);
    lcd_delay();
}


/**
 * 设置背光
 */
void lcd_set_backlight(int open) {
	unsigned char cmd[] = { LCD_CMD_LIGHT_SET, 0x00 };
	cmd[1] = open == 0 ? 0 : 1;
	lcd_send_data(cmd, 2);
	lcd_delay();
}


/**
 * 向 LCD 打印字符串
 */
void lcd_draw_string(int x, int y, char *str) {
    printf("lcd: %s\n", str);

	unsigned char cmd[3] = { LCD_MODE_STR_ASC2_8X8, 0, 0 };
	cmd[1] = x;
	cmd[2] = y;

	lcd_send_data(cmd, sizeof(cmd));
	lcd_send_data((uchar *)str, strlen(str));
	lcd_send_byte('\0');
	lcd_delay();
}


/**
 * 画图片
 */
void lcd_draw_image(int x, int y, int width, int height, uchar *data) {
    uchar cmd[] = { LCD_MODE_BMP, 0, 0, 0, 0 };
    int width_bytes = (width + 7) / 8;
    cmd[1] = y;
    cmd[2] = x;
    cmd[3] = width_bytes;
    cmd[4] = height;
    lcd_send_data(cmd, sizeof(cmd));
    lcd_send_data(data, width_bytes * height);
	lcd_delay();
}

