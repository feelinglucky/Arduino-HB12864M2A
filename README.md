# 海比邻串口液晶屏 HB12864M2A for Arduino

从电子市场获得，当时看其内置中文字库以及指令简单就花了 50 人民币的价格购入（想想现在真的是被坑死了），总体来说是一款性能不错的串口液晶模块。

![LCD Shots](http://pic.yupoo.com/feelinglucky/DYROulD0/medish.jpg)

有关具体此液晶的信息可以参考 ```「海比邻」``` 的官方网站 [http://www.hi-billion.com/](http://www.hi-billion.com/) ，但目前而言我会推荐使用 [Nokia 5110 LCD 模块](https://github.com/feelinglucky/Nokia5110)，性价比更高。


## 示例代码

    // 链接 LCD 设备，其中 pin 为输入口
    lcd_connect(int pin);

    // 关闭 LCD
    lcd_disconnect();

    // 清除 LCD 所有内容（和 Reset 不同）
    lcd_clear();

    // 重置 LCD，最好等待重置完成以后再执行相关操作
    lcd_reset();

    // 背光关闭、开启
    lcd_toggle_backlight();

    // 在 x.y 位置写入字符（LCD 的字体已经配置好，需要更改请直接更改库）
    lcd_draw_string(int x, int y, char *str);

    // 在 x.y 坐标位置画图
    lcd_draw_image(int x, int y, int width, int height, uchar *data);

## 联系

* http://www.gracecode.com/
* lucky[at]gracecode.com

```-- EOF --```

