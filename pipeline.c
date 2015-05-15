//可写性和可读性
#include"pipeline.h"
#include"camera.h"
#include"bmath.h"
#include"scene.h"
#include"mesh.h"
#include"renderlist.h"
#include"stdio.h"
#include"draw.h"
//they will be used whithin function just to avoid long-writing-reference
static NODE_MESH_PTR pt_currMeshNode;
static MESH_PTR pt_mesh;
static FACE_PTR pt_flist;
static POINT4D_PTR pt_vlist;
static POINT4D_PTR pt_tvlist;
static VECTOR4D_PTR pt_vnormals;
static VECTOR4D_PTR pt_fnormals;
static int*pt_done_list;

void pipeline_prepare(void){
	printf("\nentering pipeline_prepare,info... ");
	scene_info();
	renderlist_reset();
	camera_update();//ask camera to prepare dir_curr,mat_modelToWorld
	printf("\nleaving pipeline_prepare,info...");
	scene_info();
	camera_info();
}

void pipeline_modelToWorld(void){
	printf("\n>>>>>entering pipeline_modelToWorld,info...");
	scene_info();
	pt_currMeshNode=scene.pt_rootMeshNode;	
	while(pt_currMeshNode){
		pt_mesh=&(pt_currMeshNode->content);
		//>>>>>enter frame-content,handle a certain mesh
		MAT44 sm;//一步一扣
		MAT_RESET_SCALER(&sm,pt_mesh->scalex,pt_mesh->scaley,pt_mesh->scalez)
		MAT44 rm;
		MAT_SET_ROTATION(&rm,pt_mesh->rotationx,pt_mesh->rotationy,pt_mesh->rotationz)//atten:it's set,not reset
		MAT44 tm;
		MAT_RESET_OFFSET(&tm,pt_mesh->x,pt_mesh->y,pt_mesh->z)
		MAT44 mat_modelToWorld;
		MAT_CONCAT_3(&sm,&rm,&tm,&mat_modelToWorld)
		//got mat_modelToWorld,now traverse pt_mesh->pt_list[] to convert all vertices
		INFO_MAT44(&mat_modelToWorld)	
		for(int vindex=0;vindex<pt_mesh->len_vlist;vindex++){
			Mat_Mul_14_44(&(pt_mesh->pt_vlist[vindex]),&mat_modelToWorld,&(pt_mesh->vlistbind.pt_tvlist[vindex]));
		}
		//>>>>>leave frame-content
		pt_currMeshNode=pt_currMeshNode->pt_next;
	}
	printf("\n>>>>>leaving pipeline_modelToWorld,info...");
	scene_info();	
}

void pipeline_cull(void){
	printf("\n>>>>>entering pipeline_cull,info...");
	scene_info();
	renderlist_info();
	pt_currMeshNode=scene.pt_rootMeshNode;
	while(pt_currMeshNode){
		pt_mesh=&(pt_currMeshNode->content);
		pt_vlist=pt_mesh->pt_vlist;
		pt_flist=pt_mesh->pt_flist;
		//>>>>>enter frame-content,handle a certain mesh
		//now traverse flist[] to cull hidden-face
		for(int findex=0;findex<pt_mesh->len_flist;findex++){
			POINT4D_PTR pt_v0=&(pt_mesh->vlistbind.pt_tvlist[pt_flist[findex].indexofv[0]]);
			POINT4D_PTR pt_v1=&(pt_mesh->vlistbind.pt_tvlist[pt_flist[findex].indexofv[1]]);
			POINT4D_PTR pt_v2=&(pt_mesh->vlistbind.pt_tvlist[pt_flist[findex].indexofv[2]]);
			VECTOR4D v0_v1,v0_v2;
			Vec_Sub_4d(pt_v1,pt_v0,&v0_v1);
			Vec_Sub_4d(pt_v2,pt_v0,&v0_v2);
			VECTOR4D fdir;
			Vec_Cross_4d(&v0_v1,&v0_v2,&fdir);
			INFO_MAT14(&v0_v1)
			INFO_MAT14(&v0_v2)
			INFO_MAT14(&fdir)
			//if a face's positive side just smile to camera,add it to renderlist.mflist
			if(Vec_Dot_4d(&fdir,&camera.dir_curr)<0){
				printf("\ncollect a positive face");
				renderlist.mflist[renderlist.num_mfaces].pt_mesh=pt_mesh;
				renderlist.mflist[renderlist.num_mfaces].findex=findex;
				renderlist.num_mfaces++;
			}
			else{
				printf("\nculled");
			}
		}
		//>>>>>leave frame-content
		pt_currMeshNode=pt_currMeshNode->pt_next;
	}
	printf("\n>>>>>leaving pipeline_cull,info...");
	scene_info();
	renderlist_info();
}

