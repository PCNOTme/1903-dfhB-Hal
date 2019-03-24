#ifndef __ENCODER_H
#define __ENCODER_H


typedef volatile struct __Encoder
{
	int encoder_LU;

	int encoder_RU;

	int encoder_LD;

	int encoder_RD;

}Encoder;

int Read_Encoder(int TIMX);

void Encoder_Init(void);

void Encoder_Update(Encoder * enco);



#endif 
