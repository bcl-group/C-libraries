/*  *****************************************************************
    ����η�¬�ǡ������顢��ʿ��������ľ�����ˤ�����®�١���®�٤λ����Ѳ�
    �����®�١���®�٤��礭���λ����Ѳ�����롣 
    ****************************************************************  */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 15000

//���� DATA1�ˤϸ��ǡ��� DATA2�ˤ�®�� DATA3�ˤϲ�®�٤��Ǽ
double DATA1[MAX][2],DATA2[MAX][3],DATA3[MAX][3];


int main(int argc, char *argv[]){

  void acceleration(int i,double x1,double x2,double y1,double y2,double *D);
  void velocity(int i,double x1,double x2,double y1,double y2,double *D);

  char buffer[MAX];
  FILE *fp;
  int i=1,data;
  double j=0.0,D;

  if(argc != 3){
    printf("getspd <�ե�����̾> <����ץ�󥰼��ȿ�> \n");
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
    
//��®�٤����
void acceleration(int i,double x1,double x2,double y1,double y2,double *D) {

  DATA3[1][1]=0.0;
  DATA3[1][2]=0.0;

  DATA3[i][1] = (x2 - x1)/(*D); //x��ɸ
  DATA3[i][2] = (y2 - y1)/(*D); //y��ɸ
  DATA3[i][3] = sqrt( (DATA3[i][1])*(DATA3[i][1])+
		     (DATA3[i][2])*(DATA3[i][2]) ); //�礭��

}
//®�٤����
void velocity(int i,double x1,double x2,double y1,double y2,double *D) {

  DATA2[1][1]=0.0;
  DATA2[1][2]=0.0;
 
  DATA2[i][1] = (x2 - x1)/(*D);
  DATA2[i][2] = (y2 - y1)/(*D);
  DATA2[i][3] = sqrt( (DATA2[i][1])*(DATA2[i][1])+
		     (DATA2[i][2])*(DATA2[i][2]) );

}
