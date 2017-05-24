 /*ボタン操作での再生・停止、スロー、早送り、巻き戻し、保存、座標、
  グラフ(中心点表示)*/
#include <gtk/gtk.h>
#include <gdk_imlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <iostream.h>
#include <fstream.h>
#include <limits.h>
#include <math.h>
#include <complex.h>

#include "ReadRiff.h"

#define pspace fputs(space, stdout)
#define INTERVAL 33
#define SLOW 500
#define FAST 10
#define SPACE 30
#define Width 300
#define AREA 250
#define MAXLINE 100
#define COLORNUM 255
#define MAXMARKER 10
#define PIXELNUM 150 //ノイズ除去時の閾値となる画素数
#define MAXLEN 256
#define NOARG 1
#define NUM 4
#define MAXNUM 6

#define M_OP "/修正"
#define M_OP_SEP1 "/修正/sep1"
#define M_OP_LOW_PASS "/修正/低周波フィルター"
#define M_OP_UNSTRIATED "/修正/平滑化"
#define M_EF "/操作"
#define M_EF_SEP1 "/操作/sep1"
#define M_EF_VT "/操作/V-tグラフ"
#define M_EF_HIST "/操作/ヒストグラム"
#define M_EF_ORIGINAL "/操作/処理前"
#define M_EF_NOISE "/操作/ノイズ除去"
#define M_EF_BARF "/操作/戻す"
#define M_EF_XT "/操作/X-tグラフ"
#define M_EF_YT "/操作/Y-tグラフ"
#define M_EF_VXT "/操作/Vx-tグラフ"
#define M_EF_VYT "/操作/Vy-tグラフ"
#define M_EF_IN "/操作/輝度入力"
#define M_EF_FILE "/操作/ファイルに出力"


enum {OP_NOTHING,OP_BACK,OP_PLAY,OP_FAST,OP_SLOW,OP_PAUSE,OP_ORIGINAL,
      OP_NOISE,OP_BARF,OP_GRAPH,OP_HIST,OP_LOW_PASS,OP_UNSTRIATED,
      OP_VXT,OP_VYT,OP_VT,OP_XT,OP_YT,OP_FILE,OP_WIDTH,OP_HEIGHT};

FILE *getfp1(char* n);

int dwWidth, dwHeight;
int dwTotalFrames;
int biSizeImage;

FILE *fp1, *fp2, *fp3;
int N=OP_NOTHING;
int N1=OP_BARF;
int N2=OP_NOTHING;
int N3=OP_NOTHING;
int FRAME1=0;
long int FRAME2;
int kido;
static int tau=1;
int AREA_X,AREA_Y,AREAX=1;
int MARKER;         //マーカの数
int P[COLORNUM+1];
double X[MAXMARKER],Y[MAXMARKER],X0[MAXMARKER],Y0[MAXMARKER];
double XF[MAXMARKER],YF[MAXMARKER],XF0[MAXMARKER],YF0[MAXMARKER];
static double R[MAXNUM],I[MAXNUM];
static double_complex E[MAXNUM];
double X1,Y1;
int click=0;
int WIDTH,HEIGHT;
int markernum;

static GdkImlibImage *im=NULL;
static GdkPixmap *pixmap=NULL;

static GtkItemFactory *itemf;

static gint id;
static gboolean state;
static GtkWidget *widget;


GdkPixmap *gb_pixmap = NULL;
GdkGC *gb_gc = NULL;
GdkGC *gb_gc1 = NULL;

void search();
double center(double ic,double i,int n);  //中心点を求める関数
double distance(double ic,double i,double jc,double j);  //中心点とマーカ候補の画素との距離を測る
void check(); //どれがどのマーカであるか調べる関数
unsigned char *reading(unsigned char*);
void fixednum(double Ab[],double Bb[],double Cb[],double pb[],double qb[],double ab[],double bb[],double *Tb);   //バターワースフィルターで用いる関数
void calculation(double mb[],double nb[],double Ab[],double Bb[],double Cb[],double pb[],double qb[]);            //バターワースフィルターで用いる関数
void com();    //バターワースフィルターで用いる関数
GdkImlibImage *get_next_frame(GdkImlibImage*);
gint configure_event(GtkWidget*, GdkEventConfigure*);
gint expose_event(GtkWidget*, GdkEventExpose*);
gint update_screen(GtkWidget*);
GdkImlibImage *get_current_image();
void refresh_init(GtkWidget*);
void video_barf();
void video_throw();
void video_fast_forwarding();
void video_back_turn();
void video_coordinate(GtkWidget *widget,gpointer data);
void video_original();
void video_noise();
void video_graph();
void graph_low_pass();
void graph_unstriated();
void graph_xt();
void graph_yt();
void graph_vt();
void graph_vxt();
void graph_vyt();
void graph_hist();
void graph_file();
void graph_file2(GtkWidget *widget,gpointer data);
GtkItemFactoryEntry *create_menu_items(gint *nitems);
GtkWidget *menu_init(GtkWidget *window);
GtkItemFactoryEntry *create_menu_items2(gint *nitems);
GtkWidget *menu_init2(GtkWidget *window);
GtkItemFactoryEntry *create_menu_items3(gint *nitems);
GtkWidget *menu_init3(GtkWidget *window);
GtkItemFactoryEntry *create_menu_items4(gint *nitems);
GtkWidget *menu_init4(GtkWidget *window);
GtkItemFactoryEntry *create_menu_items4(gint *nitems);
GtkWidget *menu_init4(GtkWidget *window);
GtkItemFactoryEntry *create_menu_items5(gint *nitems);
GtkWidget *menu_init5(GtkWidget *window);
void graph();
gint proc_draw(GtkWidget *widget, GdkEventConfigure *event);
gint proc_re_draw(GtkWidget *widget, GdkEventExpose *event);
void spin_sel();
int button_ctl(GtkWidget *widget, gpointer gdata);
void low_pass_filter();
void appeal();
void unstriated();
int draw_all(gpointer  draw);
GdkColor * set_res_color(int red,int green,int blue);
gint draw_all_vt(gpointer  draw);
gint draw_all_hist(gpointer  draw);
void toggle_refresh();
GtkItemFactoryEntry *create_menu_items1(gint *nitems);
GtkWidget *menu_init1(GtkWidget *window);
gint button_pressed(GtkWidget *widget, GdkEventButton *event);
void gui_config();
void save_file();
void save_file_cbf(GtkWidget*,gpointer);
unsigned char **memory(unsigned char **x);  //メモリの確保(**の場合)
unsigned char *memory(unsigned char *x);   //メモリの確保(*の場合)

void search(){
  int c;

  while((c=fgetc(fp1))!=EOF){
    if(c!='0') continue; 
    if((c=fgetc(fp1))!='0') continue; 
    if((c=fgetc(fp1))!='d') continue; 
    if((c=fgetc(fp1))=='b') break;
  }
  
  fseek(fp1,4,1);
}

double center(double ic,double i,int n){

  ic=((double)n*ic+i)/(double)(n+1);

  return ic;

}

double distance(double ic,double i,double jc,double j){

  return ((ic-i)*(ic-i)+(jc-j)*(jc-j));

}

void check(){
  int i,j;
  double d,e;
  
  for(i=0;i<MARKER;i++){
    for(j=i+1;j<MARKER-i;j++){
      if(distance(X[i],X0[i],Y[i],Y0[i])
	 >distance(X[i+j],X0[i],Y[i+j],Y0[i])){
	  d=X[i]; e=Y[i];
	  X[i]=X[i+j]; Y[i]=Y[i+j];
	  X[i+j]=d; Y[i+j]=e;
      }
    }
  }
}
  
