#ifndef MESH_H
#define MESH_H
#include"bmath.h"
#include"face.h"
//local information of shared-vlist for every mesh,like trans-position,handle-mark,vertex-normal
typedef struct vlistbind{
	POINT4D_PTR pt_tvlist;
	int* pt_done_list;
	VECTOR4D_PTR pt_nlist;
}VLISTBIND,*VLISTBIND_PTR;

//local information of shared-flist for every mesh,like face-normal
typedef struct flistbind{
	VECTOR4D_PTR pt_nlist;
}FLISTBIND,*FLISTBIND_PTR;

typedef struct mesh{
	int id;
	float rotationx,rotationy,rotationz;//rotate by model-axis
	float scalex,scaley,scalez;//scale by model-axis
	float x,y,z;//world position
	POINT4D_PTR pt_vlist;
	int len_vlist;//that's num_vertices
	VLISTBIND vlistbind;

	FACE_PTR pt_flist;
	int len_flist;//this's num_faces
	FLISTBIND flistbind;
}MESH,*MESH_PTR;

typedef struct nodemesh{
	MESH content;
	struct nodemesh* pt_prev;
	struct nodemesh* pt_next;
}NODE_MESH,*NODE_MESH_PTR;

void mesh_update_fnormals(MESH_PTR pt_mesh);
void mesh_clear_donelist(MESH_PTR pt_mesh);
void mesh_init(MESH_PTR pt_mesh,POINT4D_PTR pt_vlist,int len_vlist,FACE_PTR pt_flist,int len_flist);
void mesh_info(MESH_PTR pt_mesh);
#endif
























