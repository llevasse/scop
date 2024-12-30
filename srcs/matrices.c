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
	float scale = 1 / tan((scene->fov * 0.5) * RADIAN); 
	for (int i =0;i < 4;i++){
		for (int j=0; j<4;j++){
			scene->matrix[i][j] = 0.0;
			scene->matrix_camera[i][j] = 0.0;
		}
	}

	scene->matrix[0][0] = scale;

	scene->matrix[1][1] = scale;

	scene->matrix[2][2] = (-scene->far_plane_distance) / (scene->far_plane_distance - scene->near_plane_distance);
	scene->matrix[2][3] = -1;

	scene->matrix[3][2] = ((-scene->far_plane_distance) * scene->near_plane_distance) / (scene->far_plane_distance - scene->near_plane_distance);

	scene->matrix_camera[3][1] = -10.0;
	scene->matrix_camera[3][2] = -20.0;
}

void	multiplyPointWithMatrix(t_scene *scene, t_vertices *p, float matrix[4][4]){
	(void)scene;

	p->matrixed_x   = p->matrixed_x * matrix[0][0] + p->matrixed_y * matrix[1][0] + p->matrixed_z * matrix[2][0] + matrix[3][0];
    p->matrixed_y   = p->matrixed_x * matrix[0][1] + p->matrixed_y * matrix[1][1] + p->matrixed_z * matrix[2][1] + matrix[3][1];
    p->matrixed_z   = p->matrixed_x * matrix[0][2] + p->matrixed_y * matrix[1][2] + p->matrixed_z * matrix[2][2] + matrix[3][2];
    float w = p->matrixed_x * matrix[0][3] + p->matrixed_y * matrix[1][3] + p->matrixed_z * matrix[2][3] + scene->matrix[3][3]; 

    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1) { 
        p->matrixed_x /= w; 
        p->matrixed_y /= w; 
        p->matrixed_z /= w; 
    }
 
	printf("processed : \n");
	printf("\t%f :\t%f\n", p->x, p->matrixed_x);
	printf("\t%f :\t%f\n", p->y, p->matrixed_y);
	printf("\t%f :\t%f\n", p->z, p->matrixed_z);
}