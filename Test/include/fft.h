/*
 * fft.h
 *
 *  Created on: 2021年11月4日
 *      Author: EUPTQWD
 */

#ifndef INCLUDE_FFT_H_
#define INCLUDE_FFT_H_

#define PI 3.1415926535
void kfft(float pr[],float pi[],int n,int k,float fr[],float fi[]);
float Get_Thd(float a[],float w,float mo[],int N);
float flyjs(int nn,float signal[],float u[]);

#endif /* INCLUDE_FFT_H_ */
