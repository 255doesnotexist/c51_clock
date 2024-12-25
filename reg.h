#ifndef REG_H
#define REG_H

#include <reg51.h>

// ��������ܶ��룬���ڿ����������ʾ����
extern unsigned char code SegCode[];

// ���������λ�룬����ѡ��ǰҪ��ʾ�������
extern unsigned char code BitCode[];

// ���尴��״̬
sbit Key_Current_Add = P3^4; // P3.4����ǰλ���� ��
sbit Key_Switch_Digit = P3^5; // P3.5���л���ǰλ ��
sbit Key_Switch_Mode = P3^3;  // P3.3���л�ģʽ ��
sbit Key_Alarm_Cancel = P3^6; // P3.6������ȡ�� ��

sbit BUZZER = P1^7; // ���������ӵ� P1.7
sbit ALARM_ENABLED = P1^0; // ����ʹ��״ָ̬ʾ�����ӵ� P1.0
sbit ALARM_RINGING = P1^1; // ������������״ָ̬ʾ�����ӵ� P1.1

sbit DQ = P3^2; // DS18B20 �� DQ ���ݶ����ӵ� P3.2
#endif
