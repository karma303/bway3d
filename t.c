//t.c只是一个测试bway3d的demo文件。
//在bway3d里，你面向v0,v1,v2看它们成顺时针，那面法线就指向你，否则就背向你。
//meshnode的content很不方便，似乎
#include"../lib/matrix/matrix.h"
#include "bmath.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"draw.h"
#include<run.h>
#include"utils.h"
#include<sys/time.h>
#include"kbd.h"
#include<linux/input.h>
#include"pipeline.h"
#include"init.h"
#include"scene.h"
#include"mesh.h"
#include"face.h"
#include"renderlist.h"
#include"camera.h"
#define FPS (30)
void handle_key(void);
void test(void);
void drawGrid(int type);
int main(void){
//	test();
	init();
	open_event4();	
	initWindow(100,100,600,550);
	struct timeval start_timeval,end_timeval;
	//create mesh_triangle
	POINT4D vlist[8];
	int l=200;
	MAT_INIT_14(vlist[0],0,0,0)
	MAT_INIT_14(vlist[1],l,0,0)
	MAT_INIT_14(vlist[2],l,l,0)
	MAT_INIT_14(vlist[3],0,l,0)
	MAT_INIT_14(vlist[4],0,0,l)
	MAT_INIT_14(vlist[5],l,0,l)
	MAT_INIT_14(vlist[6],l,l,l)
	MAT_INIT_14(vlist[7],0,l,l)
	FACE flist[1];
	flist[0].indexofv[0]=0;
	flist[0].indexofv[1]=1;
	flist[0].indexofv[2]=2;
	NODE_MESH meshNode0;
	mesh_init(&meshNode0.content,vlist,3,flist,1);//mesh_triangle prepared
	NODE_MESH meshNode1;
	mesh_init(&meshNode1.content,vlist,3,flist,1);

	//scene_addMeshNode(&meshNode0);
	//scene_addMeshNode(&meshNode1);
	NODE_MESH meshNode2;
	FACE flist_cube[12];
	face_init(flist_cube,0,3,2);
	face_init(flist_cube+1,0,2,1);
	face_init(flist_cube+2,3,7,6);
	face_init(flist_cube+3,3,6,2);
	face_init(flist_cube+4,4,6,7);
	face_init(flist_cube+5,4,5,6);
	face_init(flist_cube+6,0,5,4);
	face_init(flist_cube+7,0,1,5);
	face_init(flist_cube+8,0,4,7);
	face_init(flist_cube+9,0,7,3);
	face_init(flist_cube+10,1,2,6);
	face_init(flist_cube+11,1,6,5);
	mesh_init(&meshNode2.content,vlist,8,flist_cube,12);
	scene_addMeshNode(&meshNode2);
	while(1){
		gettimeofday(&start_timeval,NULL);
		memset(pt_memBuffer,0,5504*768);//set buffer-background black
		drawGrid(1);
		updatekbd();

		//毛病出来了，单旋转就有cull错，再平移，缩放，更有事。
		meshNode2.content.rotationz+=PI/10/30;		
		meshNode2.content.rotationx+=PI/10/30;
		meshNode2.content.rotationy+=PI/10/30;
	//	meshNode2.content.x+=2;
	//	meshNode2.content.y+=2;	
		//meshNode2.content.scalex+=0.01;
		//mesh_triangle.scaley+=1.5;
		//meshNode0.content.y+=1;
		//camera.pos.y+=5;
//		camera.rotationy-=PI/300;
		//pipeline run
		/**
		pipeline_prepare();
		pipeline_modelToWorld();
		pipeline_cull();
		pipeline_worldToCamera();
		pipeline_cut();
		pipeline_perspect();
		pipeline_mapScreen();
		pipeline_render();
		*/
		//return 10;
		handle_key();
		//RUN(1,1)
	//	RUN(500,5)
//		ENDRUN
		trans_bitBlock();	//render after processing all data,after trans_bitBlock,the frame will sleep
		gettimeofday(&end_timeval,NULL);
		int ms_past=(end_timeval.tv_sec-start_timeval.tv_sec)*1000+(end_timeval.tv_usec-start_timeval.tv_usec)/1000;
		int ms_rest=1000/FPS-ms_past;
		if(ms_rest>0); usleep(ms_rest*1000);
	}
	return 0;
}

