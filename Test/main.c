/**********************
README  信号失真度测量装置
本工程为完成了2021全国大学生电子设计竞赛A题的基础部分
使用硬件：主控 TI C2000_F280049C
                   蓝牙hc-05
        0.96OLED SSD1306
                   串口屏
功能：能够完成1khz频率正弦波，三角波，多次谐波的采集和显示，并且失真度THD<=5%
           能够将波形画在手机软件和串口屏上
           能够显示归一化幅值在OLED上
           能显示THD在所有屏幕上
算法：使用算法 快速傅里叶变换+傅里叶级数展开 在fft.c文件中
***************************/
#include "driverlib.h"
#include "F28x_Project.h"
#include "device.h"
#include "fft.h"
#include "FFT2.h"
#include "bluetooth.h"
#include "oled.h"
#include "timer.h"
#include "stdio.h"
#include "math.h"
#define u8 unsigned char
#define  N    1024
#define  Fs   10240
#define  F    10
uint16_t data[N];
double h1,h2,h3,h4;
int end = 0;
float hh;
float a[1024],mo[1024],fr[1024],fi[1024],pr[1024],pi[1024];
double u[6];

u8 temp1[20],flag;

#define u8 unsigned char
uint16_t adcAResult0;
uint16_t adcAResult1;

void initADCs(void);
void initADCSOCs(void);
uint16_t Get_Adc(){ //采集电压
            ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER1);
        while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
        adcAResult0 = ADC_readResult(ADCARESULT_BASE,ADC_SOC_NUMBER1);
        return adcAResult0;
}
int end;

float Get_qjmax(int l,int r){
    float maxm;
    int i;
    maxm = 0;
    for(i = l;i <= r;i++) {
        if(pr[i] > maxm) maxm = pr[i];
    }
    return maxm;
}
int Get_qjmaxi(int l,int r){
    int maxmi;
    int i;
    maxmi = l;
    for(i = l;i <= r;i++) {
        if(pr[i] > pr[maxmi]) maxmi = i;
    }
    return maxmi;
}
/******************  在手机软件上绘制波形图，这里根据不同软件要求的帧头和发送帧格式自行替换      **************************/
void Draw_Map(uint16_t s[],int length,float thd){
    int i;
    for(i = 0;i <= length;i++){
                       int s2,j,s3;
                       s2 = s[i];
                       s3 = thd;
                       int cur = 0;
                       u8 chh[7],sum = 0;
                       SCI_writeCharBlockingNonFIFO(SCIB_BASE, 0xa5);
                       DEVICE_DELAY_US(1000);
                       while(s2 > 0){

                           chh[++cur] = s2&255;
                           sum+=chh[cur];
                           s2 = s2>>8;
                       }
                       for(j = 1;j <= cur;j++) SCI_writeCharBlockingNonFIFO(SCIB_BASE,chh[j]);
                       for(j = cur+1;j <= 4;j++) SCI_writeCharBlockingNonFIFO(SCIB_BASE,0x00);
                       cur = 0;
                       while(s3 > 0){
                                 chh[++cur] = s3&255;
                                 sum+=chh[cur];
                                 s3 = s3>>8;
                        }
                       for(j = 1;j <= cur;j++) SCI_writeCharBlockingNonFIFO(SCIB_BASE,chh[j]);
                       for(j = cur+1;j <= 4;j++) SCI_writeCharBlockingNonFIFO(SCIB_BASE,0x00);
                       SCI_writeCharBlockingNonFIFO(SCIB_BASE, sum);
                       SCI_writeCharBlockingNonFIFO(SCIB_BASE, 0x5a);
                }
}
/********************  将THD值显示在串口屏上      **********************/
void Send_ART(float dht){
    SCI_writeCharArray(SCIB_BASE,"n0.val=",7);  //使用串口B与串口屏蓝牙连接作为与手机通讯的接口
    int ss,cur,i;
    cur = 0,i = 0;
    ss = dht;
    char ch[7];
    for(;ss>=1;ss/=10){
        ch[cur++] = ss%10+'0';
    }
    for(i = cur-1;i >= 0;i--)
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,ch[i]);
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
}
void Send_Map(){

    int i,k;
    for(i = 24;i<= 124;i++){
        k = 0;
        while(k <= 2){
       SCI_writeCharArray(SCIB_BASE,"add 1,0,",8);
       int s;
       int cur,j;
       cur = 0;
       s = data[i]/8;
       char ch[7];
           for(;s>=1;s/=10){
               ch[cur++] = s%10+'0';
           }
           for(j = cur-1;j >= 0;j--)
               SCI_writeCharBlockingNonFIFO(SCIB_BASE,ch[j]);


               SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
               SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
               SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
           k++;
        }
    }
    flag = 1;
    SCI_writeCharArray(SCIB_BASE,"cle 1,0",7);
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
    SCI_writeCharBlockingNonFIFO(SCIB_BASE,0xff);
}
void Draw_Map_Point(float ww){
                       float s1 = ww;
                       uint32_t s2,cur,j;
                       s2 = s1;
                       cur = 0;
                       u8 chh[7],sum = 0;
                       SCI_writeCharBlockingNonFIFO(SCIB_BASE, 0xa5);
                       DEVICE_DELAY_US(1000);
                       while(s2 > 0){

                           chh[++cur] = s2&255;
                           sum+=chh[cur];
                           s2 = s2>>8;
                       }
                       for(j = 1;j <= cur;j++) SCI_writeCharBlockingNonFIFO(SCIB_BASE,chh[j]),DEVICE_DELAY_US(1000);
                       for(j = cur+1;j <= 4;j++) SCI_writeCharBlockingNonFIFO(SCIB_BASE,0x00),DEVICE_DELAY_US(1000);
                       SCI_writeCharBlockingNonFIFO(SCIB_BASE, sum);
                       DEVICE_DELAY_US(1000);
                       SCI_writeCharBlockingNonFIFO(SCIB_BASE,0x5a);
                       DEVICE_DELAY_US(1000);
}
void main(void)
{
    /****************    初始化各项外设        **************/
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    OLED_Init();
    Timer_Init();
    Bluetooth_Init();
    initADCs();
    initADCSOCs();
    /**********************************************/

    OLED_Clear();
    EINT;
    ERTM;
    int count;
    int i;
    flag = 0;
    count = 0;
    i = 0;
    while(1)
    {
        if(end==1)
               {

            InitBufInArray();

            kfft(pr,pi,1024,10,fr,fi); //快速傅里叶变换计算出
            GetTHD();


            end = 0;

        // Hit run again to get updated conversions.
 //       ESTOP0;
       }
    }
}
// 初始化ADC
void initADCs(void)
{

    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    // Set ADCCLK divider to /4
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_1_0);
    ADC_setPrescaler(ADCB_BASE, ADC_CLK_DIV_4_0);


    // Set pulse positions to late
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);

    // Power up the ADCs and then delay for 1 ms
    ADC_enableConverter(ADCA_BASE);
    ADC_enableConverter(ADCB_BASE);

    DEVICE_DELAY_US(1000);

}

