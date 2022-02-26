/*
 * Timer.c
 *
 *  Created on: 2021年8月3日
 *      Author: EUPTQWD
 */
#include "F28x_Project.h"
#include "device.h"
#include "math.h"
#include "timer.h"
// 计时器中断服务程序ISR
void Timer_Init(){
       // 初始化外设，这里只初始化计时器
       // Initialize the Device Peripheral. For this example, only initialize the Cpu Timers.
       InitCpuTimers();

       // 映射中断服务程序 Map ISR functions
       EALLOW;
    //   PieVectTable.TIMER0_INT = &cpuTimer0ISR;
       PieVectTable.TIMER1_INT = &cpuTimer1ISR;
  //     PieVectTable.TIMER2_INT = &cpuTimer2ISR;
       EDIS;
       // 设置计时器周期
       // 100MHz CPU 频率, 1 second 周期 (in uSeconds)
  //     ConfigCpuTimer(&CpuTimer0, 100, 1000000);
       ConfigCpuTimer(&CpuTimer1, 100, 25);//99.94
  //     ConfigCpuTimer(&CpuTimer2, 100, 1000000);
       // 计时器的控制寄存器 TIE位置1，允许其向CPU请求中断
  //     CpuTimer0Regs.TCR.all = 0x4000;
       CpuTimer1Regs.TCR.all = 0x4000;
//       CpuTimer2Regs.TCR.all = 0x4000;
   //    Interrupt_register(INT_TIMER0, cpuTimer0ISR);
       // Enables CPU int1, int13, and int14 which are connected to CPU-Timer 0,
       // CPU-Timer 1, and CPU-Timer 2 respectively.
       // Enable TINT0 in the PIE: Group 1 interrupt 7
   //    CPUTimer_enableInterrupt(INT_TIMER0);

 //      Interrupt_enable(INT_TIMER0);
       Interrupt_enable(INT_TIMER1);
   //    Interrupt_enable(INT_TIMER2);

       //IER |= M_INT1;
      // IER |= M_INT13;
      // IER |= M_INT14;
       // Enable TINT0 in the PIE: Group 1 interrupt 7
   //    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}
// cpuTimer0ISR - CPU Timer0 ISR with interrupt counter


// cpuTimer1ISR - CPU Timer1 ISR with interrupt counter
/*
__interrupt void cpuTimer0ISR(void)
{
    GPIO_togglePin(DEVICE_GPIO_PIN_LED2);

    // The CPU acknowledges the interrupt
    CpuTimer0.InterruptCount++;
}

// cpuTimer2ISR CPU Timer2 ISR with interrupt counter
__interrupt void cpuTimer2ISR(void)
{
    // The CPU acknowledges the interrupt
    CpuTimer2.InterruptCount++;
}
*/

