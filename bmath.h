#ifndef BMATH_H
#define BMATH_H
#include<string.h>
#include<math.h>
typedef union mat14{
		float m[4];//used for computing,1-dimension is lighter
		float M[1][4]; //to compate MMmul() which need a 2-dimension array-ptr as argument
		struct{//for matrix-use
			float M00,M01,M02,M03;
		};//这里有两个struct了，初始化就再不能直接用{x,y,z,w}了。但仍然可以直呼M00,x,y,z...
		struct{//for vector4d-use
			float x,y,z,w;
		};
}MAT14,*MAT14_PTR,FLOAT4D,*FLOAT4D_PTR,POINT4D,*POINT4D_PTR,VECTOR4D,*VECTOR4D_PTR;

#define MAT_INIT_14(mat14,x,y,z)  mat14.m[0]=x;mat14.m[1]=y;mat14.m[2]=z;mat14.m[3]=1;
typedef union mat44{
		float M[4][4];
		struct{
			float M00,M01,M02,M03;
			float M10,M11,M12,M13;
			float M20,M21,M22,M23;
			float M30,M31,M32,M33;
		};
}MAT44,*MAT44_PTR;

//may wrong,for union mat14 changed member
void Mat_Mul_44_44(MAT44_PTR pt_a,MAT44_PTR pt_b,MAT44_PTR pt_c);
void Mat_Mul_14_44(MAT14_PTR pt_a,MAT44_PTR pt_b,MAT14_PTR pt_c);
void Vec_Cross_4d(VECTOR4D_PTR pt_a,VECTOR4D_PTR pt_b,VECTOR4D_PTR pt_c);
float Vec_Dot_4d(VECTOR4D_PTR pt_a,VECTOR4D_PTR pt_b);
void Vec_Sub_4d(VECTOR4D_PTR pt_a,VECTOR4D_PTR pt_b,VECTOR4D_PTR pt_c);

/**mat_zero differs from mat_reset,for the latter  create a matrix that rotate 0,scale 1,and trans 0
| 1 	0	0	0 |
| 0 	1	0	0 |
| 0 	0	1	0 |
| 0 	0	0	1 |
but in fact,mat_reset was not used in bmath.h
 */
#define MAT_ZERO_14(pt_m14)	memset((void*)(pt_m14),0,sizeof(MAT14));
#define MAT_ZERO_44(pt_m44) memset((void*)(pt_m44),0,sizeof(MAT44));

#define MAT_CP_14(pt_src,pt_dest) memcpy((void*)(pt_dest),(void*)(pt_src),sizeof(MAT14));
#define MAT_CP_44(pt_src,pt_dest) memcpy((void*)(pt_dest),(void*)(pt_src),sizeof(MAT44));

//especially for MAT44
//not-checked
MAT44 ab_;
#define MAT_CONCAT_3(pt_a,pt_b,pt_c,pt_result)\
Mat_Mul_44_44(pt_a,pt_b,&ab_);\
Mat_Mul_44_44(&ab_,pt_c,pt_result);


//here emerge one prioty of mat44's union:i just use (pt_m44)->MXX,avoid writing M[x][x]
//atten:diagonal-line should be 1-1-1-1
float cosO,sinO;
#define PI 3.14159 
#define ANGLE_TO_RADIAN(angle) (float)(angle)/180*PI 
#define ATR(angle) (float)(angle)/180*PI 
#define MAT_SET_OFFSET(pt_m44,offsetx,offsety,offsetz) (pt_m44)->M30=offsetx;(pt_m44)->M31=offsety;(pt_m44)->M32=offsetz;\
														(pt_m44)->M00=1;(pt_m44)->M11=1;(pt_m44)->M22=1;(pt_m44)->M33=1;
#define MAT_RESET_OFFSET(pt_m44,offsetx,offsety,offsetz)\
MAT_ZERO_44(pt_m44)\
MAT_SET_OFFSET(pt_m44,offsetx,offsety,offsetz)

#define MAT_SET_SCALER(pt_m44,scalex,scaley,scalez) (pt_m44)->M00=scalex;(pt_m44)->M11=scaley;(pt_m44)->M22=scalez;\
														(pt_m44)->M33=1;
