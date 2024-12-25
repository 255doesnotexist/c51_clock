#include "reg.h"
#include "global.h"
#include "ds18b20.h"

// 定义时间变量
unsigned char Hour = 23;
unsigned char Minute = 59;
unsigned char Second = 50;

// 定义年月日变量
unsigned short Year = 2024;
unsigned char Month = 12;
unsigned char Day = 24;

// 定义闹钟时间
unsigned char Alarm_Hour = 6;
unsigned char Alarm_Minute = 0;

// 定义温度变量 (假设从某个地方读取，这里先固定)
unsigned char Temper = 0;

unsigned char DaysOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 定义标志位
bit Flag_Time_Set = 0;  // 时间设置标志
bit second_1 = 0; 		// 1HZ方波信号标志
bit Alarm_Enable = 1;	// 闹钟使能标志
bit Alarm_Ringing = 0;   // 闹钟响铃标志

// 定义模式
ModeType Mode = MODE_TIME_DISPLAY;

// 定义当前调整的位
AdjustDigitType Adjust_Digit = ADJUST_SECOND;

// 定义当前调整的闹钟位
AdjustAlarmDigitType Adjust_Alarm_Digit = ADJUST_ALARM_MINUTE;

void RefreshSEG7() {
	if(second_1 == 1) // 每秒更新时间
	{
		// 闹钟逻辑
		if (Alarm_Enable && Hour == Alarm_Hour && Minute == Alarm_Minute && Second == 0) {
			Alarm_Ringing = 1;
			BUZZER = 1; // 启动蜂鸣器
		}
		// 始终更新时间，不再判断 Mode
		if(Flag_Time_Set == 0)
		{
			Second++;
			if (Second >= 60) {
				Second = 0;
				Minute++;
				if (Minute >= 60) {
					Minute = 0;
					Hour++;
					if (Hour >= 24) {
						Hour = 0;
						Day++;
						if (Day > DaysOfMonth[Month]) {
							Day = 1;
							Month++;
							if (Month > 12){
								Month = 1;
								Year++;
							}
						}
					}
				}
			}
		}
		second_1 = 0;
	}
	Key_Scan(); 	// 按键扫描
	Display();  	// 显示
}

void main() {
	// 初始化
	P3 = 0xFF; // 设置P3口为输入
	DS18B20_Init();
	Timer0_Init();
	Timer1_Init();
	BUZZER = 0;  // 初始化蜂鸣器关闭
	
	if ((Year % 4 == 0 && Year % 100 != 0) || (Year % 400 ==0)) {
		DaysOfMonth[2] = 29; // 判定闰年
	}else {
		DaysOfMonth[2] = 28;
	}

	while (1) {
		ALARM_ENABLED = Alarm_Enable;
		ALARM_RINGING = Alarm_Ringing;
		
		PauseTimer1Interrupt(); // Timer 1 用来刷新数码管的，改温度前暂时关中断不让刷新，否则数字会很乱。
		Temper = (int)ReadTemperature(); // 读取温度
		ResumeTimer1Interrupt();

		Delay(200);
	}
}
