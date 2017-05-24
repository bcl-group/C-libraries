#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#define OPEN_ERR 2

FILE *check(char *argv)
{
  FILE *fp;

  fp=fopen(argv,"r");
  if(fp==NULL){
    fprintf(stderr,"%s is not found\n",argv);
    exit(OPEN_ERR);
  }
  return(fp);
}
