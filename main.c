#include "reg.h"
#include "global.h"
#include "ds18b20.h"

// ����ʱ�����
unsigned char Hour = 23;
unsigned char Minute = 59;
unsigned char Second = 50;

// ���������ձ���
unsigned short Year = 2024;
unsigned char Month = 12;
unsigned char Day = 24;

// ��������ʱ��
unsigned char Alarm_Hour = 6;
unsigned char Alarm_Minute = 0;

// �����¶ȱ��� (�����ĳ���ط���ȡ�������ȹ̶�)
unsigned char Temper = 0;

unsigned char DaysOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// �����־λ
bit Flag_Time_Set = 0;  // ʱ�����ñ�־
bit second_1 = 0; 		// 1HZ�����źű�־
bit Alarm_Enable = 1;	// ����ʹ�ܱ�־
bit Alarm_Ringing = 0;   // ���������־

// ����ģʽ
ModeType Mode = MODE_TIME_DISPLAY;

// ���嵱ǰ������λ
AdjustDigitType Adjust_Digit = ADJUST_SECOND;

// ���嵱ǰ����������λ
AdjustAlarmDigitType Adjust_Alarm_Digit = ADJUST_ALARM_MINUTE;

void RefreshSEG7() {
	if(second_1 == 1) // ÿ�����ʱ��
	{
		// �����߼�
		if (Alarm_Enable && Hour == Alarm_Hour && Minute == Alarm_Minute && Second == 0) {
			Alarm_Ringing = 1;
			BUZZER = 1; // ����������
		}
		// ʼ�ո���ʱ�䣬�����ж� Mode
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
	Key_Scan(); 	// ����ɨ��
	Display();  	// ��ʾ
}

void main() {
	// ��ʼ��
	P3 = 0xFF; // ����P3��Ϊ����
	DS18B20_Init();
	Timer0_Init();
	Timer1_Init();
	BUZZER = 0;  // ��ʼ���������ر�
	
	if ((Year % 4 == 0 && Year % 100 != 0) || (Year % 400 ==0)) {
		DaysOfMonth[2] = 29; // �ж�����
	}else {
		DaysOfMonth[2] = 28;
	}

	while (1) {
		ALARM_ENABLED = Alarm_Enable;
		ALARM_RINGING = Alarm_Ringing;
		
		PauseTimer1Interrupt(); // Timer 1 ����ˢ������ܵģ����¶�ǰ��ʱ���жϲ���ˢ�£��������ֻ���ҡ�
		Temper = (int)ReadTemperature(); // ��ȡ�¶�
		ResumeTimer1Interrupt();

		Delay(200);
	}
}