void pipeline_worldToCamera(void){//based on vertex
	printf("\n>>>>>>entering pipeline_worldToCamera,info...");
	scene_info();
	renderlist_info();
	//traverse renderlist.mfptrlist,ofcource ,all mfaces are positive
	scene_clear_donelist();//clear all meshs' donelist
	for(int mfindex=0;mfindex<renderlist.num_mfaces;mfindex++){
		if(renderlist.mfptrlist[mfindex]){//if it's a valid pointer
			int findex=renderlist.mflist[mfindex].findex;
			MESH_PTR pt_mesh=renderlist.mflist[mfindex].pt_mesh;
			//the above two are basic member
			FACE_PTR pt_face=pt_mesh->pt_flist+findex;//handle this face
			POINT4D_PTR pt_tvlist=pt_mesh->vlistbind.pt_tvlist;
			int*pt_done_list=pt_mesh->vlistbind.pt_done_list;
			for(int i=0;i<3;i++){
				int vindex=pt_face->indexofv[i];
				POINT4D_PTR pt_v=pt_tvlist+vindex;	
				//from worldToCamera,all process should be based on renderlist.mfptrlist.that means all process should be based on face,not vertex
				//this problem is not particular for renderlist,because there exist no vertex-list especially for culled,cuted..
				if(!pt_done_list[vindex]){
					//>>>>>enter frame-content
					VECTOR4D v_cam;
					Mat_Mul_14_44(pt_tvlist+vindex,&camera.mat_worldToCamera,&v_cam);
					MAT_CP_14(&v_cam,pt_tvlist+vindex)
					pt_done_list[vindex]=1;//create done-mark to avoid repeatly processing
				}
			}
			//>>>>>leave frame-content
		}
	}
	printf("\n>>>>>>leaving pipeline_worldToCamera,info...");
	scene_info();
	renderlist_info();
}

