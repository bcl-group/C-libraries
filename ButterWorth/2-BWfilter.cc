/*  *********************************************************************
        しゃ断周波数10Hzの6次のバターワースフィルターのプログラム
    ---------------------------------------------------------------------
               2列あるデータの両方ともノイズ処理を行う
    ---------------------------------------------------------------------
    ./2-BWfilter <データファイル名> <サンプリング周波数>
    *********************************************************************   */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<stream.h>
#include<complex.h>
#include "file_check.h"
#define MAXLEN 256
#define sup 0
#define NOARG 1
#define NUM 4
#define MAX 6
#define CUTOFF_FREQ 10.0 

double R[MAX],I[MAX];
double_complex X[MAX];
void com()
{
  int i;
  double r,d;
  double_complex C1[MAX];

  r=M_PI;
  for(i=0;i<MAX;i++){
    d=(double)(2*i+7)/12.0;
    R[i]=cos(d*r);
    I[i]=sin(d*r);
    C1[i]=double_complex(R[i],I[i]);
  }

  X[0]=1.0/((C1[0]-C1[1])*(C1[0]-C1[2])*(C1[0]-C1[3])*(C1[0]-C1[4])*(C1[0]-C1[5]));
  X[1]=1.0/((C1[1]-C1[0])*(C1[1]-C1[2])*(C1[1]-C1[3])*(C1[1]-C1[4])*(C1[1]-C1[5]));
  X[2]=1.0/((C1[2]-C1[0])*(C1[2]-C1[1])*(C1[2]-C1[3])*(C1[2]-C1[4])*(C1[2]-C1[5]));
  X[3]=1.0/((C1[3]-C1[0])*(C1[3]-C1[1])*(C1[3]-C1[2])*(C1[3]-C1[4])*(C1[3]-C1[5]));
  X[4]=1.0/((C1[4]-C1[0])*(C1[4]-C1[1])*(C1[4]-C1[2])*(C1[4]-C1[3])*(C1[4]-C1[5]));
  X[5]=1.0/((C1[5]-C1[0])*(C1[5]-C1[1])*(C1[5]-C1[2])*(C1[5]-C1[3])*(C1[5]-C1[4]));

}

/* 伝達関数H(s)の定数を計算 */
void fixednum(double A[],double B[],double C[],double p[],double q[],double a[],double b[],double T)
{
  int i;
  p[1]=real(X[0]); p[2]=real(X[1]); p[3]=real(X[2]);
  q[1]=imag(X[0]); q[2]=imag(X[1]); q[3]=imag(X[2]);
  a[1]=R[0]; a[2]=R[1]; a[3]=R[2];
  b[1]=I[0]; b[2]=I[1]; b[3]=I[2];
  for(i=1;i<NUM;i++){
    A[i]=2*cos(b[i]*T)*exp(a[i]*T);
    B[i]=exp(2*a[i]*T);
    C[i]=2*(p[i]*cos(b[i]*T)+q[i]*sin(b[i]*T))*exp(a[i]*T);
  } 
}

