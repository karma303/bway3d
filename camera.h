#ifndef CAMERA_H
#define CAMERA_H
#include"bmath.h"
typedef struct camera{
	//customed by users
	float d;
	float viewx,viewy;
	float nearwall;
	float farwall;
	float rotationx,rotationy,rotationz;
	FLOAT4D pos;
	VECTOR4D dir_default;
	
	//the following were updated according to the above at update_camera
	VECTOR4D dir_curr;//for cull
	MAT44 mat_worldToCamera;//for worldToCamera
}CAMERA,*CAMERA_PTR;

CAMERA camera;//gloabl var

void camera_init(void);
void camera_update(void);
void camera_info(void);
#endif