void handle_key(){
	int increx=5;
	int increy=5;
	int increw=5;
	int increh=5;
	if(kbd[KEY_EQUAL]){
		if(kbd[KEY_X]) x_window+=increx;
		if(kbd[KEY_Y]) y_window+=increy;
		if(kbd[KEY_W]) width_window+=increw;
		if(kbd[KEY_H]) height_window+=increh;
	}
	if(kbd[KEY_MINUS]){
		if(kbd[KEY_X]) x_window-=increx;
		if(kbd[KEY_Y]) y_window-=increy;
		if(kbd[KEY_W]) width_window-=increw;
		if(kbd[KEY_H]) height_window-=increh;
	}
}

void test(void){
	/**
	MAT44 a={
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
	};
	MAT44 b={
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
	};	
	MAT14 v1={1,2,3,1};
	//printf("(&v1.w):%p",(&v1.w));
	MAT44 rym;
	MAT_RESET_ROTATIONY(&rym,ATR(30))
	MAT44 rzm;
	MAT_RESET_ROTATIONZ(&rzm,ATR(30))
	MAT44 rxm;
	MAT_RESET_ROTATIONX(&rxm,ATR(30))
	MAT44 tm={1,2,3,4};
	MAT_RESET_OFFSET(&tm,10,100,1000)
	MAT44 sm={1,2,3,4};
	MAT_RESET_SCALER(&sm,1,0.1,0.01)
	
	MAT14 v11;
	v11.x=2;
//	Mat_Mul_14_44(&v1,&rym,&v11);
//	INFO_MAT14(&v11)
	//Mat_Mul_14_44(&v1,&rzm,&v11);
//	INFO_MAT14(&v1)
	Mat_Mul_14_44(&v1,&sm,&v11);
	INFO_MAT14(&v11)
		//all done
	*/
}

void drawGrid(int type){
	switch(type){
		case 0:
			if(1){//show grid,use 0 to turn off
				for(int x=0;x<=width_window-1;x+=100 ){//并不是平分算法，固定为100X100的格子，思路很清晰。
					drawLine_dda_fixed(x,0,x,height_window-1);
				}
				for(int y=0;y<=height_window-1;y+=100){
					drawLine_dda_fixed(0,y,width_window-1,y);
				}
			}
			break;
		case 1:
			drawLine_dda_fixed(0,275,600,275);
			drawLine_dda_fixed(300,0,300,550);
			break;
		default:
			printf("\nbad type");
			break;
	}
}

/**garbage
 *
	//	drawLine_brshm(100,100,200,90);
		//drawCircle_dda(0,0,10,0xff);
		//drawCircle_dda(100,200,100,0xff);
		for(int y=100;y<200;y++){
//			drawLine_m(200,y-10,100,y);//80
//			drawLine_m(100,y,200,y+10);//87
//			drawLine_m(y,100,y+10,200);//71-5000
//			drawLine_m(y,100,y+30,200);//64-5000
//			drawLine_dda_float(100,y,200,y-10);//182
//			drawLine_dda_float(y,100,y,200);//199-5000
	//		drawLine_dda_fixed(100,y,200,y-10);//70ms-5000
			//drawLine_dda_fixed(y,100,y-10,200);//70ms-5000
//			drawLine_brshm_float(100,y,200,y-10);//93ms-5000
			//drawLine_brshm(100,y,200,y-10);//65ms-5000
			//drawLine_brshm_asis(100,y,200,y-10);
		}
		//drawCircle_dda(100,200,100,0xff);
		
 */


















