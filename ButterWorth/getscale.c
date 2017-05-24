/* ************************************************************************
     4��η�¬�ǡ������饿�����åȴ�(2����)�ε�Υ����롣
   ------------------------------------------------------------------------
     ./getscale <�ե�����̾1> <�ե�����̾2> <�������åȴ֤ε�Υ(m)> 
   ************************************************************************ */ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define sup 30

int main(int argc, char *argv[])
{
  FILE *fp1,*fp2;
  int i,data;
  char buf1[sup],buf2[sup];
  double dist,range,x[sup][2],y[sup][2];
  
  if(argc != 4){
    printf("./getscale <�ե�����1> <�ե�����2> <�������åȴ֤ε�Υ(m)>\n"); 
    exit(-1);
  }
  
  if( (fp1=fopen(argv[1],"r"))==NULL ){ 
    puts("read open error file1");
    exit(-1);
  }

  if( (fp2=fopen(argv[2],"r"))==NULL ){ 
    puts("read open error file2");
    exit(-1); 
  }
  
  dist=atof(argv[3]);

  for(i=0;i<sup;i++){
    fgets(buf1,sup,fp1);
    sscanf(buf1,"%lf %lf\n",&x[i][0],&y[i][0]); 
    
    fgets(buf2,sup,fp2);
    sscanf(buf2,"%lf %lf\n",&x[i][1],&y[i][1]); 
  }
  
  range=sqrt( (x[sup-1][1]-x[sup-1][0])*(x[sup-1][1]-x[sup-1][0])+
	      (y[sup-1][1]-y[sup-1][0])*(y[sup-1][1]-y[sup-1][0]) );
  
  /*  printf("%lf %lf %lf %lf\n",x[sup-1][0],y[sup-1][0],
      x[sup-1][1],y[sup-1][1]);*/

  printf("%lf \n",dist/range);
  
  fclose(fp1);
  fclose(fp2);

}
