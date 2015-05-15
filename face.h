#ifndef FACE_H
#define FACE_H
typedef struct face{
	//face is triangle by default
	int indexofv[3];
}FACE,*FACE_PTR;

void face_init(FACE_PTR pt_face,int index_v0,int index_v1,int index_v2);
#endif