unsigned char *reading(unsigned char *b){
  int c;
  long int k;
  int a,t,m,e,d[MAXMARKER],sum,A,q,f,h,Q=0,z;
  int i,j,n[MAXLINE],im,jh, adrs;
  double ic[MAXLINE],jc[MAXLINE];
  unsigned char **x;

  x=memory(x);  //メモリの確保
   
  for(i=0;i<dwHeight;i++)
    fread(x[i],sizeof(char),dwWidth,fp1);  //fp1からデータ読み込み

  if(FRAME1==0){
    for(kido=0;kido<=COLORNUM;kido++)
      P[kido]=0;
    for(i=0;i<dwHeight;i++){
      for(j=0;j<dwWidth;j++){
	if(FRAME1==0) P[x[i][j]]++;  //輝度分布を作る
      }
    }
  }

  sum=0;
  e=COLORNUM; f=0;

  if(FRAME1==0){
    for(kido=COLORNUM;kido>=0;kido--){
      if(sum<PIXELNUM*markernum)
	sum=sum+P[kido];               //マーカを構成する画素が合計何個か数える
       else                           //kidoには閾値が入る
	 break;
    }
  }

  if(N1==OP_ORIGINAL){               //白黒画像をカラー表示するGTK操作
    k=0;
    for(i=dwHeight-1;i>=0;i--)
      for(j=0;j<dwWidth;j++){
	b[k]=(unsigned char)x[i][j];
	b[k+1]=(unsigned char)x[i][j];
	b[k+2]=(unsigned char)x[i][j];  //R,G,Bの3色作ってマーカの輝度を構成
	k+=3;
      }
  }
  else if(N1==OP_NOISE){            //ノイズ処理をするGTK操作
    k=0;
    for(i=dwHeight-1;i>=0;i--){ 
      for(j=0;j<dwWidth;j++){
	t=0;
	for(h=-1;h<=1;h++)
	  for(m=-1;m<=1;m++){
	    im=i+m;
	    jh=j+h;                     //ターゲットの8近傍の画素の輝度を調べる
	    if(im<dwHeight&&0<=im&&jh<dwWidth&&0<=jh)
	      if(x[im][jh]<kido)
		t++;    //ターゲットの8近傍であり,マーカの輝度範囲内でないとき,
	  }             //tを増やしていく
	if(t<=5){
	  b[k]=e;
	  b[k+1]=e;            //4つ以上マーカの輝度範囲内であれば,
	  b[k+2]=e;            //ターゲットをマーカであると確定する
	}
	else{
	  b[k]=f;
	  b[k+1]=f;
	  b[k+2]=f;            //その他を背景とする
	}
	k+=3;
      }
    }
  }
  else{
    k=0;
    for(i=0;i<dwHeight-1;i++){
      for(j=0;j<dwWidth;j++){
	t=0;
	for(h=-1;h<=1;h++)
	  for(m=-1;m<=1;m++){
	    im=i+m;
	    jh=j+h;
	    if(((im<dwHeight)&&(jh<dwWidth)&&(im>=0)&&(jh>=0) )
	       &&(x[im][jh]<kido))
	      t++;
	  }
	A=0;
	if(t<=5){
	  for(q=0;q<Q;q++){
	    if((distance(ic[q],i,jc[q],j))<1000){   
	      ic[q]=center(ic[q],i,n[q]);          
	      jc[q]=center(jc[q],j,n[q]);
	      n[q]++;
	      A=1;
	      break;
	    }
	  }
	if(A==0||Q==0){
	  n[Q]=1;
	  ic[Q]=i;
	  jc[Q]=j;      
	  Q++;          //Qはマーカ+ノイズの数になる
	  }
	} 
	b[k]=(unsigned char)e;
	b[k+1]=(unsigned char)e;
	b[k+2]=(unsigned char)e;
	k+=3;
      }
    }
    
    for(i=0;i<dwHeight;i++){
      free(x[i]);
    }
    free(x);

    for(i=0;i<=MARKER;i++){
      X0[i]=X[i];
      Y0[i]=Y[i];
    }
    MARKER=0;
    for(q=Q-1;q>=0;q--)
      if(n[q]>40){                                 //マーカを画素40以上と仮定
	    X[MARKER]=jc[q]; Y[MARKER]=ic[q]; 
	    //	    cout<<X[MARKER]<<" "<<Y[MARKER]<<endl;
	    MARKER++;                       //マーカであれば中心点を入れ換える
	  }

    if(FRAME1==0)
      for(i=0;i<MARKER;i++){
	X0[i]=X[i];
	Y0[i]=Y[i];
      }

    check();
    
    for(q=0;q<MARKER;q++){                   //以降マーカの表示
      for(m=0;m<=2;m++) d[m]=0;
      d[q%3]=COLORNUM;
      
      for(z=-5;z<=5;z++){
	adrs=3*(dwWidth*(dwHeight-1-((int)Y[q]+z))+(int)X[q]);
	if((adrs>=0)&&(adrs<(dwWidth)*(dwHeight-1)*3)){
	  b[adrs]=d[0];
	  b[adrs+1]=d[1];
	  b[adrs+2]=d[2];
	}
	
	if (z!=0){
	adrs=3*(dwWidth*(dwHeight-1-(int)Y[q])+(int)X[q]+z);
	if((adrs>=0)&&(adrs<(dwWidth)*(dwHeight-1)*3)){
	  b[adrs]=d[0];
	  b[adrs+1]=d[1];
	  b[adrs+2]=d[2];     //中心点から上下,左右に5個画素を足して表示
	}}                    //マーカ以外は白くして表示
      }
    }
  }
  return b;
}  

GdkImlibImage *get_next_frame(GdkImlibImage *current){
  GdkImlibImage* im;
  unsigned char *b;

  b=memory(b);
  
      
  if(FRAME1==dwTotalFrames){
    fseek(fp1,0,SEEK_SET);
    FRAME1=0;
  }
  search();

  if(N==OP_BACK){
    if(FRAME1>0)
      reading(b);
    FRAME1--;
    fseek(fp1,(-2)*(biSizeImage+8),1);
  }
  else{
    reading(b);
    FRAME1++;
  }

  if(current) gdk_imlib_kill_image(current);
  im=gdk_imlib_create_image_from_data(b,NULL,(gint)dwWidth,(gint)dwHeight);

  free(b);
  
  return im;
  
}

gint configure_event(GtkWidget *widget,GdkEventConfigure *event){
  gint w,h;
  
  w=widget->allocation.width;
  h=widget->allocation.height;
  gdk_imlib_render(im,w,h);

  if(pixmap) gdk_pixmap_unref(pixmap);
  pixmap=gdk_imlib_move_image(im);

  return TRUE;
}

gint expose_event(GtkWidget *widget,GdkEventExpose *event){
  
  if(pixmap) gdk_draw_pixmap(widget->window,
			     widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
			     pixmap,
			     event->area.x,event->area.y,
			     event->area.x,event->area.y,
			     event->area.width,event->area.height);

  return TRUE;
}

gint update_screen(GtkWidget *widget){
  GdkEventExpose e;
  
  e.type=GDK_EXPOSE;
  e.window=widget->window;
  e.area.x=0; e.area.y=0;
  e.area.width=widget->allocation.width;
  e.area.height=widget->allocation.height;
  e.send_event=0; e.count=0;

  if(e.window!=NULL){
    im=get_next_frame(im);

    configure_event(widget,NULL);
    expose_event(widget,&e);
  }

  return TRUE;
}

GdkImlibImage *get_current_image(){
  return im;
}


void refresh_init(GtkWidget *target){
  widget=target; state=TRUE;
  id=gtk_timeout_add(INTERVAL,(GtkFunction)update_screen,
		     (gpointer)target);
}

void video_barf(){
  N1=OP_BARF;
  toggle_refresh();
}

void video_throw(){
  N=OP_SLOW;
  toggle_refresh();
}

void video_fast_forwarding(){
  N=OP_FAST;
  toggle_refresh();
}

void video_back_turn(){
  N=OP_BACK;
  fseek(fp1,(-1)*(biSizeImage+16),1);
  toggle_refresh();
}

void video_coordinate(GtkWidget *widget,gpointer data){
  int i;

  printf("  (赤)   ,  (緑)   ,  (青)  , フレーム\n");
  for(i=0;i<MARKER;i++)
    printf("(%.0lf %.0lf),",X[i],Y[i]);
  printf(" %d\n",FRAME1);

  return;
}

void video_original(){
  N1=OP_ORIGINAL;
  toggle_refresh();
}

void video_noise(){
  N1=OP_NOISE;
  toggle_refresh();
}


void video_graph(){
  N=OP_GRAPH;
  graph();
}

void graph_low_pass(){
  N2=OP_LOW_PASS;
  N=OP_NOTHING;
  spin_sel();
}

void graph_unstriated(){
  N=OP_UNSTRIATED;
  graph();
}

void graph_vxt(){
  N=OP_VXT;
  graph();
}

void graph_xt(){
  N=OP_XT;
  graph();
}

void graph_yt(){
  N=OP_YT;
  graph();
}

void graph_vt(){
  N=OP_VT;
  graph();
}

void graph_vyt(){
  N=OP_VYT;
  graph();
}

void graph_hist(){
  N=OP_HIST;
  graph();
}

void graph_file(){
  N3=OP_WIDTH;
  spin_sel();
}

