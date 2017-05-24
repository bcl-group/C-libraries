   /*
   *************************************************************************
   デバイスドライバを介してAD変換ボードを制御し、データを取得するプログラム
    -------------------------------------------------------------------------
    retの値について
     ・関数が正常終了すると0が返され、プログラムはそのまま続行。
     ・エラーが起こった時は0以外の値が返され、その時点でプログラムは終了。
    -------------------------------------------------------------------------
     データが正常に取得されたら"normal end"と表示して終了
    -------------------------------------------------------------------------
     ./samp <データファイル名> <チャンネル数> <サンプリング周波数> <データ数> 
    ************************************************************************* 
    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbiad.h"

#define dnum 1
#define NOARG -1
int main(int argc,char *argv[])
{
  int i,ret;
  ADSMPLREQ Conf;

  //  system("clear");

  if(argc != 5){
    printf("./samp <file_name> <channel_num> <frequency> <data_num>\n");
    exit(NOARG);
  }

  ret=AdOpen(dnum);
  if(ret != 0){
    printf("open error: ret=%x\n",ret);
    exit(EXIT_FAILURE);
  }

  ret=AdGetSamplingConfig(dnum,&Conf);
  if(ret != 0){
    printf("AdGetSamplingConfig error: ret=%x\n",ret);
    AdClose(dnum);
    exit(EXIT_FAILURE);
  }

  Conf.ulChCount=2*(atoi(argv[2]));
  Conf.ulSamplingMode=AD_FIFO_SAMPLING;
  Conf.ulSingleDiff=AD_INPUT_DIFF;
  Conf.fSmplFreq=atof(argv[3]);
  Conf.ulSmplNum=atoi(argv[4]);
  
  for(i=0;i<2*(atoi(argv[2]));i++){
    Conf.SmplChReq[i].ulChNo=i+1;
    Conf.SmplChReq[i].ulRange=AD_5V;
  }

  ret=AdSetSamplingConfig(dnum,&Conf);
  if(ret != 0){
    printf("AdSetSamplingConfig eror: ret=%x\n",ret);
    AdClose(dnum);
    exit(EXIT_FAILURE);
  }

  ret=AdStartFileSampling(dnum,argv[1],FLAG_CSV);
  if(ret != 0){
    printf("AdStartFileSampling error: ret=%x\n",ret);
    AdClose(dnum);
    exit(EXIT_FAILURE);
  }

  ret=AdClose(dnum);
  //  printf("AdClose %x\n",ret);
  printf("normal end\n\n");
  return(0);
}
