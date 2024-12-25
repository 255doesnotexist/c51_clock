#include "ds18b20.h"
#include "reg.h"   // DQ
#include "global.h"

/*
 * �������̣�
 * 1. ��ʼ����ͨ�����͸�λ������ DS18B20 ����ͨ�š�
 * 2. �������
 *    ���� ROM ���� (0xCC)�����������ֻ��һ�� DS18B20 �豸������ʹ�ô����������豸��ַƥ�䡣
 *    ����������磬�����¶�ת�� (0x44) ���ȡ�ݴ��� (0xBE)��
 * 3. ���ݶ�д��
 *    д���ݣ��� DS18B20 д��������Ϣ�����
 *    �����ݣ��� DS18B20 ��ȡ�¶����ݡ�
 * 4. �¶ȶ�ȡ���̣�
 *    a. ��ʼ�� DS18B20��
 *    b. �������� ROM ���
 *    c. ���������¶�ת�����
 *    d. �ȴ�ת����ɡ�
 *    e. �ٴγ�ʼ�� DS18B20��
 *    f. �������� ROM ���
 *    g. ���Ͷ�ȡ�ݴ������
 *    h. ��ȡ�¶����ݵĵ��ֽں͸��ֽڡ�
 *    i. ����ȡ�����ֽ���ϳ�һ�� 16 λ��ֵ��
 *    j. ���� DS18B20 �������ֲᣬ����ֵת��Ϊʵ���¶ȣ����������¶ȣ���
 *
 */

void ds18b20_init(void)
{
	DQ = 1;         // DQ ���øߣ��ͷ�����
	Delay(16);      // ��ʱ����֤�����ȶ�
	DQ = 0;         // ���͸�λ���壬������������ 480us
	Delay(80);      // ��ʱ������ 480us
	DQ = 1;         // ���������ߣ�DS18B20 ���� 15-60us ֮��������Ӧ��
	Delay(16);      // �ȴ�Ӧ���ź�
	Delay(60);      // ��һ����ʱ��ȷ�����Ӧ����
	DQ = 1;         // �ָ����߸ߵ�ƽ״̬
}

void WriteOneChar(unsigned char dat)
{
	unsigned char i = 0;
	DQ = 1;         // ȷ���ڿ�ʼд��ǰ����Ϊ�ߵ�ƽ
	Delay(1);       // ��΢��ʱ
	for (i = 8; i > 0; i--) {
		DQ = 0;     // �������߿�ʼдʱ϶
		DQ = dat & 0x01; // ��Ҫд���λ����� DQ
		Delay(5);   // �������� 60us
		DQ = 1;     // �ͷ����ߣ�����дʱ϶
		dat >>= 1;  // ׼��д����һλ
	}
}

unsigned char ReadOneChar(void)
{
	unsigned char i = 0;
	unsigned char dat = 0;
	DQ = 1;         // ȷ���ڿ�ʼ��ȡǰ����Ϊ�ߵ�ƽ
	Delay(1);       // ��΢��ʱ
	for (i = 8; i > 0; i--) {
		DQ = 0;     // �������ߣ���ʼ��ʱ϶
		dat >>= 1;  // Ϊ��ȡ����λ�ڳ�λ��
		DQ = 1;     // �ͷ����ߣ�DS18B20 ���� 15us �ڽ����ݷŵ�������
		Delay(1);   // ��ʱ����������
		if (DQ)     // ��ȡ����״̬
			dat |= 0x80; // ����Ǹߵ�ƽ�����λΪ 1

		Delay(30);  // �������߸ߵ�ƽֱ����ʱ϶����
		DQ = 1;     // �ָ����߸ߵ�ƽ״̬
	}
	return dat;     // ���ض�ȡ�����ֽ�
}

float ReadTemperature(void)
{
	float temperature = 0.0;
	unsigned int temflag = 0;
	unsigned int tt = 0;
	unsigned char tempL = 0; // �洢�¶ȵ��ֽ�
	unsigned char tempH = 0; // �洢�¶ȸ��ֽ�

	ds18b20_init();    // ��ʼ�� DS18B20
	WriteOneChar(0xcc); // �������� ROM ��������豸��ַ
	WriteOneChar(0x44); // ���������¶�ת������
	Delay(125);       // ��ʱ���ȴ��¶�ת����� (���� 750ms / R resolution)

	ds18b20_init();    // �ٴγ�ʼ�� DS18B20
	WriteOneChar(0xcc); // �ٴη������� ROM ����
	WriteOneChar(0xbe); // ���Ͷ�ȡ�ݴ�������
	tempL = ReadOneChar(); // ��ȡ�¶ȵ��ֽ�
	tempH = ReadOneChar(); // ��ȡ�¶ȸ��ֽ�

	tt = (unsigned int)tempH << 8; // �����ֽ��Ƶ��� 8 λ
	tt |= tempL;                  // �����ֽڼӵ��� 8 λ

	if (tt & 0xf800) { // �ж��¶��Ƿ�Ϊ�� (�� 5 λ��Ϊ 0)
		tt = ~tt + 1; // ȡ����һ�õ�����ֵ
		temflag = 0;  // ���Ϊ���¶�
	} else {
		temflag = 1;  // ���Ϊ���¶�
	}
	temperature = (float)tt;           // �������¶�ֵת��Ϊ������
	temperature = temperature * 0.0625; // ���Էֱ��� (0.0625 ��/LSB)

	if (temflag == 0) {          // ����Ǹ��¶�
		temperature = -temperature; // ȡ���õ�ʵ�ʸ��¶�
	}
	return temperature;             // ���ض�ȡ�����¶�ֵ
}