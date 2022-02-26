#ifndef __MPUIIC_H
#define __MPUIIC_H


//IO��������
#define MPU_SDA_IN()  {GPIO_setDirectionMode(7U, GPIO_DIR_MODE_IN);}  //�޸ĵ�
#define MPU_SDA_OUT() {GPIO_setDirectionMode(7U, GPIO_DIR_MODE_OUT);}  //�޸ĵ�

//IO��������	 
#define MPU_IIC_SCL_Clr()    GPIO_writePin(6U, 0); 		//SCL    �޸ĵ�
#define MPU_IIC_SCL_Set()    GPIO_writePin(6U, 1);
#define MPU_IIC_SDA_Clr()    GPIO_writePin(7U, 0); 		//SDA	 �޸ĵ�
#define MPU_IIC_SDA_Set()    GPIO_writePin(7U, 1);        //SDA    �޸ĵ�

#define MPU_READ_SDA()   GPIO_readPin(7U); 		//����SDA   �޸ĵ�
//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
unsigned char MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char MPU_IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);
#endif
