/* sに関する伝達関数をH(z)に関する伝達関数に近似したときの定数を計算 */
void calculation(double m[],double n[],double A[],double B[],double C[],double p[],double q[])
{
  int i;

  m[0]=0; m[1]=0;
  for(i=1;i<NUM;i++){
    m[0] += p[i];
    m[1] += C[i];
  }
  m[0]=2*m[0];
  m[1]=2*(p[1]*(A[2]+A[3])+p[2]*(A[1]+A[3])+p[3]*(A[1]+A[2]))+m[1];
  m[2]=2*(p[1]*(A[2]*A[3]+B[2]+B[3])+p[2]*(A[1]*A[3]+B[1]+B[3])+p[3]*(A[1]*A[2]+B[1]+B[2]))+C[1]*(A[2]+A[3])+C[2]*(A[1]+A[3])+C[3]*(A[1]+A[2]);
  m[3]=2*(p[1]*(A[2]*B[3]+A[3]*B[2])+p[2]*(A[1]*B[3]+A[3]*B[1])+p[3]*(A[1]*B[2]+A[2]*B[1]))+C[1]*(A[2]*A[3]+B[2]+B[3])+C[2]*(A[1]*A[3]+B[1]+B[3])+C[3]*(A[1]*A[2]+B[1]+B[2]);
  m[4]=2*(p[1]*B[2]*B[3]+p[2]*B[1]*B[3]+p[3]*B[1]*B[2])+C[1]*(A[2]*B[3]+A[3]*B[2])+C[2]*(A[1]*B[3]+A[3]*B[1])+C[3]*(A[1]*B[2]+A[2]*B[1]);
  m[5]=C[1]*B[2]*B[3]+C[2]*B[1]*B[3]+C[3]*B[1]*B[2];

  n[1]=0; n[2]=0;
  for(i=1;i<NUM;i++){
    n[1] += A[i];
    n[2] += B[i];
  }
  n[2]=A[1]*A[2]+A[2]*A[3]+A[3]*A[1]+n[2];
  n[3]=A[1]*B[2]+A[1]*B[3]+A[2]*B[1]+A[2]*B[3]+A[3]*B[1]+A[3]*B[2]+A[1]*A[2]*A[3];
  n[4]=A[1]*A[2]*B[3]+A[1]*A[3]*B[2]+A[2]*A[3]*B[1]+B[1]*B[2]+B[2]*B[3]+B[3]*B[1];
  n[5]=A[1]*B[2]*B[3]+A[2]*B[1]*B[3]+A[3]*B[1]*B[2];
  n[6]=B[1]*B[2]*B[3];  
}
  
int main(int argc,char *argv[])
{
  FILE *fp;
  int i;
  double f,x_start,y_start,T,x[6],X[7],y[6],Y[7];
  double A[NUM],B[NUM],C[NUM],p[NUM],q[NUM],a[NUM],b[NUM],m[6],n[7];

  if(argc != 3){
    printf("Usage: filter <filename> <sampling frequency>\n");
    exit(NOARG);
  }
  
  sscanf(argv[2],"%lf",&f);

  fp=check(argv[1]);  T=(CUTOFF_FREQ*2.0*M_PI)/f;
  
  /*sscanf(argv[3],"%lf",&x_start);
    sscanf(argv[4],"%lf",&y_start); の変わりに以下を追加した*/
  
  /*取得したデータに少しの誤差あり*/
  for(i=0;i<sup;i++){
    fscanf(fp,"%lf %lf\n",&x_start,&y_start);
  }

  /* 負の時刻におけるx[t],X[t],y[t],Y[t]は取得したデータのx[0],y[0]とする */
  for(i=0;i<6;i++){
    x[i]=x_start;
    y[i]=y_start;
  }
  for(i=0;i<7;i++){
    X[i]=x_start;
    Y[i]=y_start;
  }
  
  com();
  fixednum(A,B,C,p,q,a,b,T);
  calculation(m,n,A,B,C,p,q);
  
  while(fscanf(fp,"%lf %lf\n",&x[0],&y[0])!=EOF){
    /* 時間系列に書き直した出力関数 */
    X[0]=T*(m[0]*x[0]-m[1]*x[1]+m[2]*x[2]-m[3]*x[3]+m[4]*x[4]-m[5]*x[5])+n[1]*X[1]-n[2]*X[2]+n[3]*X[3]-n[4]*X[4]+n[5]*X[5]-n[6]*X[6];
    Y[0]=T*(m[0]*y[0]-m[1]*y[1]+m[2]*y[2]-m[3]*y[3]+m[4]*y[4]-m[5]*y[5])+n[1]*Y[1]-n[2]*Y[2]+n[3]*Y[3]-n[4]*Y[4]+n[5]*Y[5]-n[6]*Y[6];
    
    printf("%lf   %lf\n",X[0],Y[0]);
    for(i=4;i>=0;i--){
      x[i+1]=x[i];
      y[i+1]=y[i];
    }
    for(i=5;i>=0;i--){
      X[i+1]=X[i];
      Y[i+1]=Y[i];
    }
 
  }
  fclose(fp);

  return(0);
}
