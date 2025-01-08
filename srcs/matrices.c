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
	scene->matrix_camera[0][0] = 1;
	scene->matrix_camera[1][1] = 1;
	scene->matrix_camera[2][2] = 1;
	scene->matrix_camera[3][3] = 1;
	
	for (int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			scene->matrix_x_rotation[i][j] = 0;
			scene->matrix_y_rotation[i][j] = 0;
			scene->matrix_z_rotation[i][j] = 0;
		}
	}
	scene->matrix_x_rotation[0][0] = 1;
	scene->matrix_x_rotation[1][1] = cos(scene->x_angle * RADIAN);
	scene->matrix_x_rotation[1][2] = -sin(scene->x_angle * RADIAN);
	scene->matrix_x_rotation[2][1] = sin(scene->x_angle * RADIAN);
	scene->matrix_x_rotation[2][2] = cos(scene->x_angle * RADIAN);
		
	scene->matrix_y_rotation[0][0] = cos(scene->y_angle * RADIAN);
	scene->matrix_y_rotation[0][2] = sin(scene->y_angle * RADIAN);
	scene->matrix_y_rotation[1][1] = 1;
	scene->matrix_y_rotation[2][0] = -sin(scene->y_angle * RADIAN);
	scene->matrix_y_rotation[2][2] = cos(scene->y_angle * RADIAN);
	
	scene->matrix_z_rotation[0][0] = cos(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[0][1] = -sin(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[1][0] = sin(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[1][1] = cos(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[2][2] = 1;
}

void	multiplyPointWithMatrix(t_scene *scene, t_vertices *p, float matrix[4][4]){
	(void)scene;
	t_vertices	tmp;
	(void)tmp;
	tmp.matrixed_x	= p->matrixed_x * matrix[0][0] + p->matrixed_y * matrix[1][0] + p->matrixed_z * matrix[2][0] + matrix[3][0];
    tmp.matrixed_y	= p->matrixed_x * matrix[0][1] + p->matrixed_y * matrix[1][1] + p->matrixed_z * matrix[2][1] + matrix[3][1];
    tmp.matrixed_z	= p->matrixed_x * matrix[0][2] + p->matrixed_y * matrix[1][2] + p->matrixed_z * matrix[2][2] + matrix[3][2];
    //float w			= p->matrixed_x * matrix[0][3] + p->matrixed_y * matrix[1][3] + p->matrixed_z * matrix[2][3] + matrix[3][3]; 
	p->matrixed_x = tmp.matrixed_x;
	p->matrixed_y = tmp.matrixed_y;
	p->matrixed_z = tmp.matrixed_z;

    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    /*if (w != 0) { 
        p->matrixed_x /= w; 
        p->matrixed_y /= w; 
        p->matrixed_z /= w; 
    }*/
 
	p->matrixed_x *= scene->zoom;
	p->matrixed_y *= scene->zoom;
	p->matrixed_z *= scene->zoom;

	/*printf("processed : \n");
	printf("\t%f :\t%f\n", p->x, p->matrixed_x);
	printf("\t%f :\t%f\n", p->y, p->matrixed_y);
	printf("\t%f :\t%f\n", p->z, p->matrixed_z);*/
}

void	multiplyPointWithRotationsMatrixes(t_scene *scene, t_vertices *p){
	t_vertices tmp;
    tmp.matrixed_y   = p->matrixed_y * scene->matrix_x_rotation[1][1] + p->matrixed_z * scene->matrix_x_rotation[1][2];
    tmp.matrixed_z   = p->matrixed_y * scene->matrix_x_rotation[2][1] + p->matrixed_z * scene->matrix_x_rotation[2][2];
	p->matrixed_y = tmp.matrixed_y;
	p->matrixed_z = tmp.matrixed_z;
    
	tmp.matrixed_x   = p->matrixed_x * scene->matrix_y_rotation[0][0] + p->matrixed_z * scene->matrix_y_rotation[0][2];
    tmp.matrixed_z   = p->matrixed_x * scene->matrix_y_rotation[2][0] + p->matrixed_z * scene->matrix_y_rotation[2][2];
    p->matrixed_x = tmp.matrixed_x;
	p->matrixed_z = tmp.matrixed_z;
    
	tmp.matrixed_x   = p->matrixed_x * scene->matrix_z_rotation[0][0] + p->matrixed_y * scene->matrix_z_rotation[0][1];
    tmp.matrixed_y   = p->matrixed_x * scene->matrix_z_rotation[1][0] + p->matrixed_y * scene->matrix_z_rotation[1][1];
    p->matrixed_x = tmp.matrixed_x;
	p->matrixed_y = tmp.matrixed_y;
}