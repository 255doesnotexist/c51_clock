#include "global.h"
#include "reg.h"

// 按键扫描函数
void Key_Scan() {
	static unsigned char key_down[] = {1, 1, 1, 1}; // 1: 释放, 0: 按下

	// 当前位增加键
	if (Key_Current_Add == 0) {
		Delay(10); // 简单消抖
		if (Key_Current_Add == 0 && key_down[0] == 1) {
			key_down[0] = 0;
			if (Mode == MODE_TIME_ADJUST) {
				switch (Adjust_Digit) {
					case ADJUST_HOUR:
						Hour++;
						if (Hour > 23) Hour = 0;
						break;
					case ADJUST_MINUTE:
						Minute++;
						if (Minute > 59) Minute = 0;
						break;
					case ADJUST_SECOND:
						if (Second > 30) {
							Second = 0;
							Minute++;
							if (Minute > 59) {
								Minute = 0;
								Hour++;
								if (Hour > 23) Hour = 0;
							}
						} else {
							Second = 0;
						}
						break;
				}
			} else if (Mode == MODE_ALARM_ADJUST) {
				switch (Adjust_Alarm_Digit) {
					case ADJUST_ALARM_HOUR:
						Alarm_Hour++;
						if (Alarm_Hour > 23) Alarm_Hour = 0;
						break;
					case ADJUST_ALARM_MINUTE:
						Alarm_Minute++;
						if (Alarm_Minute > 59) Alarm_Minute = 0;
						break;
				}
			} else if (Mode == MODE_JCY_ABOUT) {
				About_Page++; 
				About_Page == 5 ? (About_Page = 1) : (0);
			} else if (Mode == MODE_YEAR_SHOW) {
				Year_Rolling_Speed /= 2; // 三档可调，50 (25 默认) 12
				if(Year_Rolling_Speed < 10) Year_Rolling_Speed = 50;
			}
		}
	} else {
		key_down[0] = 1;
	}

	// 切换当前位键
	if (Key_Switch_Digit == 0) {
		Delay(10); // 简单消抖
		if (Key_Switch_Digit == 0 && key_down[1] == 1) {
			key_down[1] = 0;
			if (Mode == MODE_TIME_ADJUST) {
				Adjust_Digit++;
				if (Adjust_Digit > ADJUST_HOUR) Adjust_Digit = ADJUST_SECOND;
			} else if (Mode == MODE_ALARM_ADJUST) {
				Adjust_Alarm_Digit++;
				if (Adjust_Alarm_Digit > ADJUST_ALARM_HOUR) Adjust_Alarm_Digit = ADJUST_ALARM_MINUTE;
			}
		}
	} else {
		key_down[1] = 1;
	}

	// 切换模式键
	if (Key_Switch_Mode == 0) {
		Delay(10); // 简单消抖
		if (Key_Switch_Mode == 0 && key_down[2] == 1) {
			key_down[2] = 0;
			Mode++;
			if (Mode > MODE_TIME_ADJUST) Mode = MODE_TIME_DISPLAY;
		}
	} else {
		key_down[2] = 1;
	}

	// 闹钟取消键/闹铃开关键（如果不在响铃且在闹铃调整模式）
	if (Key_Alarm_Cancel == 0) {
		Delay(10); // 简单消抖
		if (Key_Alarm_Cancel == 0 && key_down[3] == 1) {
			key_down[3] = 0;
			if (Alarm_Ringing) {
				Alarm_Ringing = 0;
				BUZZER = 0; // 关闭蜂鸣器
			} else if (Mode == MODE_ALARM_ADJUST) {
				Alarm_Enable = ~Alarm_Enable; // 按下取消键可以切换闹钟使能状态
			}
		}
	} else {
		key_down[3] = 1;
	}
}
