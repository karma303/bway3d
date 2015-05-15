#include"scene.h"
#include"mesh.h"
#include<stdio.h>
void scene_init(void){
	scene.num_meshNodes=0;
	scene.pt_rootMeshNode=scene.pt_endMeshNode=NULL;
}

void scene_addMeshNode(NODE_MESH_PTR pt_meshNode){
	if(!scene.pt_rootMeshNode){
		scene.pt_rootMeshNode=scene.pt_endMeshNode=pt_meshNode;//wrong pointer
		scene.pt_rootMeshNode->pt_prev=scene.pt_rootMeshNode->pt_next=(void*)0;
		scene.pt_endMeshNode->pt_prev=scene.pt_endMeshNode->pt_next=(void*)0;
	}
	else{
		scene.pt_endMeshNode->pt_next=pt_meshNode;
		pt_meshNode->pt_prev=scene.pt_endMeshNode;
		pt_meshNode->pt_next=NULL;
		scene.pt_endMeshNode=pt_meshNode;
	}
	if(pt_meshNode->content.id==99999) pt_meshNode->content.id=scene.num_meshNodes;
	scene.num_meshNodes++;
}

void scene_clear_donelist(void){
	NODE_MESH_PTR pt_currMeshNode=scene.pt_rootMeshNode;
	while(pt_currMeshNode){
		mesh_clear_donelist(&(pt_currMeshNode->content));
		pt_currMeshNode=pt_currMeshNode->pt_next;
	}
}

void scene_info(void){
	printf("\nscene-info 	num_meshNode:%d,pt_rootMeshNode->content.id:%d,pt_endMeshNode->content.id:%d",scene.num_meshNodes,scene.pt_rootMeshNode->content.id,scene.pt_endMeshNode->content.id);
	NODE_MESH_PTR pt_currMeshNode=scene.pt_rootMeshNode;
	while(pt_currMeshNode){
		mesh_info(&(pt_currMeshNode->content));
		pt_currMeshNode=pt_currMeshNode->pt_next;
	}
	printf("\nend-scene-info");
}
















