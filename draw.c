//Assume:bits_perPixel=32768
//count memBuffer_size using 768*5504,无视bytes_w了。
//注意，draw.c的版本是全屏版本
#include<math.h>
#include"bmath.h"
#include"draw.h"
#include <run.h>
#include"../include/fixed_digit.h"
#define LOWPOINT_X y_round
#define LOWPOINT_Y x_round
#define DRAW_PIXEL(x,y,color) *(pt_memBuffer+((x)<<2)+(y)*bytes_w)=color;
//BUG:ignore overflow,x should be positive

void initWindow(int x,int y,int w,int h){
	x_window=x;
	y_window=y;
	width_window=w;
	height_window=h;
	int fbfd=open("/dev/fb0",O_RDWR);
	if(fbfd==-1){
		printf("can not open /dev/fb0\n");
		exit(-1);
	}
	if(ioctl(fbfd,FBIOGET_FSCREENINFO,&finfo)==-1){
		printf("can not access fixed information\n");
		exit(-1);
	}
	if(ioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo)==-1){
		printf("can not access var information\n");
		exit(-1);
	}
	struct fb_bitfield* red=&vinfo.red;
	struct fb_bitfield* green=&vinfo.green;
	struct fb_bitfield* blue=&vinfo.blue;
	bytes_w=finfo.line_length;
	printf("vinfo:\nred:offset:%u,length:%u,msb_right:%u\n",red->offset,red->length,red->msb_right);
	printf("green:offset:%u,length:%u,msb_right:%u\n",green->offset,green->length,green->msb_right);
	printf("blue:offset:%u,length:%u,msb_right:%u\n",blue->offset,blue->length,blue->msb_right);
	printf("bits_per_pixel:%d\n",vinfo.bits_per_pixel);
	printf("height:%u,width:%u\n",vinfo.height,vinfo.width);
	printf("xres:%u,yres:%u\n",vinfo.xres,vinfo.yres);
	printf("xres_virtual:%u,yres_virtual:%u\n",vinfo.xres_virtual,vinfo.yres_virtual);
	printf("xoffset:%u,yoffset:%u\n-----------------\n",vinfo.xoffset,vinfo.yoffset);
	//finfo	
	printf("finfo:\nid:%s",(char*)finfo.id);
	printf("smem_start:%x,smem_len:%u\n",(unsigned)finfo.smem_start,finfo.smem_len);
	printf("line_length:%u\n",bytes_w);
	//mmap now
	pt_buffer=mmap(NULL,finfo.smem_len,PROT_WRITE|PROT_READ,MAP_SHARED,fbfd,0);	
	if((int)pt_buffer==-1){
		printf("can not map framebuffer-device to user-space\n");
		exit(-1);
	}
	//	 init memBuffer now
	//	 memBuffer宽5504(vinfo.line_length),高768(vinfo.yres_virtual)。
	pt_memBuffer=malloc(768*5504);
	if(pt_memBuffer==NULL){
		printf("pt_memBuffer can not malloc");
		exit(0);
	}
	int grid=1;
}
void drawCircle_dda(int x0,int y0,int r,char color){
	double di=1/(double)r;
	double x=r;
	double y=0;
	*(pt_memBuffer+((int)(x+0.5)<<2)+(int)(y+0.5)*bytes_w)=color;
	int x_round,y_round;
	for(double i=0;i<=PI/4;i+=di){//the logical-starting radian should be di.
		x_round=(int)(x+0.5);
		y_round=(int)(y+0.5);
		*(pt_memBuffer+(x_round+x0<<2)+(y_round+y0)*bytes_w)=color;//1  即drawPixel(x_round+x0,y_round+y0)
		*(pt_memBuffer+(LOWPOINT_X+x0<<2)+(LOWPOINT_Y+y0)*bytes_w)=color;//2<-1 即drawPixel(LOWPOINT_X+x0,LOWpOINT_Y+y0),下面类似
		*(pt_memBuffer+(-LOWPOINT_X+x0<<2)+(LOWPOINT_Y+y0)*bytes_w)=color;//3<-2
		*(pt_memBuffer+(-x_round+x0<<2)+(y_round+y0)*bytes_w)=color;//4<-1
		*(pt_memBuffer+(-x_round+x0<<2)+(-y_round+y0)*bytes_w)=color;//5<-1
		*(pt_memBuffer+(-LOWPOINT_X+x0<<2)+(-LOWPOINT_Y+y0)*bytes_w)=color;//6<-2
		*(pt_memBuffer+(LOWPOINT_X+x0<<2)+(-LOWPOINT_Y+y0)*bytes_w)=color;//7<-2
		*(pt_memBuffer+(x_round+x0<<2)+(-y_round+y0)*bytes_w)=color;//8<-1
		x+=-sin(i);//di=1/r,dx=-sin(i);
		y+=cos(i);//di=1/r,dy=cos(i);
	}
//	printf("r:%d,di:%.3f,x:%.3f,x should be%.3f\n",r,1/(double)r,x,(r*cos(PI/4)));
}
void drawLine_dda_float(int x0,int y0,int x1,int y1){
	//1000*100次，66ms
	int x0_x1=x1-x0;
	int y0_y1=y1-y0;
	if((x0_x1>0?x0_x1:-x0_x1)>(y0_y1>0?y0_y1:-y0_y1)){
		//直线趋于水平，故沿x轴逐点描画
		//保证x0_x1指向x轴正方向
		if(x0_x1<0){
			int temp=x0;
			x0=x1;
			x1=temp;
			temp=y0;
			y0=y1;
			y1=temp;
		}
		float k=(float)y0_y1/(float)x0_x1;
		float y=y0;
		for(int x=x0;x<=x1;x++){
			*(pt_memBuffer+(x<<2)+(int)(y+0.5)*5504)=0xff;
			y+=k;
		}
	}
	else{
		//直线趋于竖直，故沿y轴逐点描画
		//确保y0_y1沿y正方向
		if(y0_y1<0){
			int temp=x0;
			x0=x1;
			x1=temp;
			temp=y0;
			y0=y1;
			y1=temp;
		}
		float dxdy=(float)x0_x1/(float)y0_y1;
		float x=x0;
		for(int y=y0;y<=y1;y++){
			*(pt_memBuffer+((int)(x+0.5)<<2)+y*5504)=0xff;
			x+=dxdy;
		}
	}
}

