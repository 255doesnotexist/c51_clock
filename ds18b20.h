#ifndef DS18B20_H
#define DS18B20_H

#include <reg51.h>

// DS18B20 初始化
void ds18b20_init(void);

// 向 DS18B20 写一字节数据
void WriteOneChar(unsigned char dat);

// 从 DS18B20 读一字节数据
unsigned char ReadOneChar(void);

// 从 DS18B20 读取温度值
float ReadTemperature(void);

#endif