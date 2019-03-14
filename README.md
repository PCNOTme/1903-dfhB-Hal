# 1903-dfhB-Hal
a contesr that i want to improve myself and prove myself.
##################################
This is my first time to use this guy , this is incredible.



东方红B类电控履带底盘
主控：STM32F407vet6

外设：超声波*4  …… 6轴陀螺仪*1……电机驱动模块*2
……降压模块（12-5v）……船型开关*1……拨键开关*1

功能：4路超声波导航+陀螺仪角度+PID闭环控制。

  设计框架：
1.实现超声波测距
2.路径规划问题（转弯）
3.走直线问题（速度闭环）
4.姿态问题（陀螺仪环/超声波环）
  设计步骤：
1.串口读写（usart.c）
2.超声波串口读写（hc-sr04）
3.陀螺仪读数据（mpu6050.c）
4.DMA传输
5.电机控制模块（motor.c）
6.电机码盘测示（encoder.c）
7.电机速度环控制（PID参数）（pid.c）
8.路径规划（转弯）（roadplan.c）
9.转弯姿态（陀螺仪闭环）（mpu6050.c）





