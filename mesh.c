#include"mesh.h"
#include"stdlib.h"
#include"stdio.h"
void mesh_update_fnormals(MESH_PTR pt_mesh){
	for(int findex=0;findex<pt_mesh->len_flist;findex++){
		POINT4D_PTR pt_v0=&(pt_mesh->pt_vlist[pt_mesh->pt_flist[findex].indexofv[0]]);
		POINT4D_PTR pt_v1=&(pt_mesh->pt_vlist[pt_mesh->pt_flist[findex].indexofv[1]]);
		POINT4D_PTR pt_v2=&(pt_mesh->pt_vlist[pt_mesh->pt_flist[findex].indexofv[2]]);
		VECTOR4D v0_v1,v0_v2;
		Vec_Sub_4d(pt_v1,pt_v0,&v0_v1);
		Vec_Sub_4d(pt_v2,pt_v0,&v0_v2);
		Vec_Cross_4d(&v0_v1,&v0_v2,&(pt_mesh->flistbind.pt_nlist[findex]));
	}
}

void mesh_clear_donelist(MESH_PTR pt_mesh){
	for(int vindex=0;vindex<pt_mesh->len_vlist;vindex++){
		pt_mesh->vlistbind.pt_done_list[vindex]=0;
	}
}

void mesh_init(MESH_PTR pt_mesh,POINT4D_PTR pt_vlist,int len_vlist,FACE_PTR pt_flist,int len_flist){
	pt_mesh->pt_flist=pt_flist;
	pt_mesh->len_flist=len_flist;
	pt_mesh->pt_vlist=pt_vlist;
	pt_mesh->len_vlist=len_vlist;
	pt_mesh->vlistbind.pt_done_list=calloc(len_vlist,4);
	pt_mesh->vlistbind.pt_tvlist=calloc(len_vlist,sizeof(POINT4D));
	pt_mesh->vlistbind.pt_nlist=calloc(len_vlist,sizeof(POINT4D));
	pt_mesh->flistbind.pt_nlist=calloc(len_flist,sizeof(VECTOR4D));
	pt_mesh->scalex=1;
	pt_mesh->scaley=1;
	pt_mesh->scalez=1;
	pt_mesh->x=0;
	pt_mesh->y=0;
	pt_mesh->z=0;
	pt_mesh->rotationx=0;
	pt_mesh->rotationy=0;
	//pt_mesh->rotationz=PI/4;
	pt_mesh->rotationz=0;
	pt_mesh->id=99999;
}

void mesh_info(MESH_PTR pt_mesh){
	printf("\nmesh%d	x:%.0f,y:%.0f,z:%.0f,rotationx:%.0f,rotationy:%.0f,rotationz:%.0f,scalex:%.0f,scaley:%.0f,scalez:%.0f,len_vlist:%d  len_flist:%d----------------------------------------",pt_mesh->id,pt_mesh->x,pt_mesh->y,pt_mesh->z,pt_mesh->rotationx,pt_mesh->rotationy,pt_mesh->rotationz,pt_mesh->scalex,pt_mesh->scaley,pt_mesh->scalez,pt_mesh->len_vlist,pt_mesh->len_flist);
	printf("\n vlist	");
	for(int vindex=0;vindex<pt_mesh->len_vlist;vindex++){
		POINT4D_PTR pt_v=pt_mesh->pt_vlist+vindex;
		printf(" [%d][%.0f,%.0f,%.0f,%.0f] ",vindex,pt_v->x,pt_v->y,pt_v->z,pt_v->w);
	}
	printf("\ntvlist	");
	for(int vindex=0;vindex<pt_mesh->len_vlist;vindex++){
		POINT4D_PTR pt_v=pt_mesh->vlistbind.pt_tvlist+vindex;
		printf(" [%d][%.0f,%.0f,%.0f,%.0f] ",vindex,pt_v->x,pt_v->y,pt_v->z,pt_v->w);
	}
	printf("\ndone_list   ");
	for(int vindex=0;vindex<pt_mesh->len_vlist;vindex++){
		printf(" [%d]:%d ",vindex,pt_mesh->vlistbind.pt_done_list[vindex]);
	}
	printf("\nflist  ");
	for(int findex=0;findex<pt_mesh->len_flist;findex++){
		FACE_PTR pt_face=pt_mesh->pt_flist+findex;
		printf("[%d]:[%d,%d,%d] ",findex,pt_face->indexofv[0],pt_face->indexofv[1],pt_face->indexofv[2]);
	}
	printf("\n-----------------------------------------------");
}



































