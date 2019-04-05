#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "DATA.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"

u8 Val1[3]={0x00};
u8 Val2[3]={0x00};
u8 Val3[3]={0x00};
void DATA_Processing(void)
{
     unsigned char more;
	   unsigned long sensor_timestamp;
	   short gyro[3], accel[3], sensors;//陀螺仪存放数组，加速度存放数组，返回状态量
	   long quat[4];//四元数存放数组
	   float roll,yaw,pitch;
	   int p,r,y;
	   //u8 i=1;
	   Val1[2]=1;
	   Val2[2]=2;
	   Val3[2]=3;    	  			
	   dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);
     if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		 {
		      if(pitch<0) {pitch=pitch*(-1);Val1[1]=0;} else Val1[1]=1;
			    if(roll<0)  {roll=roll*(-1);Val2[1]=0;} else Val2[1]=1;
			    if(yaw<0)   {yaw=yaw*(-1);Val3[1]=0;} else Val3[1]=1;
			    p=(int)pitch;
					Val1[0]=p;
			    r=(int)roll;
					Val2[0]=r;
			    y=(int)yaw;
					Val3[0]=y;
			
		 }     	
}

