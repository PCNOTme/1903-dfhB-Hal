#include "sys.h"
#include "delay.h"

void Echo_pid1(float dis_front,float dis_after);
void Echo_pid2(float target_distance);
void Echo_pid3(float dis_front,float dis_after);
void Echo_pwm_change(int change);
int Motor1_pid(int encoder_left,int target);
int Motor2_pid(int encoder_righ,int target);
void Motor_pwm_change(int change);
void xianfu_pwm(int pwm);
int PI_A (int Encoder,int Target);
int PI_B (int Encoder,int Target);