void graph_file2(GtkWidget *widget,gpointer data){
  int i;
  double vx[MAXMARKER],vx0[MAXMARKER],vy[MAXMARKER],vy0[MAXMARKER];
  //GtkSpinButton *spin;

  fseek(fp2,0,SEEK_SET);

  if(N!=OP_HIST){
    //HEIGHT=gtk_spin_button_get_value_as_int(spin);
    HEIGHT=gtk_spin_button_get_value_as_int((GtkSpinButton *)data);
    printf("高さは%dです\n",HEIGHT);
    N3=OP_NOTHING;
  }
  
  for(i=0;i<MARKER;i++){
    vx0[i]=0;
    vy0[i]=0;
  }
  for(FRAME2=0;FRAME2<dwTotalFrames;FRAME2++){
    appeal();
    for(i=0;i<MARKER;i++){
      X[i]=X[i]*(double)WIDTH/(double)dwWidth;
      Y[i]=Y[i]*(double)HEIGHT/(double)dwHeight;
    }
    if(FRAME2==0)
      for(i=0;i<MARKER;i++){
	X0[i]=X[i];
	Y0[i]=Y[i];
	if(N2==OP_LOW_PASS){
	  XF0[i]=X[i];
	  YF0[i]=Y[i];
	}
      }
    check();
    if(N2==OP_LOW_PASS)
      low_pass_filter();
    if(N==OP_VXT||N==OP_VYT){
      for(i=0;i<MARKER;i++){
	vx0[i]=vx[i];
	vy0[i]=vy[i];
	vx[i]=(X[i]-X0[i])*30;
	vy[i]=(Y[i]-Y0[i])*30;
	if(N2==OP_LOW_PASS){
	  vx[i]=(XF[i]-XF0[i])*30;
	  vy[i]=(YF[i]-YF0[i])*30;
	}
      }
    }
    if(N==OP_HIST)
      for(i=0;i<=COLORNUM;i++)
	fprintf(fp3,"%d %d\n",i,P[i]);
    else{
      fprintf(fp3,"%lf",(double)FRAME2/30.0);
      for(i=0;i<MARKER;i++){
	if(N2==OP_LOW_PASS){
	  if(N==OP_XT)
	    fprintf(fp3," %lf",XF[i]);
	  else if(N==OP_YT)
	    fprintf(fp3," %lf",YF[i]);
	  else if(N==OP_VXT)
	    fprintf(fp3," %lf",vx[i]);
	  else if(N==OP_VYT)
	    fprintf(fp3," %lf",vy[i]);
	}
	else if(N==OP_VXT)
	  fprintf(fp3," %lf",vx[i]);
	else if(N==OP_VYT)
	  fprintf(fp3," %lf",vy[i]);
	else if(N==OP_XT)
	  fprintf(fp3," %lf",X[i]);
	else if(N==OP_YT)
	  fprintf(fp3," %lf",Y[i]);
      }
      fprintf(fp3,"\n");
    }
  }
  printf("ファイル出力終了\n");

}
 
static GtkItemFactoryEntry default_menu_items[] = {
  {M_OP, NULL, NULL, 0, "<Branch>"},
  {M_OP_SEP1, NULL, NULL, 0, "<Separator>"},
     //{M_OP_UNSTRIATED, NULL, graph_unstriated, 0, NULL},
  {M_OP_LOW_PASS, NULL, graph_low_pass, 0, NULL},
  {M_EF, NULL, NULL, 0, "<Branch>"},
  {M_EF_SEP1, NULL, NULL, 0, "<Separator>"},
  {M_EF_XT, NULL, graph_xt, 0, NULL},
  {M_EF_YT, NULL, graph_yt, 0, NULL},
  {M_EF_VT, NULL, graph_vt, 0, NULL},
  {M_EF_HIST, NULL, graph_hist, 0, NULL}
};

GtkItemFactoryEntry *create_menu_items(gint *nitems){
  *nitems=sizeof(default_menu_items) / sizeof(default_menu_items[0]);
  return default_menu_items;
}

GtkWidget *menu_init(GtkWidget *window){
  GtkAccelGroup *acclgrp;
  GtkItemFactoryEntry *menu_items;
  gint nitems;

  acclgrp=gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window),acclgrp);
  itemf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acclgrp);

  menu_items=create_menu_items(&nitems);
  gtk_item_factory_create_items(itemf,nitems,menu_items,NULL);

  return gtk_item_factory_get_widget(itemf,"<main>");
}

static GtkItemFactoryEntry default_menu_items2[] = {
  {M_EF, NULL, NULL, 0, "<Branch>"},
  {M_EF_SEP1, NULL, NULL, 0, "<Separator>"},
  {M_EF_XT, NULL, graph_xt, 0, NULL},
  {M_EF_YT, NULL, graph_yt, 0, NULL},
  {M_EF_VT, NULL, graph_vt, 0, NULL}
};

GtkItemFactoryEntry *create_menu_items2(gint *nitems){
  *nitems=sizeof(default_menu_items2) / sizeof(default_menu_items2[0]);
  return default_menu_items2;
}

GtkWidget *menu_init2(GtkWidget *window){
  GtkAccelGroup *acclgrp;
  GtkItemFactoryEntry *menu_items;
  gint nitems;

  acclgrp=gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window),acclgrp);
  itemf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acclgrp);

  menu_items=create_menu_items2(&nitems);
  gtk_item_factory_create_items(itemf,nitems,menu_items,NULL);

  return gtk_item_factory_get_widget(itemf,"<main>");
}

static GtkItemFactoryEntry default_menu_items3[] = {
  {M_EF, NULL, NULL, 0, "<Branch>"},
  {M_EF_SEP1, NULL, NULL, 0, "<Separator>"},
  {M_EF_VXT, NULL, graph_vxt, 0, NULL},
  {M_EF_VYT, NULL, graph_vyt, 0, NULL}
};

GtkItemFactoryEntry *create_menu_items3(gint *nitems){
  *nitems=sizeof(default_menu_items3) / sizeof(default_menu_items3[0]);
  return default_menu_items3;
}

GtkWidget *menu_init3(GtkWidget *window){
  GtkAccelGroup *acclgrp;
  GtkItemFactoryEntry *menu_items;
  gint nitems;

  acclgrp=gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window),acclgrp);
  itemf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acclgrp);

  menu_items=create_menu_items3(&nitems);
  gtk_item_factory_create_items(itemf,nitems,menu_items,NULL);

  return gtk_item_factory_get_widget(itemf,"<main>");
}

static GtkItemFactoryEntry default_menu_items4[] = {
  {M_EF, NULL, NULL, 0, "<Branch>"},
  {M_EF_SEP1, NULL, NULL, 0, "<Separator>"},
  {M_EF_IN, NULL, spin_sel, 0, NULL},
  {M_EF_FILE, NULL, graph_file2, 0, NULL},
};

GtkItemFactoryEntry *create_menu_items4(gint *nitems){
  *nitems=sizeof(default_menu_items4) / sizeof(default_menu_items4[0]);
  return default_menu_items4;
}

GtkWidget *menu_init4(GtkWidget *window){
  GtkAccelGroup *acclgrp;
  GtkItemFactoryEntry *menu_items;
  gint nitems;

  acclgrp=gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window),acclgrp);
  itemf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acclgrp);

  menu_items=create_menu_items4(&nitems);
  gtk_item_factory_create_items(itemf,nitems,menu_items,NULL);

  return gtk_item_factory_get_widget(itemf,"<main>");
}

static GtkItemFactoryEntry default_menu_items5[] = {
  {M_EF, NULL, NULL, 0, "<Branch>"},
  {M_EF_SEP1, NULL, NULL, 0, "<Separator>"},
  {M_EF_FILE, NULL, graph_file, 0, NULL}
};

GtkItemFactoryEntry *create_menu_items5(gint *nitems){
  *nitems=sizeof(default_menu_items5) / sizeof(default_menu_items5[0]);
  return default_menu_items5;
}

GtkWidget *menu_init5(GtkWidget *window){
  GtkAccelGroup *acclgrp;
  GtkItemFactoryEntry *menu_items;
  gint nitems;

  acclgrp=gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window),acclgrp);
  itemf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acclgrp);

  menu_items=create_menu_items5(&nitems);
  gtk_item_factory_create_items(itemf,nitems,menu_items,NULL);

  return gtk_item_factory_get_widget(itemf,"<main>");
}