// 初始化ADCSOC
void initADCSOCs(void)
{

    // - SOC0 will convert pin A0 with a sample window of 10 SYSCLK cycles.
    // - SOC1 will convert pin A1 with a sample window of 10 SYSCLK cycles.
    //
 //   ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
   //              ADC_CH_ADCIN0, 10);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY,
                 ADC_CH_ADCIN5, 20);

    //
    // Set SOC1 to set the interrupt 1 flag. Enable the interrupt and make
    // sure its flag is cleared.
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
}


void InitBufInArray()
{
 int i;
 for(i=0; i < N; i++)
    {
       pr[i]  = data[i]*3.30/4095.0;//
       pi[i] = 0;
             //
    }

}

void GetTHD()
{


    unsigned short i=20;//
    float  Uo1,Uo2,Uo3,Uo4,Uo5;
    float THD,thd_fz,thd_fm,thd_fc;
    Uo1 = Get_qjmax(40,60);

    int nn = Get_qjmaxi(40,60);
    InitBufInArray();

    THD = flyjs(nn,pr,u);
    THD *= 100;
    Uo2 = Get_qjmax(90,110);
    Uo3 = Get_qjmax(140,160);
    Uo4 = Get_qjmax(190,210);
    Uo5 = Get_qjmax(240,260);

    Send_ART(THD);
    Send_ART(THD);
    h1 = sqrt(u[1]/u[0]);
    int sa = h1*10;
    OLED_ShowString(1,0,"Un2/Un1=0.",16);
    OLED_ShowNum(76,0,sa%10,1,16);
    sa = h1*100;
    OLED_ShowNum(84,0,sa%10,1,16);
    h2 =  sqrt(u[2]/u[0]);
    int sb = h2*10;
    OLED_ShowString(1,2,"Un3/Un1=0.",16);
        OLED_ShowNum(76,2,sb%10,1,16);
        sb = h2*100;
        OLED_ShowNum(84,2,sb%10,1,16);
        h3 = sqrt(u[3]/u[0]);
        int sc = h3*10;
        OLED_ShowString(1,4,"Un4/Un1=0.",16);
            OLED_ShowNum(76,4,(h3*10),1,16);
            sc = h3*100;
            OLED_ShowNum(84,4,sc%10,1,16);
            OLED_ShowString(1,6,"Un5/Un1=0.",16);
            h4 = sqrt(u[4]/u[0]);
            int sd = h4*10;
                OLED_ShowNum(76,6,sd%10,1,16);
               sd = h4*100;
                OLED_ShowNum(84,6,sd%10,1,16);
    Send_Map();
    Draw_Map(data,80,THD);
    CPUTimer_startTimer(CpuTimer1.RegsAddr);
 //   goto reset;
 //   OLED_ShowString(1 ,2,s,16);
}

//利用定时器中断采集电压信息
__interrupt void cpuTimer1ISR(void)
{
       data[CpuTimer1.InterruptCount]=Get_Adc();
       CpuTimer1.InterruptCount++;

            if(CpuTimer1.InterruptCount==1024) // 若已采集1024个点 关闭定时器
            {
                CPUTimer_stopTimer(CpuTimer1.RegsAddr);
                CpuTimer1.InterruptCount=0;
                end=1;

            }
    // Acknowledge this interrupt to receive more interrupts from group 1
   //      Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
 //   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

