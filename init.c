#include"scene.h"
#include"camera.h"
#include"renderlist.h"
void init(void){
	scene_init();//meshNode-list inited
	camera_init();//camera inited
	renderlist_init();//rederlist created,namely mflist[],mfptrlist[] inited
}