void drawTriangle(POINT4D_PTR pt_v0,POINT4D_PTR pt_v1,POINT4D_PTR pt_v2){
	int x0=(int)pt_v0->x;
	int y0=(int)pt_v0->y;
	int x1=(int)pt_v1->x;
	int y1=(int)pt_v1->y;
	int x2=(int)pt_v2->x;
	int y2=(int)pt_v2->y;
	drawLine_dda_fixed(x0,y0,x1,y1);
	drawLine_dda_fixed(x1,y1,x2,y2);
	drawLine_dda_fixed(x0,y0,x2,y2);
}
void drawLine_dda_fixed(int x0,int y0,int x1,int y1){
	int x0_x1=x1-x0;
	int y0_y1=y1-y0;
	if((x0_x1==0)&&(y0_y1==0)) return;//avoid division-overflow
	if((x0_x1>0?x0_x1:-x0_x1)>(y0_y1>0?y0_y1:-y0_y1)){
		//直线趋于水平，故沿x轴逐点描画
		//保证x0_x1指向x轴正方向
		if(x0_x1<0){
			int temp=x0;
			x0=x1;
			x1=temp;
			temp=y0;
			y0=y1;
			y1=temp;
		}
		fixed k=(INT_TO_FIXED(y1)-INT_TO_FIXED(y0))/(x1-x0);//使用顶点数来代替浮点数，是为了在下面的for循环里避开类型转换
		fixed y=INT_TO_FIXED(y0);
		for(int x=x0;x<=x1;x++){
			*(pt_memBuffer+(x<<2)+(FIXED_TO_INT(y+32768)*5504))=0xff;
			y+=k;
		}
	}
	else{
		//直线趋于竖直，故沿y轴逐点描画
		//确保y0_y1沿y正方向
		if(y0_y1<0){
			int temp=x0;
			x0=x1;
			x1=temp;
			temp=y0;
			y0=y1;
			y1=temp;
		}
		fixed dxdy=(INT_TO_FIXED(x1)-INT_TO_FIXED(x0))/(y1-y0);//使用顶点数来代替浮点数，是为了在下面的for循环里避开类型转换
		fixed x=INT_TO_FIXED(x0);
		for(int y=y0;y<=y1;y++){
			*(pt_memBuffer+(FIXED_TO_INT(x+32768)<<2)+y*5504)=0xff;
			x+=dxdy;
		}
	}
}

