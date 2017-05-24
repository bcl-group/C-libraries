#include <iostream>
#include <fstream>
#include <math.h>
#include <newmat/newmatio.h>
#include "B-spline.h" // B-���ץ饤��η׻��򤹤�

extern const int NSMP=6;        // B-���ץ饤���ɸ�����ο�(����6��fix?)
extern const int JISU=3;        // B-���ץ饤��μ���
extern const int KAI=JISU+1;// B-���ץ饤��γ���
const int STEP=60;       // ���֤�ʬ���
const double dt=1.0/(double)STEP;

Matrix B(NSMP+1,KAI);    // B���ץ饤��δ���ؿ�

// -r��r������ǹ�������
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
// �ȥ륯������:B-spline�ؿ����Ѥ��ƥ��ץ饤����֤���tau�˳�Ǽ
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
  ColumnVector tau(STEP);  // �ȥ륯
  ColumnVector Alp(NSMP);  // ����ؿ��νŤ�
  ColumnVector SMP(NSMP);  // NSMP�Ĥ�ɸ�������Ǽ

  cout <<"sample points: ";
  for (int i=1;i<=NSMP;i++) {SMP(i)=double(i-1)/double(NSMP-1); cout<<SMP(i)<<" ";}
  cout <<endl;
  B_spline(SMP,B);  /* B-���ץ饤��η׻� */

  initAlp(Alp);
  Trq(Alp,tau);
  showTrq(tau);
}
