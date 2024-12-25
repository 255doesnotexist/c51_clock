#include "global.h"
#include "reg.h"

// ��������ܶ��룬���ڿ����������ʾ����
unsigned char code SegCode[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40
}; // 0-9, Ϩ��, ����

// ���������λ�룬����ѡ��ǰҪ��ʾ�������
unsigned char code BitCode[] = {
	0xFE, // P2.0�����Ƶ�һ�������
	0xFD, // P2.1�����Ƶڶ��������
	0xFB, // P2.2�����Ƶ����������
	0xF7, // P2.3�����Ƶ��ĸ������
	0xEF, // P2.4�����Ƶ���������
	0xDF, // P2.5�����Ƶ����������
	0xBF, // P2.6�����Ƶ��߸������
	0x7F  // P2.7�����Ƶڰ˸������
};

unsigned char loading_SegCode[] = {
	0x01, // a
	0x02, // b
	0x04, // c
	0x08, // d
	0x10, // e
	0x20, // f
	0x40  // g
};

unsigned char by_jcy_SegCode[] = {
	0x7C, // ��ĸ b
	0x6E, // ��ĸ y
	0x00, // �ո�
	0x0E, // ��ĸ J
	0x39, // ��ĸ C
	0x6E  // ��ĸ y
};

unsigned char ID[] = {
	2, 0, 2, 2, 1, 1, 2, 0, 0, 2, 2, 7
};

unsigned char About_Page = 1;

unsigned char LoadingSegCode[] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20
};
unsigned char Loading_Page = 0, Loading_Count = 0;

unsigned char Year_Offset = 0;

unsigned char Year_Rolling_Speed = 25; // ������������ٶȣ�ÿ���ܸ�Speed/100�����֣�

// ��ʱ����
void Delay(unsigned int t) {
	while (t--);
}

