/*  *****************************************************************
    ２列の計測データから、水平方向、垂直方向における速度・加速度の時間変化
    および速度・加速度の大きさの時間変化を求める。 
    ****************************************************************  */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 15000

//行列 DATA1には元データ DATA2には速度 DATA3には加速度を格納
double DATA1[MAX][2],DATA2[MAX][3],DATA3[MAX][3];


int main(int argc, char *argv[]){

  void acceleration(int i,double x1,double x2,double y1,double y2,double *D);
  void velocity(int i,double x1,double x2,double y1,double y2,double *D);

  char buffer[MAX];
  FILE *fp;
  int i=1,data;
  double j=0.0,D;

  if(argc != 3){
    printf("getspd <ファイル名> <サンプリング周波数> \n");
    exit(-1);
  }

  D=1.0/atof(argv[2]);

  if( (fp=fopen(argv[1],"r"))==NULL ){ 
    puts("read open error!!");
    exit(-1);
  }

  while ((fgets(buffer,256,fp)) != NULL) {

    sscanf(buffer,"%lf %lf\n",&DATA1[i][1],&DATA1[i][2]);
    
    if(i>1){

    velocity(i,DATA1[i-1][1],DATA1[i][1],DATA1[i-1][2],DATA1[i][2],&D);

    acceleration(i,DATA2[i-1][1],DATA2[i][1],DATA2[i-1][2],DATA2[i][2],&D);
    
    }
    
    i++; 
  }

  data=i;
  for(i=1;i<data;i++) {
    // cout<<DATA2(i,1)<<" "<<DATA2(i,2)<<" "<<DATA2(i,3)<<" "
    //  <<DATA3(i,1)<<" "<<DATA3(i,2)<<" "<<DATA3(i,3)<<"\n";

    printf("%f %lf %lf %lf %lf %lf %lf \n",j,
	   DATA2[i][1],DATA2[i][2],DATA2[i][3],
	   DATA3[i][1],DATA3[i][2],DATA3[i][3]);

    j+=D;
  }
  
  fclose(fp);
}
    
//加速度を求める
void acceleration(int i,double x1,double x2,double y1,double y2,double *D) {

  DATA3[1][1]=0.0;
  DATA3[1][2]=0.0;

  DATA3[i][1] = (x2 - x1)/(*D); //x座標
  DATA3[i][2] = (y2 - y1)/(*D); //y座標
  DATA3[i][3] = sqrt( (DATA3[i][1])*(DATA3[i][1])+
		     (DATA3[i][2])*(DATA3[i][2]) ); //大きさ

}
//速度を求める
void velocity(int i,double x1,double x2,double y1,double y2,double *D) {

  DATA2[1][1]=0.0;
  DATA2[1][2]=0.0;
 
  DATA2[i][1] = (x2 - x1)/(*D);
  DATA2[i][2] = (y2 - y1)/(*D);
  DATA2[i][3] = sqrt( (DATA2[i][1])*(DATA2[i][1])+
		     (DATA2[i][2])*(DATA2[i][2]) );

}
