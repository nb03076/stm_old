/*
 * TLV5638.c
 *
 *  Created on: Aug 12, 2022
 *      Author: kang
 */


#include "TLV5638.h"


#if 0
data frame
R1 SPD PWR R0 data[0:11]

SPD 1:fast 0:slow
PWR 1:off 0:on

R1	R0
0	0	write dac b & buf
0	1	write buf
1	0	write dac a & update dac b with buf content
1	1	write control reg

control frame
D1	D0
0	0	external
0	1	1.024V
1	0	2.048V
1	1	external

A만 할거면 R10
B만 할거면 R00

A,B 둘다 동시에 dac쓸거면
우선 control레지스터로 전압 조정하고
R01(buf)에다가 B값 넣고
R10 A값 쓰고 버퍼에있는 A,B 업데이트


scl max 20mhz

spi 16비트로 설정해서 만듦

깃허브에 있는 라이브러리를 수정 보완해서 만들었음
#endif

SPI_HandleTypeDef* _tlv5638_spi;

void TLV5638_Init(SPI_HandleTypeDef* hspi)
{
	_tlv5638_spi = hspi;
	TLV5638_Control_Reg();
}


void TLV5638_Write_Data(uint16_t Data)
{
	uint8_t tmp[2];
	tmp[0] = Data;
	tmp[1] = (Data>>8);
    TLV5638_CS_Reset;
    if (HAL_SPI_Transmit(_tlv5638_spi, tmp, 1, 1)!=HAL_OK)  {
    	while(1);
    }
    TLV5638_CS_Set;
}

/*************************************
fast && REF=2.048V (0b1101000000000010)
**************************************/

void TLV5638_Control_Reg(void)
{
    TLV5638_Write_Data(0xD002);
}

/*************************************
출력전압 2.048로 설정했다면 전압범위는 0~4.096이 된다
전압 계산식 = 2 * 2.048 * (CODE / 0x1000
**************************************/

void TLV5638_DAC_A_Output_Voltage(float Voltage)
{
    uint16_t CODE = 0x0000;
    TLV5638_Control_Reg();
    CODE = (uint16_t)(Voltage/4.096*0x1000);
    CODE |= 0xC000;
    TLV5638_Write_Data(CODE);
}

void TLV5638_DAC_A_Output(uint16_t dat)
{
    uint16_t CODE = 0x0000;
    TLV5638_Control_Reg();
    CODE = dat;
    CODE |= 0xC000;
    TLV5638_Write_Data(CODE);
}


/*************************************
출력전압 2.048로 설정했다면 전압범위는 0~4.096이 된다
전압 계산식 = 2 * 2.048 * (CODE / 0x1000)
**************************************/

void TLV5638_DAC_B_Output_Voltage(float Voltage)
{
	uint16_t CODE = 0x0000;
    TLV5638_Control_Reg();
    CODE = (uint16_t)(Voltage/4.096*0x1000);
    CODE |= 0x8000;
    TLV5638_Write_Data(CODE);
}

void TLV5638_DAC_B_Output(uint16_t dat)
{
    uint16_t CODE = 0x0000;
    TLV5638_Control_Reg();
    CODE = dat;
    CODE |= 0x8000;
    TLV5638_Write_Data(CODE);
}

/*************************************
전압 A,B 가 최대값 아래로 설정해야할듯? 이건 확인해봐야함
**************************************/

void TLV5638_DAC_A_AND_B_Output_Voltage(float VoltageA, float VoltageB)
{
	uint16_t CODE_A = 0x0000, CODE_B = 0x0000;
    TLV5638_Control_Reg();
    CODE_A = (uint16_t)(VoltageA/4.096*0x1000);
    CODE_A |= 0xC000;
    CODE_B = (uint16_t)(VoltageB/4.096*0x1000);
    CODE_B |= 0x5000;
    TLV5638_Write_Data(CODE_B);
    TLV5638_Write_Data(CODE_A);
}

void TLV5638_DAC_A_AND_B_Output(uint16_t datA, uint16_t datB)
{
	uint16_t CODE_A = 0x0000, CODE_B = 0x0000;
    TLV5638_Control_Reg();
    CODE_A = datA;
    CODE_A |= 0xC000;
    CODE_B = datB;
    CODE_B |= 0x5000;
    TLV5638_Write_Data(CODE_B);
    TLV5638_Write_Data(CODE_A);
}