//单纯在memBuffer里绘制1000面100线的面，耗时30毫秒，够慢的。
void drawLine_m(int x0_int,int y0_int,int x1_int,int y1_int){
	int x0_x1_int=x1_int-x0_int;
	int y0_y1_int=y1_int-y0_int;
	if((x0_x1_int>0?x0_x1_int:-x0_x1_int)>(y0_y1_int>0?y0_y1_int:-y0_y1_int)){
		//保证x0,y0在直线左端，作为线头
			if(x0_int>x1_int){
				int temp=x0_int;
				x0_int=x1_int;
				x1_int=temp;
				temp=y0_int;
				y0_int=y1_int;
				y1_int=temp;
			}
		//x0,y0,x1,y1确定下来，接下来可以计算a,b,c了,因为下面代码都默认x0,y0为左端点。	
		//a=y0-y1,b=x1-x0,c=x0y1-x1y0
			int a=y0_int-y1_int;
			int b=x1_int-x0_int;
			int c=x0_int*y1_int-x1_int*y0_int;
			int _2a=a+a;
			int _2b=b+b;
			int _2c=c+c;
			//也可以定位pt_write的初始位置了。
			char*pt_write=pt_memBuffer+(x0_int<<2)+y0_int*bytes_w;
			*pt_write=0xff;
			if(y0_int>y1_int){
				//误写成+b,划线结果竟也不差
				int _2Fm=_2a*x0_int+_2b*y0_int+(_2a-b+_2c);
				for(int x=x0_int+1;x<=x1_int;x++){//中点划线法从x0+1处开始描点。
					//pt_write要线性推移，_2Fm也要线性推移
					//之所以用线性递增，是为了避免乘法运算——毕竟它与x，y是线性关系,就更要利用
					if(_2Fm>0){//(a>0,_2Fm>0)-->(xm>xline)此时中点落在直线下面，或者说直线在中点上面。y--
						_2Fm+=-_2b;//_2Fm(x)向_2Fm(x+1)转变
						pt_write+=-bytes_w;	//显存指针上移一行
					}
					//下面两句是x递增引起的基本递增部分，不受中点上下关系影响。
					pt_write+=4;//显存指针水平向右移动一个像素
					_2Fm+=_2a;//_2Fm(x)向_2Fm(x+1)转变------_2Fm(x)指x横坐标处中点代入ax+by+c的值的2倍。
					*(pt_write)=0xff;//默认是蓝色
				}
			}
			//直线往右下方走
			else{
				int _2Fm=_2a*x0_int+_2b*y0_int+(_2a+b+_2c);
				for(int x=x0_int+1;x<=x1_int;x++){
					if(_2Fm<0){//此时中点落在直线上面，y递增了
						_2Fm+=_2b;
						pt_write+=bytes_w;	
					}
					//这是x递增引起的基本递增部分，不受中点上下关系影响。
					pt_write+=4;
					_2Fm+=_2a;
					//pt_write要线性推移，Fm也要线性推移
					//之所以用线性递增，是为了避免乘法运算——毕竟它与x，y是线性关系,就更要利用
					*(pt_write)=0xff;
				}
			}
	}
	else{
	//保证x0,y0在直线上端
			if(y0_int>y1_int){
				int temp=x0_int;
				x0_int=x1_int;
				x1_int=temp;
				temp=y0_int;
				y0_int=y1_int;
				y1_int=temp;
			}
		//x0,y0,x1,y1确定下来，可以计算a,b,c了,因为下面代码都默认x0,y0在直线上端。	
			int a=y0_int-y1_int;
			int b=x1_int-x0_int;
			int c=x0_int*y1_int-x1_int*y0_int;
			int _2a=a+a;
			int _2b=b+b;
			int _2c=c+c;
			//也可以计算pt_write了。
			char*pt_write=pt_memBuffer+(x0_int<<2)+y0_int*bytes_w;
			if(x1_int>x0_int){
				//误写成+b,划线结果竟也不差
				int _2Fm=_2a*x0_int+_2b*y0_int+(a+_2b+_2c);
				for(int y=y0_int+1;y<=y1_int;y++){
					//pt_write要线性推移，Fm也要线性推移
					//之所以用线性递增，是为了避免乘法运算——毕竟它与x，y是线性关系,就更要利用
					if(_2Fm>0){//(_2Fm>0,a<0)-->(xm<xline),此时中点落在直线左面，x++						_2Fm+=_2b;
						_2Fm+=_2a;//其实是x++带来_2Fm增加_2a
						pt_write+=4;//x++带来pt_write+4	
					}
					//这是x递增引起的基本递增部分，不受中点上下关系影响。
					pt_write+=bytes_w;
					_2Fm+=_2b;
					*(pt_write)=0xff;
				}
			}
			//直线往左下方走
			else{
				int _2Fm=_2a*x0_int+_2b*y0_int+(-a+_2b+_2c);
				for(int y=y0_int+1;y<=y1_int;y++){
					//pt_write要线性推移，Fm也要线性推移
					//之所以用线性递增，是为了避免乘法运算——毕竟它与x，y是线性关系,就更要利用
					if(_2Fm<0){//(_2Fm<0,a<0)-->(xm>xline),此时中点落在直线右面,x--
						_2Fm-=_2a;//x--造成_2Fm-=_2a
						pt_write-=4;//x--造成pt_write-=4
					}
					//这是x递增引起的基本递增部分，不受中点上下关系影响。
					pt_write+=bytes_w;
					_2Fm+=_2b;
					*(pt_write)=0xff;
				}
			}
	}
}