void pipeline_cut(void){
	printf("\n>>>>>>entering pipeline_cut,info...");
	scene_info();
	renderlist_info();
	//traverse renderlist.mfptrlist
	for(int mfindex=0;mfindex<renderlist.num_mfaces;mfindex++){
		if(renderlist.mfptrlist[mfindex]){//if it's a valid pointer
			int findex=renderlist.mflist[mfindex].findex;
			MESH_PTR pt_mesh=renderlist.mflist[mfindex].pt_mesh;
			//the above two are basic member
			//>>>>>enter frame-content
			FACE_PTR pt_face=pt_mesh->pt_flist+findex;//handle this face
			POINT4D_PTR pt_tvlist=pt_mesh->vlistbind.pt_tvlist;
			POINT4D_PTR pt_v0=pt_tvlist+pt_face->indexofv[0];
			POINT4D_PTR pt_v1=pt_tvlist+pt_face->indexofv[1];
			POINT4D_PTR pt_v2=pt_tvlist+pt_face->indexofv[2];
			float minz;
			MINZ(pt_v0->z,pt_v1->z,pt_v2->z,minz);//macro-function can receive normal-variable,not necessary pointer	
			if(minz<camera.nearwall||minz>camera.farwall) renderlist.mfptrlist[mfindex]=(void*)0;//cut it
			//>>>>>leave frame-content
		}
	}
	printf("\n>>>>>>leaving pipeline_cut,info...");
	scene_info();
	renderlist_info();
}
void pipeline_perspect(void){//based on vertex
	printf("\n>>>>>>entering pipeline_perspect...");
	scene_info();
	renderlist_info();
	scene_clear_donelist();//clear all meshs' donelist
	//traverse renderlist.mfptrlist
	for(int mfindex=0;mfindex<renderlist.num_mfaces;mfindex++){
		if(renderlist.mfptrlist[mfindex]){//if it's a valid pointer
			int findex=renderlist.mflist[mfindex].findex;
			MESH_PTR pt_mesh=renderlist.mflist[mfindex].pt_mesh;
			//the above two are basic member
			FACE_PTR pt_face=pt_mesh->pt_flist+findex;//handle this face
			POINT4D_PTR pt_tvlist=pt_mesh->vlistbind.pt_tvlist;
			int*pt_done_list=pt_mesh->vlistbind.pt_done_list;
			for(int i=0;i<3;i++){
				int vindex=pt_face->indexofv[i];
				POINT4D_PTR pt_v=pt_tvlist+vindex;	
				//from worldToCamera,all process should be based on renderlist.mfptrlist.that means all process should be based on face,not vertex
				//this problem is not particular for renderlist,because there exist no vertex-list especially for culled,cuted..
				if(!pt_done_list[vindex]){
					//>>>>>enter frame-content
					float scaler=camera.d/(pt_v->z);
					pt_v->x*=scaler;
					pt_v->y*=scaler;
					pt_done_list[vindex]=1;
					printf("\nscaler=%f,pt_v->x=%f,pt_v->y=%f",scaler,pt_v->x,pt_v->y);
				}
				else{
					printf("\n meet a done vindex");
				}
			}
			//>>>>>leave frame-content
		}
	}
	printf("\n>>>>>>leaving pipeline_perspect,info...");
	scene_info();
	renderlist_info();
}
void pipeline_mapScreen(void){//based on vertex
	printf("\n>>>>>>entering pipeline_mapScreen,info...");
	scene_info();
	renderlist_info();
	scene_clear_donelist();//clear all meshs' donelist
	//traverse renderlist.mfptrlist
	for(int mfindex=0;mfindex<renderlist.num_mfaces;mfindex++){
		if(renderlist.mfptrlist[mfindex]){//if it's a valid pointer
			int findex=renderlist.mflist[mfindex].findex;
			MESH_PTR pt_mesh=renderlist.mflist[mfindex].pt_mesh;
			//the above two are basic member
			FACE_PTR pt_face=pt_mesh->pt_flist+findex;//handle this face
			POINT4D_PTR pt_tvlist=pt_mesh->vlistbind.pt_tvlist;
			int*pt_done_list=pt_mesh->vlistbind.pt_done_list;
			for(int i=0;i<3;i++){
				int vindex=pt_face->indexofv[i];
				POINT4D_PTR pt_v=pt_tvlist+vindex;	
				//from worldToCamera,all process should be based on renderlist.mfptrlist.that means all process should be based on face,not vertex
				//this problem is not particular for renderlist,because there exist no vertex-list especially for culled,cuted..
				if(!pt_done_list[vindex]){
					//>>>>>enter frame-content
					pt_v->x=camera.viewx+pt_v->x;
					pt_v->y=camera.viewy+(-pt_v->y);
					pt_done_list[vindex]=1;
					printf("\npt_v->x=%f,pt_v->y=%f",pt_v->x,pt_v->y);
				}
			}
			//>>>>>leave frame-content
		}
	}
	printf("\n>>>>>>leaving pipeline_mapScreen,info...");
	scene_info();
	renderlist_info();
}

void pipeline_render(void){
	printf("\n>>>>>enter pipeline_render");
	//traverse renderlist.mfptrlist
	for(int mfindex=0;mfindex<renderlist.num_mfaces;mfindex++){
		if(renderlist.mfptrlist[mfindex]){//if it's a valid pointer
			int findex=renderlist.mflist[mfindex].findex;
			MESH_PTR pt_mesh=renderlist.mflist[mfindex].pt_mesh;
			//the above two are basic member
			//>>>>>enter frame-content
			FACE_PTR pt_face=pt_mesh->pt_flist+findex;//handle this face
			POINT4D_PTR pt_tvlist=pt_mesh->vlistbind.pt_tvlist;
			POINT4D_PTR pt_v0=pt_tvlist+pt_face->indexofv[0];
			POINT4D_PTR pt_v1=pt_tvlist+pt_face->indexofv[1];
			POINT4D_PTR pt_v2=pt_tvlist+pt_face->indexofv[2];
			drawTriangle(pt_v0,pt_v1,pt_v2);	
			//>>>>>leave frame-content
		}
	}
	printf("\n>>>>>leave pipeline_render");
}


































