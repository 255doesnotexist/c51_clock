#ifndef DS18B20_H
#define DS18B20_H

#include <reg51.h>

// DS18B20 ��ʼ��
void ds18b20_init(void);

// �� DS18B20 дһ�ֽ�����
void WriteOneChar(unsigned char dat);

// �� DS18B20 ��һ�ֽ�����
unsigned char ReadOneChar(void);

// �� DS18B20 ��ȡ�¶�ֵ
float ReadTemperature(void);

#endif