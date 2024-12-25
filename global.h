#ifndef GLOBAL_H
#define GLOBAL_H

#include "reg.h" // 包含寄存器定义

// 定义时间变量
extern unsigned char Hour;
extern unsigned char Minute;
extern unsigned char Second;

// 定义年月日变量
extern unsigned short Year;
extern unsigned char Month;
extern unsigned char Day;

// 定义闹钟时间
extern unsigned char Alarm_Hour;
extern unsigned char Alarm_Minute;

// 定义温度变量
extern unsigned char Temper;

// 定义标志位
extern bit Flag_Time_Set;  // 时间设置标志
extern bit second_1;       // 1HZ方波信号标志
extern bit Alarm_Enable;    // 闹钟使能标志
extern bit Alarm_Ringing;   // 闹钟响铃标志
extern unsigned char About_Page;   // 署名页的翻页页号
extern unsigned char Year_Rolling_Speed;  // 年月日跑马灯速度（每秒跑个Speed/100个数字）
// 定义模式
typedef enum {
	MODE_TIME_DISPLAY,
	MODE_JCY_ABOUT,
	MODE_YEAR_SHOW,
	MODE_ALARM_ADJUST,
	MODE_TIME_ADJUST
} ModeType;
extern ModeType Mode;

// 定义当前调整的位
typedef enum {
	ADJUST_SECOND,
	ADJUST_MINUTE,
	ADJUST_HOUR
} AdjustDigitType;
extern AdjustDigitType Adjust_Digit;

// 定义当前调整的闹钟位
typedef enum {
	ADJUST_ALARM_MINUTE,
	ADJUST_ALARM_HOUR
} AdjustAlarmDigitType;
extern AdjustAlarmDigitType Adjust_Alarm_Digit;

// 延时函数声明
void Delay(unsigned int t);

// 显示函数声明
void Display();

// 按键扫描函数声明
void Key_Scan();

// DS18B20 相关函数声明
unsigned char DS18B20_Init(void);
void DS18B20_WriteByte(unsigned char dat);
unsigned char DS18B20_ReadByte(void);
void DS18B20_ConvertTemp(void);
int DS18B20_ReadTemp(void);

// 声明刷新数码管的函数
void RefreshSEG7(void);

// 声明 Timer 0, 1 初始化函数
void Timer0_Init(void);
void Timer1_Init(void);
void PauseTimer1Interrupt(void);
void ResumeTimer1Interrupt(void);

#endif
