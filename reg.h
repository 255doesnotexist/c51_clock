#ifndef REG_H
#define REG_H

#include <reg51.h>

// 定义数码管段码，用于控制数码管显示数字
extern unsigned char code SegCode[];

// 定义数码管位码，用于选择当前要显示的数码管
extern unsigned char code BitCode[];

// 定义按键状态
sbit Key_Current_Add = P3^4; // P3.4，当前位增加 键
sbit Key_Switch_Digit = P3^5; // P3.5，切换当前位 键
sbit Key_Switch_Mode = P3^3;  // P3.3，切换模式 键
sbit Key_Alarm_Cancel = P3^6; // P3.6，闹钟取消 键

sbit BUZZER = P1^7; // 蜂鸣器连接到 P1.7
sbit ALARM_ENABLED = P1^0; // 闹铃使能状态指示灯连接到 P1.0
sbit ALARM_RINGING = P1^1; // 闹铃正在响铃状态指示灯连接到 P1.1

sbit DQ = P3^2; // DS18B20 的 DQ 数据端连接到 P3.2
#endif
