#ifndef DRAW_H
#define DRAW_H
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/fb.h>
#include<sys/mman.h>
#include<unistd.h>
#include"bmath.h"
char*pt_memBuffer;
char*pt_buffer;
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
int bytes_w,width_window,height_window,x_window,y_window;

void initWindow(int x,int y,int w,int h);
void drawLine_m(int x0,int y0,int x1,int y1);
void drawLine_dda_float(int x0,int y0,int x1,int y1);
void drawLine_dda_fixed(int x0,int y0,int x1,int y1);
void drawLine_brshm_float(int x0,int y0,int x1,int y1);
void drawLine_brshm(int x0,int y0,int x1,int y1);
void drawCircle_dda(int x0,int y0,int r,char color);
void drawTriangle(POINT4D_PTR pt_v0,POINT4D_PTR pt_v1,POINT4D_PTR pt_v2);
#endif
