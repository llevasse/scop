#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

//{
//	S = (1 / tan((fov / 2) * (pi / 180)))
//
//	n = distance to near clipping plane
//	f = distance to far clipping plane
//	C	= -(f / (f - n))
//	C1	= -((f * n) / (f - n))
//
//	{S, 0, 0,  0},
//	{0, S, 0,  0},
//	{0, 0, C, -1}
//	{0, 0, C1, 0},
//}

void	setMatrix(t_scene *scene){
	double scale = 1 / tan((scene->fov * 0.5) * RADIAN);
	scene->matrix[0][0] = scale;
	scene->matrix[0][1] = 0;
	scene->matrix[0][2] = 0;
	scene->matrix[0][3] = 0;

	scene->matrix[1][0] = 0;
	scene->matrix[1][1] = scale;
	scene->matrix[1][2] = 0;
	scene->matrix[1][3] = 0;

	scene->matrix[2][0] = 0;
	scene->matrix[2][1] = 0;
	scene->matrix[2][2] = (-scene->far_plane_distance) / (scene->far_plane_distance - scene->near_plane_distance);
	scene->matrix[2][3] = -1;

	scene->matrix[3][0] = 0;
	scene->matrix[3][1] = 0;
	scene->matrix[3][2] = ((-scene->far_plane_distance) * scene->near_plane_distance) / (scene->far_plane_distance - scene->near_plane_distance);
	scene->matrix[3][3] = 0;
}

void	multiplyPointWithMatrix(t_scene *scene, t_vertices *p){
//	printf("\t%f\n", p->x);
//	printf("\t%f\n", p->y);
//	printf("\t%f\n", p->z);

	(void)scene;
	p->matrixed_x	= p->x;
	p->matrixed_y	= p->y;
	p->matrixed_z	= p->z;

	p->matrixed_x   = p->x * scene->matrix[0][0] + p->y * scene->matrix[1][0] + p->z * scene->matrix[2][0] + scene->matrix[3][0];
    p->matrixed_y   = p->x * scene->matrix[0][1] + p->y * scene->matrix[1][1] + p->z * scene->matrix[2][1] + scene->matrix[3][1];
    p->matrixed_z   = p->x * scene->matrix[0][2] + p->y * scene->matrix[1][2] + p->z * scene->matrix[2][2] + scene->matrix[3][2];
    //float w = p->x * scene->matrix[0][3] + p->y * scene->matrix[1][3] + p->z * scene->matrix[2][3] + scene->matrix[3][3]; 
 
//	printf("processed : \n");
//	printf("\t%f\n", p->matrixed_x);
//	printf("\t%f\n", p->matrixed_y);
//	printf("\t%f\n", p->matrixed_z);

    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    /*if (w != 1) { 
        p->matrixed_x /= w; 
        p->matrixed_y /= w; 
        p->matrixed_z /= w; 
    } */
}