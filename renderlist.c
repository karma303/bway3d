#include"renderlist.h"
#include"stdio.h"
void renderlist_init(void){
	for(int mfid=0;mfid<MAX_RENDER_FACE;mfid++) renderlist.mfptrlist[mfid]=&renderlist.mflist[mfid];
	renderlist.num_mfaces=0;
	pt_renderlist=&renderlist;
}

void renderlist_reset(void){//a little ugly
	for(int mfid=0;mfid<MAX_RENDER_FACE;mfid++) renderlist.mfptrlist[mfid]=&renderlist.mflist[mfid];
	renderlist.num_mfaces=0;
}

void renderlist_info(void){
	printf("\nrenderlist-info num_mfaces:%d",renderlist.num_mfaces);
	printf("\nmfindex[meshid.findex][v0,v1,v2][pt]	");
	for(int mfid=0;mfid<renderlist.num_mfaces;mfid++){
		MESH_PTR pt_mesh=renderlist.mflist[mfid].pt_mesh;
		int findex=renderlist.mflist[mfid].findex;
		FACE_PTR pt_face=pt_mesh->pt_flist+findex;
		POINT4D_PTR pt_v0=pt_mesh->vlistbind.pt_tvlist+pt_face->indexofv[0];
		POINT4D_PTR pt_v1=pt_mesh->vlistbind.pt_tvlist+pt_face->indexofv[1];
		POINT4D_PTR pt_v2=pt_mesh->vlistbind.pt_tvlist+pt_face->indexofv[2];
		printf(" 	%d[%d.%d][(%.0f,%.0f,%.0f,%.0f),(%.0f,%.0f,%.0f,%.0f),(%.0f,%.0f,%.0f,%.0f)][%p]",mfid,pt_mesh->id,findex,pt_v0->x,pt_v0->y,pt_v0->z,pt_v0->w,pt_v1->x,pt_v1->y,pt_v1->z,pt_v1->w,pt_v2->x,pt_v2->y,pt_v2->z,pt_v2->w,renderlist.mfptrlist+mfid);
	}
}
/**
void renderlist_info(void){
	printf("\nrenderlist	num_mfaces:%d ",renderlist.num_mfaces);
	printf("\nkkk"
}
*/