void graph(){
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *mbar;
  AREA_X=dwTotalFrames+SPACE;
  AREA_Y=dwWidth+SPACE;

  if(AREA/AREA_X!=0)
    AREAX=AREA/AREA_X;
  AREA_X=AREAX*AREA_X;
  
  if(N==OP_HIST){
    AREAX=1;
    AREA_X=COLORNUM+SPACE;
  }
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT(window), "destroy",
		     GTK_SIGNAL_FUNC(gtk_grab_remove), NULL);

  if(N==OP_GRAPH)
    gtk_window_set_title(GTK_WINDOW(window),"グラフ");
  else if(N==OP_UNSTRIATED)
    gtk_window_set_title(GTK_WINDOW(window),"平滑化");
  else if(N2==OP_LOW_PASS){
    if(N==OP_VT)
      gtk_window_set_title(GTK_WINDOW(window),"V-tグラフ");
    else if(N==OP_XT)
      gtk_window_set_title(GTK_WINDOW(window),"X-tグラフ");
    else if(N==OP_YT)
      gtk_window_set_title(GTK_WINDOW(window),"Y-tグラフ");
    else if(N==OP_VXT)
      gtk_window_set_title(GTK_WINDOW(window),"Vx-tグラフ");
    else if(N==OP_VYT)
      gtk_window_set_title(GTK_WINDOW(window),"Vy-tグラフ");
    else if(N==OP_HIST)
      gtk_window_set_title(GTK_WINDOW(window),"ヒストグラム"); 
    else
      gtk_window_set_title(GTK_WINDOW(window),"low pass filter");
  }
  else if(N==OP_VT)
    gtk_window_set_title(GTK_WINDOW(window),"V-tグラフ");
  else if(N==OP_XT)
    gtk_window_set_title(GTK_WINDOW(window),"X-tグラフ");
  else if(N==OP_YT)
    gtk_window_set_title(GTK_WINDOW(window),"Y-tグラフ");
  else if(N==OP_VXT)
    gtk_window_set_title(GTK_WINDOW(window),"Vx-tグラフ");
  else if(N==OP_VYT)
    gtk_window_set_title(GTK_WINDOW(window),"Vy-tグラフ");
  else if(N==OP_HIST)
    gtk_window_set_title(GTK_WINDOW(window),"ヒストグラム");
  
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  if(N==OP_GRAPH){
    mbar=menu_init(window);
    gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
  }
  else if(N2==OP_LOW_PASS){
    if(N==OP_VT){
      mbar=menu_init3(window);
      gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
    }
    else if(N==OP_HIST){
      mbar=menu_init4(window);
      gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
    }
    else if(N==OP_XT||N==OP_YT||N==OP_VXT||N==OP_VYT){
      mbar=menu_init5(window);
      gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
    }
    else{
      mbar=menu_init2(window);
      gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
    }
  }
  else if(N==OP_VT){
    mbar=menu_init3(window);
    gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
  }
  else if(N==OP_HIST){
    mbar=menu_init4(window);
    gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
  }
  else if(N==OP_XT||N==OP_YT||N==OP_VXT||N==OP_VYT){
    mbar=menu_init5(window);
    gtk_box_pack_start(GTK_BOX(vbox),mbar,FALSE,FALSE,0);
  }
  
  drawing_area = gtk_drawing_area_new();
  gtk_drawing_area_size(GTK_DRAWING_AREA(drawing_area),
			AREA_X, AREA_Y);
  gtk_signal_connect(GTK_OBJECT(drawing_area),"expose_event",
		      (GtkSignalFunc)proc_re_draw, NULL);
  gtk_signal_connect(GTK_OBJECT(drawing_area),"configure_event",
		     (GtkSignalFunc)proc_draw, NULL);
  gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);
  gtk_widget_set_events(drawing_area,GDK_EXPOSURE_MASK);
  gtk_widget_show_all(window);
}

gint proc_draw(GtkWidget *widget,GdkEventConfigure *event){
  if(gb_pixmap != NULL)
    gdk_pixmap_unref(gb_pixmap);
  else{
    if(N!=OP_VYT)
      gb_gc = gdk_gc_new(widget->window);
    gb_gc1 = gdk_gc_new(widget->window);
  }
    
  gb_pixmap = gdk_pixmap_new(widget->window, widget->allocation.width,
			      widget->allocation.height, -1);

  if(N==OP_GRAPH)
    draw_all(widget);
  else if(N==OP_UNSTRIATED)
    draw_all(widget);
  else if(N2==OP_LOW_PASS){
    if(N==OP_VT)
      draw_all_vt(widget);
    else if(N==OP_XT)
      draw_all(widget);
    else if(N==OP_YT)
      draw_all(widget); 
    else if(N==OP_VXT)
      draw_all_vt(widget);
    else if(N==OP_VYT)
      draw_all_vt(widget);
    else if(N==OP_HIST)
      draw_all_hist(widget);
    else
      draw_all(widget);
  }
  else if(N==OP_VT)
    draw_all_vt(widget);
  else if(N==OP_XT)
    draw_all(widget);
  else if(N==OP_YT)
    draw_all(widget); 
  else if(N==OP_VXT)
    draw_all_vt(widget);
  else if(N==OP_VYT)
    draw_all_vt(widget);
  else if(N==OP_HIST)
    draw_all_hist(widget);
  

  return(FALSE);
}

gint proc_re_draw(GtkWidget *widget,GdkEventExpose *event){
  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  gb_pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height );

  return(FALSE);
}

void spin_sel(){
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *spin;
  GtkWidget *button;
  GtkAdjustment *adjust;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT(window), "destroy",
		     GTK_SIGNAL_FUNC(gtk_grab_remove), NULL);
  gtk_widget_set_usize(window, 200,50);

  if(N3==OP_WIDTH)
    gtk_window_set_title(GTK_WINDOW(window),"幅入力");
  else if(N3==OP_HEIGHT)
    gtk_window_set_title(GTK_WINDOW(window),"高さ入力");
  else if(N2==OP_LOW_PASS){
    if(N==OP_HIST)
      gtk_window_set_title(GTK_WINDOW(window),"輝度入力");
    else
      gtk_window_set_title(GTK_WINDOW(window),"周波数入力(Hz)");
  }
  else if(N==OP_HIST)
    gtk_window_set_title(GTK_WINDOW(window),"輝度入力");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
  adjust = (GtkAdjustment *)gtk_adjustment_new
    (0.0, 0.0, 200.0, 1.0, 10.0, 0.0);  
  spin= gtk_spin_button_new(adjust, 0, 0);

  gtk_signal_connect(GTK_OBJECT(spin), "changed",
		      GTK_SIGNAL_FUNC(NULL), NULL );
  gtk_box_pack_start(GTK_BOX(vbox), spin, FALSE, FALSE, 0);
  gtk_widget_show(spin); 

  button = gtk_button_new_with_label("O K !");
  gtk_box_pack_end(GTK_BOX(vbox), button, FALSE, FALSE, 0);
  if(N3==OP_HEIGHT)
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(graph_file2), spin);
  else
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
		       GTK_SIGNAL_FUNC(button_ctl), spin);
  gtk_widget_show_all(window);
}

int button_ctl(GtkWidget *widget, gpointer data)
{
  if(N==OP_HIST){
    printf("前の輝度は%dです\n",kido);
    kido=gtk_spin_button_get_value_as_int( (GtkSpinButton *)data );
    printf("変更した輝度は%dです\n",kido);
  }
  else if(N3==OP_WIDTH){
    WIDTH=gtk_spin_button_get_value_as_int((GtkSpinButton *)data);
    printf("幅は%dです\n",WIDTH);
    N3=OP_HEIGHT;
    spin_sel();
  }
  else if(N2==OP_LOW_PASS){
    tau=gtk_spin_button_get_value_as_int( (GtkSpinButton *)data );
    printf("周波数は%dです\n",tau);
    graph();
  }
    
  return( TRUE );
}


// 以降バターワースフィルター 

