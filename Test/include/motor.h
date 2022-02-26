#ifndef _MOTOR_H

#define _MOTOR_H
void Motor_Init();

void MoveForward();
void MoveBack();
__interrupt void epwm1ISR(void);
/*
void PWM_M1(uint16_t val);
void PWM_M2(uint16_t val);
void PWM_M3(uint16_t val);
void PWM_M4(uint16_t val);
*/
#endif 


