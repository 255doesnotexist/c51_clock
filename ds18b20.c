#include "ds18b20.h"
#include "reg.h"   // DQ
#include "global.h"

/*
 * 工作流程：
 * 1. 初始化：通过发送复位脉冲与 DS18B20 建立通信。
 * 2. 发送命令：
 *    跳过 ROM 命令 (0xCC)：如果总线上只有一个 DS18B20 设备，可以使用此命令跳过设备地址匹配。
 *    功能命令：例如，启动温度转换 (0x44) 或读取暂存器 (0xBE)。
 * 3. 数据读写：
 *    写数据：向 DS18B20 写入配置信息或命令。
 *    读数据：从 DS18B20 读取温度数据。
 * 4. 温度读取流程：
 *    a. 初始化 DS18B20。
 *    b. 发送跳过 ROM 命令。
 *    c. 发送启动温度转换命令。
 *    d. 等待转换完成。
 *    e. 再次初始化 DS18B20。
 *    f. 发送跳过 ROM 命令。
 *    g. 发送读取暂存器命令。
 *    h. 读取温度数据的低字节和高字节。
 *    i. 将读取到的字节组合成一个 16 位的值。
 *    j. 根据 DS18B20 的数据手册，将该值转换为实际温度（包括正负温度）。
 *
 */

void ds18b20_init(void)
{
	DQ = 1;         // DQ 先置高，释放总线
	Delay(16);      // 延时，保证总线稳定
	DQ = 0;         // 发送复位脉冲，拉低总线至少 480us
	Delay(80);      // 延时，大于 480us
	DQ = 1;         // 拉高数据线，DS18B20 会在 15-60us 之间拉低以应答
	Delay(16);      // 等待应答信号
	Delay(60);      // 进一步延时，确保完成应答检测
	DQ = 1;         // 恢复总线高电平状态
}

void WriteOneChar(unsigned char dat)
{
	unsigned char i = 0;
	DQ = 1;         // 确保在开始写入前总线为高电平
	Delay(1);       // 稍微延时
	for (i = 8; i > 0; i--) {
		DQ = 0;     // 拉低总线开始写时隙
		DQ = dat & 0x01; // 将要写入的位输出到 DQ
		Delay(5);   // 保持至少 60us
		DQ = 1;     // 释放总线，结束写时隙
		dat >>= 1;  // 准备写入下一位
	}
}

unsigned char ReadOneChar(void)
{
	unsigned char i = 0;
	unsigned char dat = 0;
	DQ = 1;         // 确保在开始读取前总线为高电平
	Delay(1);       // 稍微延时
	for (i = 8; i > 0; i--) {
		DQ = 0;     // 拉低总线，开始读时隙
		dat >>= 1;  // 为读取到的位腾出位置
		DQ = 1;     // 释放总线，DS18B20 会在 15us 内将数据放到总线上
		Delay(1);   // 延时，采样数据
		if (DQ)     // 读取总线状态
			dat |= 0x80; // 如果是高电平，则该位为 1

		Delay(30);  // 保持总线高电平直到读时隙结束
		DQ = 1;     // 恢复总线高电平状态
	}
	return dat;     // 返回读取到的字节
}

float ReadTemperature(void)
{
	float temperature = 0.0;
	unsigned int temflag = 0;
	unsigned int tt = 0;
	unsigned char tempL = 0; // 存储温度低字节
	unsigned char tempH = 0; // 存储温度高字节

	ds18b20_init();    // 初始化 DS18B20
	WriteOneChar(0xcc); // 发送跳过 ROM 命令，忽略设备地址
	WriteOneChar(0x44); // 发送启动温度转换命令
	Delay(125);       // 延时，等待温度转换完成 (至少 750ms / R resolution)

	ds18b20_init();    // 再次初始化 DS18B20
	WriteOneChar(0xcc); // 再次发送跳过 ROM 命令
	WriteOneChar(0xbe); // 发送读取暂存器命令
	tempL = ReadOneChar(); // 读取温度低字节
	tempH = ReadOneChar(); // 读取温度高字节

	tt = (unsigned int)tempH << 8; // 将高字节移到高 8 位
	tt |= tempL;                  // 将低字节加到低 8 位

	if (tt & 0xf800) { // 判断温度是否为负 (高 5 位不为 0)
		tt = ~tt + 1; // 取反加一得到绝对值
		temflag = 0;  // 标记为负温度
	} else {
		temflag = 1;  // 标记为正温度
	}
	temperature = (float)tt;           // 将整型温度值转换为浮点型
	temperature = temperature * 0.0625; // 乘以分辨率 (0.0625 度/LSB)

	if (temflag == 0) {          // 如果是负温度
		temperature = -temperature; // 取反得到实际负温度
	}
	return temperature;             // 返回读取到的温度值
}