void com()
{
  int i;
  double r,d;
  double_complex C1[MAXNUM];

  r=M_PI;
  for(i=0;i<MAXNUM;i++) {
    d=(double)(2*i+7)/12.0;
    R[i]=cos(d*r);
    I[i]=sin(d*r);
    C1[i]=double_complex(R[i],I[i]);
  }
  
  E[0]=1.0/((C1[0]-C1[1])*(C1[0]-C1[2])*(C1[0]-C1[3])*(C1[0]-C1[4])*(C1[0]-C1[5]));
  E[1]=1.0/((C1[1]-C1[0])*(C1[1]-C1[2])*(C1[1]-C1[3])*(C1[1]-C1[4])*(C1[1]-C1[5]));
  E[2]=1.0/((C1[2]-C1[0])*(C1[2]-C1[1])*(C1[2]-C1[3])*(C1[2]-C1[4])*(C1[2]-C1[5]));
  E[3]=1.0/((C1[3]-C1[0])*(C1[3]-C1[1])*(C1[3]-C1[2])*(C1[3]-C1[4])*(C1[3]-C1[5])); 
  E[4]=1.0/((C1[4]-C1[0])*(C1[4]-C1[1])*(C1[4]-C1[2])*(C1[4]-C1[3])*(C1[4]-C1[5]));
  E[5]=1.0/((C1[5]-C1[0])*(C1[5]-C1[1])*(C1[5]-C1[2])*(C1[5]-C1[3])*(C1[5]-C1[4]));

}

/* 伝達関数H(s)の定数を計算 */
void fixednum(double Ab[],double Bb[],double Cb[],double pb[],double qb[],double ab[],double bb[],double *Tb)
{
  int i;
  pb[1]=real(E[0]); pb[2]=real(E[1]); pb[3]=real(E[2]);
  qb[1]=imag(E[0]); qb[2]=imag(E[1]); qb[3]=imag(E[2]);
  ab[1]=R[0]; ab[2]=R[1]; ab[3]=R[2];
  bb[1]=I[0]; bb[2]=I[1]; bb[3]=I[2];
  for(i=1;i<NUM;i++){
    Ab[i]=2*cos(bb[i]*(*Tb))*exp(ab[i]*(*Tb));
    Bb[i]=exp(2*ab[i]*(*Tb));
    Cb[i]=2*(pb[i]*cos(bb[i]*(*Tb))+qb[i]*sin(bb[i]*(*Tb)))*exp(ab[i]*(*Tb));
  }
}

/* sに関する伝達関数をH(z)に関する伝達関数に近似したときの定数を計算 */
void calculation(double mb[],double nb[],double Ab[],double Bb[],double Cb[],double pb[],double qb[])
{
  int i;

  mb[0]=0;
  mb[1]=0;
  for(i=1;i<NUM;i++){
    mb[0] += pb[i];
    mb[1] += Cb[i];
  }
  mb[0]=2*mb[0];
  mb[1]=2*(pb[1]*(Ab[2]+Ab[3])+pb[2]*(Ab[1]+Ab[3])+pb[3]*(Ab[1]+Ab[2]))+mb[1];
  mb[2]=2*(pb[1]*(Ab[2]*Ab[3]+Bb[2]+Bb[3])+pb[2]*(Ab[1]*Ab[3]+Bb[1]+Bb[3])+pb[3]*(Ab[1]*Ab[2]+Bb[1]+Bb[2]))+Cb[1]*(Ab[2]+Ab[3])+Cb[2]*(Ab[1]+Ab[3])+Cb[3]*(Ab[1]+Ab[2]);
  mb[3]=2*(pb[1]*(Ab[2]*Bb[3]+Ab[3]*Bb[2])+pb[2]*(Ab[1]*Bb[3]+Ab[3]*Bb[1])+pb[3]*(Ab[1]*Bb[2]+Ab[2]*Bb[1]))+Cb[1]*(Ab[2]*Ab[3]+Bb[2]+Bb[3])+Cb[2]*(Ab[1]*Ab[3]+Bb[1]+Bb[3])+Cb[3]*(Ab[1]*Ab[2]+Bb[1]+Bb[2]);
  mb[4]=2*(pb[1]*Bb[2]*Bb[3]+pb[2]*Bb[1]*Bb[3]+pb[3]*Bb[1]*Bb[2])+Cb[1]*(Ab[2]*Bb[3]+Ab[3]*Bb[2])+Cb[2]*(Ab[1]*Bb[3]+Ab[3]*Bb[1])+Cb[3]*(Ab[1]*Bb[2]+Ab[2]*Bb[1]);
  mb[5]=Cb[1]*Bb[2]*Bb[3]+Cb[2]*Bb[1]*Bb[3]+Cb[3]*Bb[1]*Bb[2];

  nb[1]=0;
  nb[2]=0;
  for(i=1;i<NUM;i++){
    nb[1] += Ab[i];
    nb[2] += Bb[i];
  }
  nb[2]=Ab[1]*Ab[2]+Ab[2]*Ab[3]+Ab[3]*Ab[1]+nb[2];
  nb[3]=Ab[1]*Bb[2]+Ab[1]*Bb[3]+Ab[2]*Bb[1]+Ab[2]*Bb[3]+Ab[3]*Bb[1]+Ab[3]*Bb[2]+Ab[1]*Ab[2]*Ab[3];
  nb[4]=Ab[1]*Ab[2]*Bb[3]+Ab[1]*Ab[3]*Bb[2]+Ab[2]*Ab[3]*Bb[1]+Bb[1]*Bb[2]+Bb[2]*Bb[3]+Bb[3]*Bb[1];
  nb[5]=Ab[1]*Bb[2]*Bb[3]+Ab[2]*Bb[1]*Bb[3]+Ab[3]*Bb[1]*Bb[2];
  nb[6]=Bb[1]*Bb[2]*Bb[3];
}

void low_pass_filter(){
  int i,sum;
  double pai,fb,Tb;
  static double xb[6][3],xb2[6][3],yb[7][3],yb2[7][3];
  static int IN=1;
  double Ab[NUM],Bb[NUM],Cb[NUM],pb[NUM],qb[NUM],ab[NUM],bb[NUM],mb[6],nb[7];

  pai=M_PI;
  fb=40;  //サンプリング周波数…１秒間に何フレームか 本実験では１秒間に40フレーム
  Tb=(tau*2*pai)/fb;

  com();
  fixednum(Ab,Bb,Cb,pb,qb,ab,bb,&Tb);
  calculation(mb,nb,Ab,Bb,Cb,pb,qb);

  for(sum=0;sum<MARKER;sum++) {

    if(IN==1) {
      for(i=0;i<6;i++){
        xb[i][sum]=X[sum];
        xb2[i][sum]=Y[sum];
      }
      for(i=0;i<7;i++){
        yb[i][sum]=X[sum];
        yb2[i][sum]=Y[sum];
      }
    }
    if(IN==2) {
      xb[0][sum]=X[sum];
      xb2[0][sum]=Y[sum];
    }

/* 時間系列に書き直した出力関数 */    
    yb[0][sum]=Tb*(mb[0]*xb[0][sum]-mb[1]*xb[1][sum]+mb[2]*xb[2][sum]-mb[3]*xb[3][sum]+mb[4]*xb[4][sum]-mb[5]*xb[5][sum])+nb[1]*yb[1][sum]-nb[2]*yb[2][sum]+nb[3]*yb[3][sum]-nb[4]*yb[4][sum]+nb[5]*yb[5][sum]-nb[6]*yb[6][sum];

    yb2[0][sum]=Tb*(mb[0]*xb2[0][sum]-mb[1]*xb2[1][sum]+mb[2]*xb2[2][sum]-mb[3]*xb2[3][sum]+mb[4]*xb2[4][sum]-mb[5]*xb2[5][sum])+nb[1]*yb2[1][sum]-nb[2]*yb2[2][sum]+nb[3]*yb2[3][sum]-nb[4]*yb2[4][sum]+nb[5]*yb2[5][sum]-nb[6]*yb2[6][sum];
 
    if(tau!=0){
      XF[sum]=yb[0][sum];  
      YF[sum]=yb2[0][sum]; 
    }
    for(i=4;i>=0;i--){
      xb[i+1][sum]=xb[i][sum];
      xb2[i+1][sum]=xb2[i][sum];
    }
    for(i=5;i>=0;i--){
      yb[i+1][sum]=yb[i][sum];
      yb2[i+1][sum]=yb2[i][sum];
    }

  }
  if(IN==1) IN=2;
}

/* バターワースフィルターここまで */

