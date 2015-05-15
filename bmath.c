#include"bmath.h"
void Mat_Mul_44_44(MAT44_PTR pt_a,MAT44_PTR pt_b,MAT44_PTR pt_c){
	for(int line=0;line<4;line++){
		for(int col=0;col<4;col++){
			//get pt_c[line][col]
			float tmp=0;
			for(int lc=0;lc<4;lc++){//lc means line or column
				tmp+=pt_a->M[line][lc]*pt_b->M[lc][col];//attention here,i use pt_a[][] invest a struct(infact it's a union)very strange.
			}
			pt_c->M[line][col]=tmp;
		}
	}
}

void Mat_Mul_14_44(MAT14_PTR pt_a,MAT44_PTR pt_b,MAT14_PTR pt_c){
	for(int col=0;col<4;col++){
		//get pt_c->M[0][col]
		float sum=0;
		for(int lc=0;lc<4;lc++){
			sum+=pt_a->m[lc]*pt_b->M[lc][col];
		}
		pt_c->m[col]=sum;
	}
}

void Vec_Cross_4d(VECTOR4D_PTR pt_a,VECTOR4D_PTR pt_b,VECTOR4D_PTR pt_c){
	float ax=pt_a->x;
	float ay=pt_a->y;
	float az=pt_a->z;
	float bx=pt_b->x;
	float by=pt_b->y;
	float bz=pt_b->z;
	pt_c->x=ay*bz-az*by;
	pt_c->y=az*bx-ax*bz;
	pt_c->z=ax*by-ay*bx;
	pt_c->w=1;
}

float Vec_Dot_4d(VECTOR4D_PTR pt_a,VECTOR4D_PTR pt_b){
	return pt_a->x*pt_b->x+pt_a->y*pt_b->y+pt_a->z*pt_b->z;
}

void Vec_Sub_4d(VECTOR4D_PTR pt_a,VECTOR4D_PTR pt_b,VECTOR4D_PTR pt_c){
	pt_c->w=1;
	pt_c->x=pt_a->x-pt_b->x;
	pt_c->y=pt_a->y-pt_b->y;
	pt_c->z=pt_a->z-pt_b->z;
}




























