#include "camera.h"
void camera_init(void){//so far,bway3d only supply 1 camera,because it's enough
	camera.viewx=300;
	camera.viewy=275;
	camera.nearwall=100;
	camera.farwall=10000;
	camera.d=100;
	MAT_INIT_14(camera.pos,0,0,-1000)
	//camera.pos={0,0,-1000,1};
	MAT_INIT_14(camera.dir_default,0,0,1)
}

void camera_update(void){
	MAT44 rxm,rym,rzm,rm;
	MAT_RESET_ROTATIONX(&rxm,camera.rotationx)
	MAT_RESET_ROTATIONY(&rym,camera.rotationy)
	MAT_RESET_ROTATIONZ(&rzm,camera.rotationz)
	MAT_CONCAT_3((&rxm),(&rym),(&rzm),(&rm))
	Mat_Mul_14_44(&camera.dir_default,&rm,&camera.dir_curr);//camera.dir_curr updated

	MAT44_PTR pt_rzm_r=&rzm;//for prior
	MAT44_PTR pt_rym_r=&rym;
	MAT44_PTR pt_rxm_r=&rxm;
	MAT44_PTR pt_rm_r=&rm;
	MAT_SET_ROTATIONZ(pt_rzm_r,-camera.rotationz)
	MAT_SET_ROTATIONY(pt_rym_r,-camera.rotationy)
	MAT_SET_ROTATIONX(pt_rxm_r,-camera.rotationx)
	MAT44 tm_r;
	MAT_RESET_OFFSET(&tm_r,-camera.pos.x,-camera.pos.y,-camera.pos.z)
	//rotate should be around a right orign,so trans at first
	Mat_Mul_44_44(&tm_r,pt_rm_r,&(camera.mat_worldToCamera));//camera.mat_worldToCamera updated
}

void camera_info(void){
	printf("\ncamera:		");
	INFO_MAT44(&camera.mat_worldToCamera)
	INFO_MAT14(&camera.dir_curr)
}