void appeal(){
  int c;
  int a,t,m,k,A,q,Q=0;
  int i,j,n[MAXLINE];
  double ic[MAXLINE],jc[MAXLINE];
  unsigned char **b;

  b=memory(b);

  while((c=fgetc(fp2))!=EOF){
    if(c!='0') continue; 
    if((c=fgetc(fp2))!='0') continue; 
    if((c=fgetc(fp2))!='d') continue; 
    if((c=fgetc(fp2))=='b') break;
  }
  fseek(fp2,4,1);
  
  for(i=0;i<dwHeight;i++)
    fread(b[i],sizeof(char),dwWidth,fp2);
 
  for(i=1;i<dwHeight-1;i++){
    for(j=1;j<dwWidth-1;j++){
      t=0;
      for(k=-1;k<=1;k++){
	for(m=-1;m<=1;m++){
	  if(b[i+m][j+k]<kido){
	    t++;
	  }
	}
      }

      A=0;
      if(t<=5){
	for(q=0;q<Q;q++){
	  if((distance(ic[q],i,jc[q],j))<1000){
	    ic[q]=center(ic[q],i,n[q]);
	    jc[q]=center(jc[q],j,n[q]);
	    n[q]++;
	    A=1;
	    break;
	  }
	}
	if(A==0||Q==0){
	  n[Q]=1;
	  ic[Q]=i;
	  jc[Q]=j;
	  Q++;
	}
      }
    }    
  }

  for(i=0;i<=MARKER;i++){
    X0[i]=X[i];
    Y0[i]=Y[i];
    if(N2==OP_LOW_PASS){
      XF0[i]=XF[i];
      YF0[i]=YF[i];
    }
  }
  
  MARKER=0;
  for(q=Q-1;q>=0;q--)
    if(n[q]>40){
      X[MARKER]=jc[q]; Y[MARKER]=ic[q];
      MARKER++;
    }
  
  for(i=0;i<dwHeight;i++){
    free(b[i]);
  }
  free(b);
}

void unstriated(){
  double x0[MAXMARKER],x[MAXMARKER],x1[MAXMARKER];
  double y0[MAXMARKER],y[MAXMARKER],y1[MAXMARKER];
  int i;

  if(FRAME2==0){
      appeal();
      for(i=0;i<MARKER;i++){
	X0[i]=X[i];
	Y0[i]=Y[i];
      }
    }
  else if(FRAME2==1){
      appeal();
      for(i=0;i<MARKER;i++){
	x0[i]=X0[i];
	y0[i]=Y0[i];
	x[i]=X[i];
	y[i]=Y[i];
      }
      appeal();
      for(i=0;i<MARKER;i++){
	x1[i]=X[i];
	y1[i]=Y[i];
	X[i]=(x0[i]+x[i]+x1[i])/3;
	Y[i]=(y0[i]+y[i]+y1[i])/3;
	X0[i]=x0[i];
	x0[i]=X[i];
	x[i]=x1[i];
	Y0[i]=y0[i];
	y0[i]=Y[i];
	y[i]=y1[i];
     }
  }
  else if(FRAME2==dwTotalFrames-1){
    for(i=0;i<MARKER;i++){
      X0[i]=x0[i];
      Y0[i]=y0[i];
      X[i]=x[i];
      Y[i]=y[i];
    }
  }
  else{
    appeal();
    for(i=0;i<MARKER;i++){
      x1[i]=X[i];
      y1[i]=Y[i];
      X[i]=(x0[i]+x[i]+x1[i])/3;
      Y[i]=(y0[i]+y[i]+y1[i])/3;
      X0[i]=x0[i];
      x0[i]=X[i];
      x[i]=x1[i];
      Y0[i]=y0[i];
      y0[i]=Y[i];
      y[i]=y1[i];
    }
  }
}

int draw_all(gpointer draw){
  GtkWidget    *draw_area = (GtkWidget *)draw;
  GdkRectangle update_rect;
  GdkFont* gfont;
  int a,k,i,j;
  int m=50;
  char buf[4];
  int c[MAXMARKER];
	     
  fseek(fp2,0,SEEK_SET);
    
  gfont=gdk_font_load("-alias-fixed-medium-r-normal--14-*-iso8859-1");
  gdk_draw_rectangle(gb_pixmap, draw_area->style->white_gc,
		       1, 0, 0, AREA_X, AREA_Y);
  gdk_gc_set_foreground(gb_gc, set_res_color(0,0,0));
  gdk_gc_set_background(gb_gc , set_res_color(255,255,255));

  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		0, dwWidth, AREA_X, dwWidth);
  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		SPACE, 0, SPACE, AREA_Y);
  gdk_draw_text(gb_pixmap, gfont,			       
		draw_area->style->black_gc,	   
		SPACE-10, dwWidth+12, "0", strlen("0"));
  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		SPACE, dwWidth-1, SPACE, dwWidth+1);
  k=50;
  while(k<=AREA_X){
    gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		  SPACE+k*AREAX, dwWidth-1, SPACE+k*AREAX, dwWidth+1);
    gdk_draw_text(gb_pixmap, gfont,			       
		  draw_area->style->black_gc,SPACE+k*AREAX,
		  dwWidth+12, gcvt(m,5,buf),
		  strlen(gcvt(m,5,buf)));
    if(k<=AREA_Y){
      gdk_draw_line(gb_pixmap,draw_area->style->black_gc,SPACE-1,
		    (AREA_Y-k-SPACE), SPACE+1, (AREA_Y-k-SPACE));
      gdk_draw_text(gb_pixmap, gfont,			       
		    draw_area->style->black_gc,	   
		    SPACE-25, (AREA_Y-k-SPACE), gcvt(m,5,buf),
		    strlen(gcvt(m,5,buf)));
    }
    k=k+50;
    m=m+50;
  }
  
  for(FRAME2=0;FRAME2<dwTotalFrames;FRAME2++){
    if(N==OP_UNSTRIATED)
      unstriated();
    else{
      appeal();
      if(FRAME2==0)
	for(i=0;i<MARKER;i++){
	  X0[i]=X[i];
	  Y0[i]=Y[i];
	  if(N2==OP_LOW_PASS){
	    XF0[i]=X[i];
	    YF0[i]=Y[i];
	  }
	}
      check();
      if(N2==OP_LOW_PASS)
	low_pass_filter();
    }
    
    for(i=0;i<=MARKER;i++)
      c[i]=0;
    a=0;
    for(i=0;i<MARKER;i++){
      c[a]=COLORNUM;
      if(FRAME2!=0){
	if(N!=OP_YT){
	  gdk_gc_set_foreground(gb_gc , set_res_color(c[0],c[1],c[2]));
	  if(tau==0||N2!=OP_LOW_PASS)
	    gdk_draw_line(gb_pixmap, gb_gc,(FRAME2-1)*AREAX+SPACE,
			  (dwWidth-(int)X0[i]),
			  FRAME2*AREAX+SPACE, (dwWidth-(int)X[i]));
	  else if(N2==OP_LOW_PASS)
	    gdk_draw_line(gb_pixmap, gb_gc,(FRAME2-1)*AREAX+SPACE,
			  (dwWidth-(int)XF0[i]),
			  FRAME2*AREAX+SPACE, (dwWidth-(int)XF[i]));
	}
	if(N!=OP_XT){
	  gdk_gc_set_foreground(gb_gc1 , set_res_color(c[0],c[1],c[2]));
	  gdk_gc_set_line_attributes(gb_gc1, 2, GDK_LINE_SOLID,
				     GDK_CAP_ROUND, GDK_JOIN_ROUND);
	  if(tau==0||N2!=OP_LOW_PASS)
	    gdk_draw_line(gb_pixmap, gb_gc1,(FRAME2-1)*AREAX+SPACE,
			  dwWidth-(int)Y0[i],
			  FRAME2*AREAX+SPACE, dwWidth-(int)Y[i]);
	  else if(N2==OP_LOW_PASS)
	    gdk_draw_line(gb_pixmap, gb_gc1,(FRAME2-1)*AREAX+SPACE,
			  dwWidth-(int)YF0[i],
			  FRAME2*AREAX+SPACE, dwWidth-(int)YF[i]);
	}
      }
      for(j=0;j<MARKER;j++)
	c[j]=0;
      a++;
      //      if(a%3==0)
      //	a=0;
      a%=3;
    }
  }

  update_rect.x = 0;
  update_rect.y = 0;
  update_rect.width = AREA_X;
  update_rect.height = AREA_Y;
  gtk_widget_draw(draw_area, &update_rect);

  return(0);
}

GdkColor *set_res_color(int red,int green,int blue)
{
static  GdkColor    color;

  color.red    = red * (65535/255);
  color.green    = green * (65535/255);
  color.blue    = blue * (65535/255);
  color.pixel    = (gulong)(red*65536 + green*256 + blue);
  
  
  gdk_color_alloc( gdk_colormap_get_system(), &color );
  
  return(&color);
}