void drawLine_brshm(int x0,int y0,int x1,int y1){
	int x0_x1_int=x1-x0;
	int y0_y1_int=y1-y0;
	
	if((x0_x1_int>0?x0_x1_int:-x0_x1_int)>(y0_y1_int>0?y0_y1_int:-y0_y1_int)){
		if(x0>x1){
			EXCHANGE_INT(x0,x1);
			EXCHANGE_INT(y0,y1);
		}
		int _2dx=x0_x1_int<<1;
		int _2dy=y0_y1_int+y0_y1_int;//may be negative
		int _2ddx=-x0_x1_int;
		char*pt_write=pt_memBuffer+(x0<<2)+y0*bytes_w;
		if(y0>y1){
			_2dy=-_2dy;//k=-k
			for(int x=x0;x<=x1;x++){
				if(_2ddx>0){
					*(pt_write-bytes_w)=0xff;
					if(_2ddx>x0_x1_int){
						_2ddx-=_2dx;
						pt_write-=bytes_w;//affected by y--
					}
				}
				else{
					*pt_write=0xff;
				}
				_2ddx+=_2dy;
				pt_write+=4;
			}
		}
	}
}
void drawLine_brshm_asis(int x0,int y0,int x1,int y1){
	int x0_x1_int=x1-x0;
	int y0_y1_int=y1-y0;
	if((x0_x1_int>0?x0_x1_int:-x0_x1_int)>(y0_y1_int>0?y0_y1_int:-y0_y1_int)){
		if(x0>x1){
			EXCHANGE_INT(x0,x1);
			EXCHANGE_INT(y0,y1);
		}
		float k=(float)y0_y1_int/(float)x0_x1_int;
		int y=y0;
		float d=0;
		if(y0>y1){
			k=-k;//k<0,取正好思考些
			for(int x=x0;x<=x1;x++){
				if(d>0.5){
					*(pt_memBuffer+(x<<2)+(y-1)*bytes_w)=0xff;//drawPixel(x,y-1)
					if(d>1){
						d--;
						y--;
					}
				}
				else{
					*(pt_memBuffer+(x<<2)+y*bytes_w)=0xff;//drawPixel(x,y)
				}
				d+=k;
			}
		}
	}
}
void drawLine_brshm_float(int x0,int y0,int x1,int y1){
	int x0_x1_int=x1-x0;
	int y0_y1_int=y1-y0;
	if((x0_x1_int>0?x0_x1_int:-x0_x1_int)>(y0_y1_int>0?y0_y1_int:-y0_y1_int)){//直线趋于水平
		if(x0>x1){
			EXCHANGE_INT(x0,x1);
			EXCHANGE_INT(y0,y1);
		}
		float k=(float)y0_y1_int/(float)x0_x1_int;
		char*pt_write=pt_memBuffer+(x0<<2)+(y0*bytes_w);
		char*pt_write_now=pt_write;
		if(y0>y1){//往左上走
			//核心循环太冗长，90-5000也不夸张
			k=-k;//k<0，取正好思考些
			float d=-0.5;//其实初始置0.5很恰当，Y0-Ym本来就是0.5
			for(int x=x0;x<=x1;x++){
				if(d>0){//偏差已过中点
				//	pt_write_now=pt_write-bytes_w;
				*(pt_write-bytes_w)=0xff;
					if(d>0.5){
						d--;//将偏差约束回-0.5~0.5
						pt_write-=bytes_w;//affected by y--	
					}
				}
				else{
					*pt_write=0xff;
				}
				//*pt_write_now=0xff;
				//affected by x++
				d+=k;
				pt_write+=4;	
			}
		}
	}
}












