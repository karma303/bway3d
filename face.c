#include"face.h"
void face_init(FACE_PTR pt_face,int index_v0,int index_v1,int index_v2){
	pt_face->indexofv[0]=index_v0;
	pt_face->indexofv[1]=index_v1;
	pt_face->indexofv[2]=index_v2;
}
