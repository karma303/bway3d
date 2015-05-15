#ifndef SCENE_H
#define SCENE_H
#include"mesh.h"
typedef struct scenee{
	NODE_MESH_PTR pt_rootMeshNode;
	NODE_MESH_PTR pt_endMeshNode;
	int num_meshNodes;
}SCENE,*SCENE_PTR;

SCENE scene;
void scene_init(void);
void scene_addMeshNode(NODE_MESH_PTR pt_meshNode);
void scene_clear_donelist(void);
void scene_info(void);
#endif
