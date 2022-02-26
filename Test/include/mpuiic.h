#ifndef __MPUIIC_H
#define __MPUIIC_H


//IO方向设置
#define MPU_SDA_IN()  {GPIO_setDirectionMode(7U, GPIO_DIR_MODE_IN);}  //修改点
#define MPU_SDA_OUT() {GPIO_setDirectionMode(7U, GPIO_DIR_MODE_OUT);}  //修改点

//IO操作函数	 
#define MPU_IIC_SCL_Clr()    GPIO_writePin(6U, 0); 		//SCL    修改点
#define MPU_IIC_SCL_Set()    GPIO_writePin(6U, 1);
#define MPU_IIC_SDA_Clr()    GPIO_writePin(7U, 0); 		//SDA	 修改点
#define MPU_IIC_SDA_Set()    GPIO_writePin(7U, 1);        //SDA    修改点

#define MPU_READ_SDA()   GPIO_readPin(7U); 		//输入SDA   修改点
//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(unsigned char txd);			//IIC发送一个字节
unsigned char MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char MPU_IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);
#endif
















