/*  ************************************************************
                 規定となるn次のB-splineを設定する
    ------------------------------------------------------------
              標本点(n)，スプラインの次数は自分で決める
    ************************************************************  */

#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include <newmat/newmatio.h>

extern int NSMP; // 標本点の数: 
extern int JISU;
extern int KAI;

ColumnVector q(NSMP+KAI);
/* SMP:標本点とするxの座標 q:節点のx座標を格納する B:スプライン */

/* de Boor-Coxの漸化式によるスプライン関数値計算 */
// B(NSMP+1,KAI)
void de_boor(Matrix &B, double T,int &kset)
{
  int i,j;
  
  /* 2次元配列の初期化(全ての要素を0にする) */
  B=0.0;

  /* 節点q[i]とq[i+1]にデータ点が含まれているか調べる */
  /* データ点があればそのときのB[i][1]を1とする */
  for(i=1;i<=NSMP;i++){
    if(q(i) <= T && T < q(i+1)){
      B(i,1)=1.0;
      kset=i;
    }
  }
  
  if(q(NSMP)<= T && T <= q(NSMP+1)+pow(10.0,-5.0)){
    B(NSMP,1)=1.0;
    kset=NSMP;
  }
 
  /* 与えられたTにおけるJISU次のB-スプラインを得る */
  for(j=2;j<=KAI;j++){
    for(i=kset-j+1;i<=kset;i++){
      B(i,j)=0;
      if(q(i+1) != q(i+j)) B(i,j)+=(q(i+j)-T)*B(i+1,j-1)/(q(i+j)-q(i+1));
      if(q(i) != q(i+j-1)) B(i,j)+=(T-q(i))*B(i,j-1)/(q(i+j-1)-q(i));
    }
  }
}

// SMP(NSMP), B(NSMP+1,KAI)
void B_spline(ColumnVector &SMP, Matrix &B)
{
  int i,kset;

  /* 節点の設定 (x座標) */
  for(i=1;i<=KAI;i++)     { q(i)=SMP(1); q(i+NSMP)=SMP(NSMP); }
  for(i=1;i<=NSMP-KAI;i++) q(i+KAI)=(SMP(i)+SMP(i+KAI))/2.0;
  
  /* データ点でのスプライン関数値による行列の構成 */
  for(i=1;i<=NSMP;i++)     de_boor(B,SMP(i),kset);
}

