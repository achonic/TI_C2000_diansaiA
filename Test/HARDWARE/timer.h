/*
 * timer.h
 *
 *  Created on: 2021��11��5��
 *      Author: EUPTQWD
 */

#ifndef HARDWARE_TIMER_H_
#define HARDWARE_TIMER_H_


__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
__interrupt void cpuTimer2ISR(void);
void Timer_Init();


#endif /* HARDWARE_TIMER_H_ */
