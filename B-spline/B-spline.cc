/*  ************************************************************
                 ����Ȥʤ�n����B-spline�����ꤹ��
    ------------------------------------------------------------
              ɸ����(n)�����ץ饤��μ����ϼ�ʬ�Ƿ���
    ************************************************************  */

#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include <newmat/newmatio.h>

extern int NSMP; // ɸ�����ο�: 
extern int JISU;
extern int KAI;

ColumnVector q(NSMP+KAI);
/* SMP:ɸ�����Ȥ���x�κ�ɸ q:������x��ɸ���Ǽ���� B:���ץ饤�� */

/* de Boor-Cox���������ˤ�륹�ץ饤��ؿ��ͷ׻� */
// B(NSMP+1,KAI)
void de_boor(Matrix &B, double T,int &kset)
{
  int i,j;
  
  /* 2��������ν����(���Ƥ����Ǥ�0�ˤ���) */
  B=0.0;

  /* ����q[i]��q[i+1]�˥ǡ��������ޤޤ�Ƥ��뤫Ĵ�٤� */
  /* �ǡ�����������Ф��ΤȤ���B[i][1]��1�Ȥ��� */
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
 
  /* Ϳ����줿T�ˤ�����JISU����B-���ץ饤������� */
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

  /* ���������� (x��ɸ) */
  for(i=1;i<=KAI;i++)     { q(i)=SMP(1); q(i+NSMP)=SMP(NSMP); }
  for(i=1;i<=NSMP-KAI;i++) q(i+KAI)=(SMP(i)+SMP(i+KAI))/2.0;
  
  /* �ǡ������ǤΥ��ץ饤��ؿ��ͤˤ�����ι��� */
  for(i=1;i<=NSMP;i++)     de_boor(B,SMP(i),kset);
}

