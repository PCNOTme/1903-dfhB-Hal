/**
  ******************************************************************************
  * 文件名程: ff_gen_drv.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: FatFS文件系统通用存储设备操作实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "ff_gen_drv.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
Disk_drvTypeDef disk = {{0},{0},{0},0};

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 链接一个存储设备并且增加当前活动的设备数目
  * 输入参数: drv：存储设备输入输出操作函数结构体指针
  *           path：逻辑设备路径缓冲区指针
  *           lun：只用于USB设备以添加多个逻辑分区，否则设置lun为0
  * 返 回 值: 0：操作成功，1：操作出错
  * 说    明: FatFS最大支持活动设备数目为10个
  */
uint8_t FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, uint8_t lun)
{
  uint8_t ret = 1;
  uint8_t DiskNum = 0;
  
  if(disk.nbr <= _VOLUMES)
  {
    disk.is_initialized[disk.nbr] = 0;
    disk.drv[disk.nbr] = drv;  
    disk.lun[disk.nbr] = lun;  
    DiskNum = disk.nbr++;
    path[0] = DiskNum + '0';
    path[1] = ':';
    path[2] = '/';
    path[3] = 0;
    ret = 0;
  }  
  return ret;
}

/**
  * 函数功能: 链接一个存储设备并且增加当前活动的设备数目
  * 输入参数: drv：存储设备输入输出操作函数结构体指针
  *           path：逻辑设备路径缓冲区指针
  * 返 回 值: 0：操作成功，1：操作出错
  * 说    明: FatFS最大支持活动设备数目为10个
  */
uint8_t FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path)
{
  return FATFS_LinkDriverEx(drv, path, 0);
}

/**
  * 函数功能: 解除一个存储设备并且减少当前活动的设备数目
  * 输入参数: path：逻辑设备路径缓冲区指针
  *           lun：不用
  * 返 回 值: 0：操作成功，1：操作出错
  * 说    明: 无
  */
uint8_t FATFS_UnLinkDriverEx(char *path, uint8_t lun)
{ 
  uint8_t DiskNum = 0;
  uint8_t ret = 1;
  
  if(disk.nbr >= 1)
  {    
    DiskNum = path[0] - '0';
    if(disk.drv[DiskNum] != 0)
    {
      disk.drv[DiskNum] = 0;
      disk.lun[DiskNum] = 0;
      disk.nbr--;
      ret = 0;
    }
  }  
  return ret;
}

/**
  * 函数功能: 解除一个存储设备并且减少当前活动的设备数目
  * 输入参数: path：逻辑设备路径缓冲区指针
  * 返 回 值: 0：操作成功，1：操作出错
  * 说    明: 无
  */
uint8_t FATFS_UnLinkDriver(char *path)
{ 
  return FATFS_UnLinkDriverEx(path, 0);
}

/**
  * 函数功能: 获取当前FatFS模组中已链接的设备数目
  * 输入参数: 无
  * 返 回 值: 附属在FatFS模组上的设备数目
  * 说    明: 无
  */
uint8_t FATFS_GetAttachedDriversNbr(void)
{
  return disk.nbr;
}
 
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
