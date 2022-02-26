 #include "math.h"
const int N =1024;
    const int N1 = 512;
    const int n = 5;
float sig_d[N];
void kfft(float pr[],float pi[],int n,int k,float fr[],float fi[])
  {
    int it,m,is,i,j,nv,l0;
    float p,q,s,vr,vi,poddr,poddi;
    for (it=0; it<=n-1; it++)  //将pr[0]和pi[0]循环赋值给fr[]和fi[]
    {
        m=it;
        is=0;
        for(i=0; i<=k-1; i++)
        {
            j=m/2;
            is=2*is+(m-2*j);
            m=j;
        }
        fr[it]=pr[is];
        fi[it]=pi[is];
    }
    pr[0]=1.0;
    pi[0]=0.0;
    p=6.283185306/(1.0*n);
    pr[1]=cos(p); //将w=e^-j2pi/n用欧拉公式表示
    pi[1]=-sin(p);

    for (i=2; i<=n-1; i++)  //计算pr[]
    {
        p=pr[i-1]*pr[1];
        q=pi[i-1]*pi[1];
        s=(pr[i-1]+pi[i-1])*(pr[1]+pi[1]);
        pr[i]=p-q; pi[i]=s-p-q;
    }
    for (it=0; it<=n-2; it=it+2)
    {
        vr=fr[it];
        vi=fi[it];
        fr[it]=vr+fr[it+1];
        fi[it]=vi+fi[it+1];
        fr[it+1]=vr-fr[it+1];
        fi[it+1]=vi-fi[it+1];
    }
    m=n/2;
    nv=2;
    for (l0=k-2; l0>=0; l0--) //蝴蝶操作
    {
        m=m/2;
        nv=2*nv;
        for (it=0; it<=(m-1)*nv; it=it+nv)
          for (j=0; j<=(nv/2)-1; j++)
            {
                p=pr[m*j]*fr[it+j+nv/2];
                q=pi[m*j]*fi[it+j+nv/2];
                s=pr[m*j]+pi[m*j];
                s=s*(fr[it+j+nv/2]+fi[it+j+nv/2]);
                poddr=p-q;
                poddi=s-p-q;
                fr[it+j+nv/2]=fr[it+j]-poddr;
                fi[it+j+nv/2]=fi[it+j]-poddi;
                fr[it+j]=fr[it+j]+poddr;
                fi[it+j]=fi[it+j]+poddi;
            }
    }
    for (i=0; i<=n-1; i++)
       {
          pr[i]=sqrt(fr[i]*fr[i]+fi[i]*fi[i]);  //幅值计算
       }
    return;
  }
float flyjs(int nn,float signal[],double u[]){
    int fs,f;
    f = (nn-1)*40000.0/1024.0; // 25是40k
    fs = f * 40;
    int i,j=0;
    float w = 2 * 3.1415926 * f / fs*1.0;
    float thd;
    // xn = square(0.1 * pi * n)
    //xn = sawtooth(0.1 * pi * n, 0.5)
    //xn = sin(2 * pi * f / fs * n) + 4 * sin(2 * 2 * pi * f / fs * n)
    float sum = 0, sum2 = 0;
    for (i = 0; i < N1; i++)
    {
        sig_d[i * 2] = signal[i];
    }
    for (i = 0; i < N1 - 2; i++)
    {
        sig_d[i * 2+1] = sig_d[i * 2 ] + sig_d[i * 2 + 2] / 2;
    }
    sig_d[N-1] = sig_d[N - 2] * 2 - sig_d[N - 3];
    for (j = 1; j < 6; j++)
    {
        sum = sum2 = (float)(0.0);
        for (i = 0; i < N; i++)
        {
            sum = sum + sin(j *w * i/2) * sig_d[i];
            sum2 = sum2 + cos(j * w * i/2) * sig_d[i];
        }
        u[j - 1] = sum * sum + sum2 * sum2;
    }

    return sqrt((u[1] + u[2] + u[3] + u[4]) / u[0]);
}




