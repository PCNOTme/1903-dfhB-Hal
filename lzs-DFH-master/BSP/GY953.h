#ifndef __GY953_H
#define __GY953_H
#include "stm32f4xx_hal.h"

#define Go_straight 0
#define Turn_right_90 1
#define Turn_left_90  2
#define Back  3


typedef volatile struct __GY953
{
	float ROLL;

	float PITCH;

	float YAW;

}gy953;


typedef volatile struct __angle
{
	int time;    //陀螺仪实时值
	int car;     //车身偏移值
	int front;
	int right;
	int left;
	int back;
	int flag;    //转向标志位
}angle;


void GY953_Init(void);
void Angle_Init(void);
int16_t GY953_GetDate(void);
void Angle_change(void);

#endif




