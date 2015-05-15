#ifndef RENDERLIST_H
#define RENDERLIST_H
#include"mesh.h"
#define MAX_RENDER_FACE 10000
typedef struct meshface{
	MESH_PTR pt_mesh;//which mesh
	int findex;//which face
}MESHFACE,*MESHFACE_PTR;

typedef struct renderlist{
	int num_mfaces;
	MESHFACE mflist[MAX_RENDER_FACE];
	MESHFACE_PTR mfptrlist[MAX_RENDER_FACE];
}RENDERLIST,*RENDERLIST_PTR;


//global renderlist
RENDERLIST renderlist,*pt_renderlist;//after init_renderlist,pt_renderlist will point to renderlist.

void renderlist_init(void);
void renderlist_reset(void);
void renderlist_info(void);
#endif