int draw_all_vt(gpointer draw){
  GtkWidget    *draw_area = (GtkWidget *)draw;
  GdkRectangle update_rect;
  GdkFont* gfont;
  double vx[MAXMARKER],vx0[MAXMARKER],vy[MAXMARKER],vy0[MAXMARKER];
  int a,i,j,k,m=0,n=50;
  char buf[4];
  int c[MAXMARKER];
  
  fseek(fp2,0,SEEK_SET);
  for(i=0;i<=MARKER;i++){
    vx0[i]=0;
    vy0[i]=0;
  }
    
  gfont=gdk_font_load("-alias-fixed-medium-r-normal--14-*-iso8859-1");
  gdk_draw_rectangle(gb_pixmap, draw_area->style->white_gc,
		       1, 0, 0, AREA_X, AREA_Y);
  gdk_gc_set_foreground(gb_gc, set_res_color(0,0,0));
  gdk_gc_set_background(gb_gc , set_res_color(255,255,255));

  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		0, AREA_Y/2, AREA_X, AREA_Y/2);
  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		SPACE, 0, SPACE, AREA_Y);
  gdk_draw_text(gb_pixmap, gfont,			       
		draw_area->style->black_gc,	   
		SPACE-10, AREA_Y/2+12, "0", strlen("0"));
  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		SPACE, AREA_Y/2-1, SPACE, AREA_Y/2+1);
  k=50;
  while(k<=AREA_X){
    gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		  SPACE+k*AREAX, AREA_Y/2-1, SPACE+k*AREAX, AREA_Y/2+1);
    gdk_draw_text(gb_pixmap, gfont,			       
		  draw_area->style->black_gc,	   
		  SPACE+k*AREAX, AREA_Y/2+12, gcvt((double)(m+50)/100,5,buf),
		  strlen(gcvt((double)(m+50)/100,5,buf)));
    if(m<=AREA_Y/2){
      gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		    SPACE-1, AREA_Y/2-m, SPACE+1, AREA_Y/2-m);
      gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		    SPACE-1, AREA_Y/2+m, SPACE+1, AREA_Y/2+m);
      gdk_draw_text(gb_pixmap, gfont,			       
		    draw_area->style->black_gc,	   
		    SPACE-25, AREA_Y/2-m-50, gcvt(n,5,buf),
		    strlen(gcvt(n,5,buf)));
      gdk_draw_text(gb_pixmap, gfont,			       
		    draw_area->style->black_gc,	   
		    SPACE-30, AREA_Y/2+m+50, gcvt(-1*n,5,buf),
		    strlen(gcvt(-1*n,5,buf)));
    }
    k=k+50;
    m=m+50;
    n=n+50;
  }
  
  for(FRAME2=0;FRAME2<dwTotalFrames;FRAME2++){
    appeal();
    if(FRAME2==0)
      for(i=0;i<MARKER;i++){
	X0[i]=X[i];
	Y0[i]=Y[i];
	if(N2==OP_LOW_PASS){
	  XF0[i]=X[i];
	  YF0[i]=Y[i];
	}
      }
    check();
    if(N2==OP_LOW_PASS)
      low_pass_filter();
    
    for(i=0;i<MARKER;i++){
      vx0[i]=vx[i];
      vy0[i]=vy[i];
      if(tau==0||N2!=OP_LOW_PASS){
	vx[i]=(X[i]-X0[i])*10+(double)AREA_Y/2;
	vy[i]=(Y[i]-Y0[i])*10+(double)AREA_Y/2;
      }
      else if(N2==OP_LOW_PASS){
	vx[i]=(XF[i]-XF0[i])*10+(double)AREA_Y/2;
	vy[i]=(YF[i]-YF0[i])*10+(double)AREA_Y/2;
      }
      c[i]=0;
    }
    a=0;
    for(i=0;i<MARKER;i++){
      c[a]=COLORNUM;
      if(N!=OP_VYT){
	gdk_gc_set_foreground(gb_gc , set_res_color(c[0],c[1],c[2]));
	if(FRAME2!=0)
	  gdk_draw_line(gb_pixmap, gb_gc,(FRAME2-1)*AREAX+SPACE,
			(AREA_Y/2-(int)vx0[i])+AREA_Y/2,
			FRAME2*AREAX+SPACE, (AREA_Y/2-(int)vx[i])+AREA_Y/2);
      }
      if(N!=OP_VXT){
	gdk_gc_set_foreground(gb_gc1 , set_res_color(c[0],c[1],c[2]));
	gdk_gc_set_line_attributes(gb_gc1, 2, GDK_LINE_SOLID,
				  GDK_CAP_ROUND, GDK_JOIN_ROUND);
	if(FRAME2!=0)
	  gdk_draw_line(gb_pixmap, gb_gc1,(FRAME2-1)*AREAX+SPACE,
			(AREA_Y/2-(int)vy0[i])+AREA_Y/2,
			FRAME2*AREAX+SPACE, (AREA_Y/2-(int)vy[i])+AREA_Y/2);
      }
      for(j=0;j<MARKER;j++)
	c[j]=0;
      a++;
      if(a%3==0)
	a=0;
    }
  }

  update_rect.x = 0;
  update_rect.y = 0;
  update_rect.width = AREA_X;
  update_rect.height = AREA_Y;
  gtk_widget_draw(draw_area, &update_rect);

  return(0);
}

int draw_all_hist(gpointer draw){
  GtkWidget    *draw_area = (GtkWidget *)draw;
  GdkRectangle update_rect;
  GdkFont* gfont;
  int k,i,m=50;
  char buf[4];
	     
  fseek(fp2,0,SEEK_SET);
  
  gfont=gdk_font_load("-alias-fixed-medium-r-normal--14-*-iso8859-1");
  gdk_draw_rectangle(gb_pixmap, draw_area->style->white_gc,
		       1, 0, 0, AREA_X, AREA_Y);
  gdk_gc_set_foreground(gb_gc, set_res_color(0,0,0));
  gdk_gc_set_background(gb_gc , set_res_color(255,255,255));

  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		0, dwWidth, AREA_X, dwWidth);
  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		SPACE, 0, SPACE, AREA_Y);
  gdk_draw_text(gb_pixmap, gfont,			       
		draw_area->style->black_gc,	   
		SPACE-10, dwWidth+12, "0", strlen("0"));
  gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		SPACE, dwWidth-1, SPACE, dwWidth+1);
  k=50;
  while(k<=AREA_X){
    gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		  SPACE+k*AREAX, dwWidth-1, SPACE+k*AREAX, dwWidth+1);
    gdk_draw_text(gb_pixmap, gfont,			       
		  draw_area->style->black_gc,SPACE+k*AREAX,
		  dwWidth+12, gcvt(m,5,buf), strlen(gcvt(m,5,buf)));
    if(k<=AREA_Y){
      gdk_draw_line(gb_pixmap,draw_area->style->black_gc,
		    SPACE-1, AREA_Y-k-SPACE, SPACE+1, AREA_Y-k-SPACE);
      gdk_draw_text(gb_pixmap, gfont,			       
		    draw_area->style->black_gc,	   
		    SPACE-25, AREA_Y-k-SPACE, gcvt(m,5,buf),
		    strlen(gcvt(m,5,buf)));
    }
    k=k+50;
    m=m+50;
  }
  
  for(i=1;i<=COLORNUM;i++){
    gdk_gc_set_foreground(gb_gc, set_res_color(0,0,0));
    gdk_draw_line(gb_pixmap, gb_gc,(i-1)*AREAX+SPACE,dwWidth-P[i-1],
		  i*AREAX+SPACE, dwWidth-P[i]);
  }

  update_rect.x = 0;
  update_rect.y = 0;
  update_rect.width = AREA_X;
  update_rect.height = AREA_Y;
  gtk_widget_draw(draw_area, &update_rect);

  return(0);
}

void toggle_refresh(){

  if(state){
    gtk_timeout_remove(id);
    N=OP_NOTHING;
  }
  else if(N==OP_SLOW) id=gtk_timeout_add(SLOW,(GtkFunction)update_screen,
					  (gpointer)widget);
  else if(N==OP_FAST) id=gtk_timeout_add(FAST,(GtkFunction)update_screen,
					 (gpointer)widget);
  else if(N==OP_BACK) id=gtk_timeout_add(INTERVAL,(GtkFunction)update_screen,
					 (gpointer)widget);
  else if(N1==OP_ORIGINAL)
    id=gtk_timeout_add(INTERVAL,(GtkFunction)update_screen,
		       (gpointer)widget);
  else if(N1==OP_NOISE) id=gtk_timeout_add(INTERVAL,(GtkFunction)update_screen,
					  (gpointer)widget);
  else id=gtk_timeout_add(INTERVAL,(GtkFunction)update_screen,
			  (gpointer)widget);

  state = state ? FALSE : TRUE;
}