#define MAT_RESET_SCALER(pt_m44,scalex,scaley,scalez)\
MAT_ZERO_44(pt_m44)\
MAT_SET_SCALER(pt_m44,scalex,scaley,scalez) 

//MAT_SET_ROTATIONX create a matrix for rotating around axis-x/y/z from a zero matrix.
//MAT_RESET_ROTATIONX create a matrix for rotating around axis-x/y/z from a certain matrix,it will pre-zero the supplied-matrix
/**Mx
 | 1		0		0		0 |
 | 0		cosO	sinO	0 |
 | 0		-sinO	cosO	0 |
 | 0		0		0		1 |
 */
#define MAT_SET_ROTATIONX(pt_m44,radian)\
cosO=cos(radian);sinO=sin(radian);\
(pt_m44)->M11=cosO;\
(pt_m44)->M12=sinO;\
(pt_m44)->M21=-sinO;\
(pt_m44)->M22=cosO;\
(pt_m44)->M00=(pt_m44)->M33=1;

#define MAT_RESET_ROTATIONX(pt_m44,radian)\
MAT_ZERO_44(pt_m44)\
MAT_SET_ROTATIONX(pt_m44,radian)

/**My
 | cosO		0		-sinO		0 |
 | 0		1		0			0 |
 | sinO		0		cosO		0 |
 | 0		0		0			1 |
 */
#define MAT_SET_ROTATIONY(pt_m44,radian)\
cosO=cos(radian);sinO=sin(radian);\
(pt_m44)->M00=cosO;\
(pt_m44)->M02=-sinO;\
(pt_m44)->M20=sinO;\
(pt_m44)->M22=cosO;\
(pt_m44)->M11=(pt_m44)->M33=1;

#define MAT_RESET_ROTATIONY(pt_m44,radian)\
MAT_ZERO_44(pt_m44)\
MAT_SET_ROTATIONY(pt_m44,radian)
/**Mz
 | cosO		sinO	0		0 |
 | -sinO	cosO	0		0 |
 | 0		0		1		0 |
 | 0		0		0		1 |
 */
#define MAT_SET_ROTATIONZ(pt_m44,radian)\
cosO=cos(radian);sinO=sin(radian);\
(pt_m44)->M00=cosO;\
(pt_m44)->M01=sinO;\
(pt_m44)->M10=-sinO;\
(pt_m44)->M11=cosO;\
(pt_m44)->M22=(pt_m44)->M33=1;

#define MAT_RESET_ROTATIONZ(pt_m44,radian)\
MAT_ZERO_44(pt_m44)\
MAT_SET_ROTATIONZ(pt_m44,radian)

MAT44 tmprx,tmpry,tmprz;//can not use to create mat_worldToCamera,for that's rz_r,ry_r,rx_r
#define MAT_SET_ROTATION(pt_m44,rotationx,rotationy,rotationz)\
MAT_RESET_ROTATIONX(&tmprx,rotationx)\
MAT_RESET_ROTATIONY(&tmpry,rotationy)\
MAT_RESET_ROTATIONZ(&tmprz,rotationz)\
MAT_CONCAT_3(&tmprx,&tmpry,&tmprz,pt_m44)

int ii,jj;//used in macro
#define INFO_MAT44(pt_m44)\
	printf("\n%s=",#pt_m44);\
	for(ii=0;ii<4;ii++){\
			putchar('\n');\
			for(jj=0;jj<4;jj++){\
				printf("%10.2f ",(pt_m44)->M[ii][jj]);\
			}\
		}

#define INFO_MAT14(pt_m)\
	printf("\n%s=",#pt_m);\
			putchar('\n');\
			for(jj=0;jj<4;jj++){\
				printf("%10.2f ",(pt_m)->m[jj]);\
			}
	

//there is better way x?x:x...
#define MINZ(a,b,c,minz)\
if(a<b){\
	if(a<c) minz=a;\
	else minz=c;\
}\
else{\
	if(b<c) minz=b;\
	else minz=c;\
}
#endif


































