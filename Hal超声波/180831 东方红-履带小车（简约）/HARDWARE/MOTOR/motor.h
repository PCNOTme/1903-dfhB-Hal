#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h"

void motor1(int left_pwm,int right_pwm) ;
void Motor_trig_Init(void);
void Chasu_Analysis1(void);
void Chasu_Analysis2(void);
int PID_Echo_PI (int length,int echo_Target);
void PID_Analysis(int Velocity);
void Motor_pwm(void);
void Xianfu_Pwm(void);
void Set_Pwm(int motor_a,int motor_b);