// ��ʾ����
void Display() {
	unsigned char i;
	unsigned char Time_Buf[6];
	unsigned char Temper_Buf[2];
	unsigned char Alarm_Buf[4];
	unsigned char Year_Buf[10];
	static unsigned char blink_count = 0;

	blink_count++;
	if (blink_count >= 100) blink_count = 0; // Reset blink counter

	switch (Mode) {
		case MODE_TIME_DISPLAY:
			Time_Buf[0] = Hour / 10;
			Time_Buf[1] = Hour % 10;
			Time_Buf[2] = Minute / 10;
			Time_Buf[3] = Minute % 10;
			Time_Buf[4] = Second / 10;
			Time_Buf[5] = Second % 10;

			for (i = 0; i < 6; i++) {
				P0 = SegCode[Time_Buf[i]];
				P2 = BitCode[i];
				Delay(100);
				P2 = 0xFF;
			}

			if (Temper != 85) {
				Temper_Buf[0] = (Temper / 10) % 10;
				Temper_Buf[1] = (Temper % 10) % 10;
				P0 = SegCode[Temper_Buf[0]];
				P2 = BitCode[6];
				Delay(100);
				P2 = 0xFF;
				P0 = SegCode[Temper_Buf[1]];
				P2 = BitCode[7];
				Delay(100);
				P2 = 0xFF;
			} else {
				P0 = LoadingSegCode[Loading_Page % 8];
				P2 = BitCode[6];
				Delay(100);
				P0 = LoadingSegCode[Loading_Page % 8];
				P2 = BitCode[7];
				Delay(100);
				// ���� P2 Ӫ�����ᶯ����
				
				++Loading_Count;
				if (Loading_Count >= 5) {
					Loading_Page++;
					Loading_Page == 12 ? (Loading_Page = 1) : 0;
					Loading_Count = 0;
				}
			}
			
			// ���� DP
			if (Mode == MODE_TIME_DISPLAY) {
				P0 = 0x80; // DP λΪ 1������λΪ 0
				P2 = BitCode[1] & BitCode[3]; //& BitCode[5]; // �ڵ�����λ���� DP
				Delay(100);
				P2 = 0xFF;
			}
			
			break;

		case MODE_ALARM_ADJUST:
			Alarm_Buf[0] = Alarm_Hour / 10;
			Alarm_Buf[1] = Alarm_Hour % 10;
			Alarm_Buf[2] = Alarm_Minute / 10;
			Alarm_Buf[3] = Alarm_Minute % 10;

			for (i = 0; i < 4; i++) {
				if (blink_count < 50) { // ������˸�ٶ�
					if (!((Adjust_Alarm_Digit == ADJUST_ALARM_HOUR && (i == 0 || i == 1)) ||
  						(Adjust_Alarm_Digit == ADJUST_ALARM_MINUTE && (i == 2 || i == 3))))
					{
						P0 = SegCode[Alarm_Buf[i]];
					} else {
						P0 = SegCode[10]; // Ϩ��
					}
				} else {
					P0 = SegCode[Alarm_Buf[i]];
				}
				P2 = BitCode[i];
				Delay(100);
				P2 = 0xFF;
			}
			
			// �ҷ����õ��ŵ� N(���ŵ�U)��ʾû����ȥ�İ�ť n��ʾ����ȥ�İ�ť������
			// �����Ҵ����������λ�� ��ʾ����ʹ��״̬������״̬ N��ʾ0 n��ʾ1
			#define OFF_SEG (0x37)
			#define ON_SEG (0x54)
			P0 = Alarm_Enable ? ON_SEG : OFF_SEG;
			P2 = BitCode[4];
			Delay(100);
			P2 = 0xFF;
			P0 = Alarm_Ringing ? ON_SEG : OFF_SEG;
			P2 = BitCode[5];
			Delay(100);
			#undef OFF_SEG
			#undef ON_SEG
			P2 = 0xFF;
			// ���� DP
			if (Mode == MODE_ALARM_ADJUST) {
				P0 = 0x80; // DP λΪ 1������λΪ 0
				P2 = BitCode[Adjust_Alarm_Digit == ADJUST_ALARM_HOUR ? 1 : 3]; // �ڵ�����λ���� DP
				Delay(100);
				P2 = 0xFF;
			}
			break;

		case MODE_TIME_ADJUST:
			Time_Buf[0] = Hour / 10;
			Time_Buf[1] = Hour % 10;
			Time_Buf[2] = Minute / 10;
			Time_Buf[3] = Minute % 10;
			Time_Buf[4] = Second / 10;
			Time_Buf[5] = Second % 10;

			for (i = 0; i < 6; i++) {
				if (blink_count < 50) { // ������˸�ٶ�
					if (!((Adjust_Digit == ADJUST_HOUR && (i == 0 || i == 1)) ||
						(Adjust_Digit == ADJUST_MINUTE && (i == 2 || i == 3)) ||
						(Adjust_Digit == ADJUST_SECOND && (i == 4 || i == 5))))
					{
						P0 = SegCode[Time_Buf[i]];
					} else {
						P0 = SegCode[10]; // Ϩ��
					}
				} else {
					P0 = SegCode[Time_Buf[i]];
				}
				P2 = BitCode[i];
				Delay(100);
				P2 = 0xFF;
			}
			// ���� DP
			if (Mode == MODE_TIME_ADJUST) {
				P0 = 0x80; // DP λΪ 1������λΪ 0
				P2 = BitCode[Adjust_Digit == ADJUST_HOUR ? 1 : (Adjust_Digit == ADJUST_MINUTE ? 3 : 5)]; // �ڵ�����λ���� DP
				Delay(100);
				P2 = 0xFF;
			}
			break;

		case MODE_JCY_ABOUT:
			if (About_Page == 1) {
				for (i = 0; i < 6; i++) {
					P0 = by_jcy_SegCode[i];
					P2 = BitCode[i];
					Delay(100);
					P2 = 0xFF;
				}
			} else if (About_Page == 2) {
				for (i = 0; i < 6; i++) {
					P0 = SegCode[ID[i]];
					P2 = BitCode[i];
					Delay(100);
					P2 = 0xFF;
				}
			} else if (About_Page == 3) {
				for (i = 6; i < 12; i++) {
					P0 = SegCode[ID[i]];
					P2 = BitCode[i - 6];
					Delay(100);
					P2 = 0xFF;
				}
			} else {
				for (i = 0; i < 8; i++) {
					P0 = LoadingSegCode[(Loading_Page + i) % 8]; // ÿһλ�ļ��ؽ���������һ���ٺ�
					P2 = BitCode[i];
					Delay(100);
					// ���� P2 Ӫ�����ᶯ����
				}
				
				++Loading_Count;
				if (Loading_Count >= 5) {
					Loading_Page++;
					Loading_Page == 12 ? (Loading_Page = 1) : 0;
					Loading_Count = 0;
				}
			}
		break;
			
		case MODE_YEAR_SHOW:
			if(blink_count % Year_Rolling_Speed == 0) { // ÿ������ĸ����֣���Ϊ 10ms ˢ��һ�Σ�ÿ��ˢ�� 100 �Σ�ÿ 25 ��ˢ������һλ���� 
				Year_Offset++;
				Year_Offset %= 24;
			}
			
			Year_Buf[0] = (Year / 1000) % 10;
			Year_Buf[1] = (Year / 100) % 10;
			Year_Buf[2] = (Year / 10) % 10;
			Year_Buf[3] = (Year / 1) % 10;
			Year_Buf[4] = 11;
			Year_Buf[5] = (Month / 10) % 10;
			Year_Buf[6] = (Month / 1) % 10;
			Year_Buf[7] = 11; // SegCode 11 �Ǻ������ּ���
			Year_Buf[8] = (Day / 10) % 10;
			Year_Buf[9] = (Day / 1) % 10;
			Year_Buf[10] = 10;
			Year_Buf[11] = 10; // �����ո��������Ʒ�ֹ��һ��
			
			for(i = 0; i < 6; i++) {
				P0 = SegCode[Year_Buf[(i + Year_Offset) % 12]];
				P2 = BitCode[i];
				Delay(100);
				P2 = 0xFF;
			}
	}
}
