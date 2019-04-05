#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h"

void turn1();
void turn2();
int turn_small(float num1,float num2);
int turn_controlA(float dis_front,float dis_after);
void turn_controlB(float dis_front,float dis_after);
void turn_temp(void);
void turn_temp1(void);
void turn_temp2(void);
void duoji_motor_analysis(float dis_left,float dis_right);
float	value_abs(float temp);

