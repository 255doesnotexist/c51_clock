#ifndef GLOBAL_H
#define GLOBAL_H

#include "reg.h" // �����Ĵ�������

// ����ʱ�����
extern unsigned char Hour;
extern unsigned char Minute;
extern unsigned char Second;

// ���������ձ���
extern unsigned short Year;
extern unsigned char Month;
extern unsigned char Day;

// ��������ʱ��
extern unsigned char Alarm_Hour;
extern unsigned char Alarm_Minute;

// �����¶ȱ���
extern unsigned char Temper;

// �����־λ
extern bit Flag_Time_Set;  // ʱ�����ñ�־
extern bit second_1;       // 1HZ�����źű�־
extern bit Alarm_Enable;    // ����ʹ�ܱ�־
extern bit Alarm_Ringing;   // ���������־
extern unsigned char About_Page;   // ����ҳ�ķ�ҳҳ��
extern unsigned char Year_Rolling_Speed;  // ������������ٶȣ�ÿ���ܸ�Speed/100�����֣�
// ����ģʽ
typedef enum {
	MODE_TIME_DISPLAY,
	MODE_JCY_ABOUT,
	MODE_YEAR_SHOW,
	MODE_ALARM_ADJUST,
	MODE_TIME_ADJUST
} ModeType;
extern ModeType Mode;

// ���嵱ǰ������λ
typedef enum {
	ADJUST_SECOND,
	ADJUST_MINUTE,
	ADJUST_HOUR
} AdjustDigitType;
extern AdjustDigitType Adjust_Digit;

// ���嵱ǰ����������λ
typedef enum {
	ADJUST_ALARM_MINUTE,
	ADJUST_ALARM_HOUR
} AdjustAlarmDigitType;
extern AdjustAlarmDigitType Adjust_Alarm_Digit;

// ��ʱ��������
void Delay(unsigned int t);

// ��ʾ��������
void Display();

// ����ɨ�躯������
void Key_Scan();

// DS18B20 ��غ�������
unsigned char DS18B20_Init(void);
void DS18B20_WriteByte(unsigned char dat);
unsigned char DS18B20_ReadByte(void);
void DS18B20_ConvertTemp(void);
int DS18B20_ReadTemp(void);

// ����ˢ������ܵĺ���
void RefreshSEG7(void);

// ���� Timer 0, 1 ��ʼ������
void Timer0_Init(void);
void Timer1_Init(void);
void PauseTimer1Interrupt(void);
void ResumeTimer1Interrupt(void);

#endif
