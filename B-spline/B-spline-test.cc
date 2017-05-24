#include <iostream>
#include <fstream>
#include <math.h>
#include <newmat/newmatio.h>
#include "B-spline.h" // B-スプラインの計算をする

extern const int NSMP=6;        // B-スプラインの標本点の数(今は6でfix?)
extern const int JISU=3;        // B-スプラインの次数
extern const int KAI=JISU+1;// B-スプラインの階数
const int STEP=60;       // 時間の分割数
const double dt=1.0/(double)STEP;

Matrix B(NSMP+1,KAI);    // Bスプラインの基底関数

// -r〜rの乱数で行列初期化
void initAlp(ColumnVector &Alp)
{
  Alp(1)=0;
  Alp(2)=10;
  Alp(3)=15;
  Alp(4)=16;
  Alp(5)=15;
  Alp(6)=10;
}

// ---------------------------------------------------------------------
// トルクの設定:B-spline関数を用いてスプライン補間してtauに格納
void Trq(ColumnVector &Alp,  ColumnVector &tau)
{
  int kset;
  double t;

  for(int k=1;k<=STEP;k++){
    t=(double)(k-1)*dt;
    tau(k)=0;
    de_boor(B,t,kset);
  
    for(int i=kset-JISU;i<=kset;i++){
      tau(k) += Alp(i)*B(i,KAI);
    }
  }
}

void showTrq(ColumnVector &tau)
{
  for(int k=1;k<=STEP;k++)
    cout<< k <<"\t:"<<tau(k)<<"\n";
}

int main(void)
{
  ColumnVector tau(STEP);  // トルク
  ColumnVector Alp(NSMP);  // 基底関数の重み
  ColumnVector SMP(NSMP);  // NSMP個の標本点を格納

  cout <<"sample points: ";
  for (int i=1;i<=NSMP;i++) {SMP(i)=double(i-1)/double(NSMP-1); cout<<SMP(i)<<" ";}
  cout <<endl;
  B_spline(SMP,B);  /* B-スプラインの計算 */

  initAlp(Alp);
  Trq(Alp,tau);
  showTrq(tau);
}