static GtkItemFactoryEntry default_menu_items1[] = {
  {M_EF, NULL, NULL, 0, "<Branch>"},
  {M_EF_SEP1, NULL, NULL, 0, "<Separator>"},
  {M_EF_ORIGINAL, NULL, video_original, 0, NULL},
  {M_EF_NOISE, NULL, video_noise, 0, NULL},
  {M_EF_BARF, NULL, video_barf, 0, NULL}
};

GtkItemFactoryEntry *create_menu_items1(gint *nitems){
  *nitems=sizeof(default_menu_items1) / sizeof(default_menu_items1[0]);
  return default_menu_items1;
}

GtkWidget *menu_init1(GtkWidget *window){
  GtkAccelGroup *acclgrp;
  GtkItemFactoryEntry *menu_items;
  gint nitems;

  acclgrp=gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window),acclgrp);
  itemf=gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",acclgrp);

  menu_items=create_menu_items1(&nitems);
  gtk_item_factory_create_items(itemf,nitems,menu_items,NULL);

  return gtk_item_factory_get_widget(itemf,"<main>");
}

gint button_pressed(GtkWidget *widget, GdkEventButton *event)
{
  double d;
  
  if(click==1){
    d=(X1-(event->x))*(X1-(event->x))+
      (Y1-dwHeight-(event->y))*(Y1-dwHeight-(event->y));
    printf("%.0f,%.0f\n", event->x, dwHeight-(event->y));
    printf("%.0f,%.0fと%.0f,%.0fとの間の距離は%.0fです。\n",
	   X1,Y1,event->x,dwHeight-(event->y),sqrt(d));
    click=0;
  }
  else{
    printf("%.0f,%.0f\n", event->x, dwHeight-(event->y));
    X1=event->x;
    Y1=dwHeight-(event->y);
    click=1;
  }
  
  return TRUE;
}

void gui_config(){
  GtkWidget *window;
  GtkWidget *drawing_area;
  GtkWidget *hbox;
  GtkWidget *vbox;
  GtkWidget *vbox1;
  GtkWidget *button;
  GtkWidget *mbar;
  GtkWidget *aframe;
 
  window=gtk_window_new(GTK_WINDOW_TOPLEVEL); 
  gtk_signal_connect(GTK_OBJECT(window),"destroy",
		     GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

  drawing_area=gtk_drawing_area_new();
  gtk_drawing_area_size(GTK_DRAWING_AREA(drawing_area),dwWidth,dwHeight);
  
  vbox1=gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(window),vbox1);
  mbar=menu_init1(window);
  gtk_box_pack_start(GTK_BOX(vbox1),mbar,FALSE,FALSE,0);
  
  hbox=gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox1),hbox,TRUE,TRUE,0);

  aframe=gtk_aspect_frame_new(NULL,0.5,0.5,1.0,TRUE);
  gtk_frame_set_shadow_type(GTK_FRAME(aframe),GTK_SHADOW_NONE);
  gtk_container_add(GTK_CONTAINER(aframe),drawing_area);
  gtk_box_pack_start(GTK_BOX(hbox),aframe,TRUE,TRUE,0);
  
  vbox=gtk_vbox_new(FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox),vbox,TRUE,TRUE,0);
  
  button=gtk_button_new_with_label("停止・再生");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(toggle_refresh),NULL);
  
  button=gtk_button_new_with_label("スロー");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(video_throw),NULL);
  
  button=gtk_button_new_with_label("早送り");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(video_fast_forwarding),NULL);
					       
  button=gtk_button_new_with_label("巻き戻し");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(video_back_turn),NULL);

  button=gtk_button_new_with_label("座標");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(video_coordinate),NULL);

  button=gtk_button_new_with_label("グラフ");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(video_graph),NULL);
  
  button=gtk_button_new_with_label("保存");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(save_file),NULL);
  
  button=gtk_button_new_with_label("終了");
  gtk_box_pack_start(GTK_BOX(vbox),button,TRUE,TRUE,0);
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
		     GTK_SIGNAL_FUNC(gtk_main_quit),NULL);
  
  gtk_signal_connect (GTK_OBJECT (window), "button_press_event",
		      GTK_SIGNAL_FUNC(button_pressed), NULL);
  gtk_widget_set_events (window, GDK_BUTTON_PRESS_MASK);
  
  gtk_signal_connect(GTK_OBJECT(drawing_area),"expose_event",
		     GTK_SIGNAL_FUNC(expose_event),NULL);
  gtk_signal_connect(GTK_OBJECT(drawing_area),"configure_event",
		     GTK_SIGNAL_FUNC(configure_event),NULL);

  gtk_widget_set_events(drawing_area,GDK_EXPOSURE_MASK);

  gtk_widget_show_all(window);

  refresh_init(drawing_area);
}

void save_file(){
  GtkWidget *fselect;
  if(N==OP_FILE)
    fselect=gtk_file_selection_new("座標データの保存");
  else
    fselect=gtk_file_selection_new("画像データの保存");
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(fselect)->ok_button),
		     "clicked",GTK_SIGNAL_FUNC(save_file_cbf),fselect);
  gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(fselect)
				->cancel_button),
		     "clicked",GTK_SIGNAL_FUNC(save_file_cbf),fselect);
  gtk_signal_connect(GTK_OBJECT(fselect),
		     "destroy",GTK_SIGNAL_FUNC(gtk_grab_remove),fselect);

  gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(fselect));

  gtk_widget_show(fselect);
  gtk_grab_add(fselect);
}

void save_file_cbf(GtkWidget *widget,gpointer data){
  GtkWidget *fselect=GTK_WIDGET(data);
  char *filename;
  GdkCursor *watch;

  if(widget==(GTK_FILE_SELECTION(fselect)->ok_button)){
    filename=gtk_file_selection_get_filename(GTK_FILE_SELECTION(fselect));
    watch=gdk_cursor_new(GDK_WATCH);
    gdk_window_set_cursor(fselect->window,watch);
    gdk_flush();
    gdk_imlib_save_image(get_current_image(),filename,NULL);
    gdk_cursor_destroy(watch);
  }
  gtk_widget_destroy(fselect);
}

FILE *getfp1(char* n)
{
    FILE *fp1;

    fp1=fopen(n,"rb");

    if(fp1==NULL){
        fprintf(stderr,"%s not found\n",n);

        exit(1);
    }
    return fp1;
}

FILE *getfp2(char* n)
{
    FILE *fp1;

    fp1=fopen(n,"w");

    if(fp1==NULL){
        fprintf(stderr,"%s not found\n",n);

        exit(1);
    }
    return fp1;
}

void setImgData(ShowRiff rr)
{
  dwWidth=rr.getWidth();
  dwHeight=rr.getHeight();
  dwTotalFrames=rr.getTotalFrames();
  biSizeImage=rr.getbiSizeImage();

}

unsigned char** memory(unsigned char** x)
{
  int i;
  x=(unsigned char**)calloc(dwHeight,sizeof(char *));
  
  if(x==NULL){
    printf("メモリが足りません。他のプログラムを終了して下さい。\n");
    exit(0);
  }
  
  for(i=0;i<dwHeight;i++){
    x[i]=(unsigned char*)calloc(dwWidth,sizeof(char));
    if(x[i]==NULL){
      printf("メモリが足りません。他のプログラムを終了して下さい。\n");
      exit(0);
    }   
  }
  return(x);
}

unsigned char* memory(unsigned char *x){
  
  x=(unsigned char *)calloc(3*biSizeImage,sizeof(unsigned char));

  if(x==NULL){
    printf("メモリが足りません。他のプログラムを終了して下さい。\n");
    exit(0);
  }
  return(x);
}


int main(gint argc, gchar *argv[]){

  if(argc!=4){
    printf("Usage:moca <file name> <file name> <marker number>\n");
    exit(1);
  }

  
  ShowRiff rr(argv[1]);

  gtk_set_locale();
  gtk_init(&argc,&argv);
  gdk_imlib_init();

  setImgData(rr);

  fp1=getfp1(argv[1]);
  fp2=getfp1(argv[1]);
  fp3=getfp2(argv[2]);

  markernum=atoi(argv[3]);
  
  gui_config();
  gtk_main();
  
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);

  return 0;
}

