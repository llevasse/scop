#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

void	set_view_matrix(t_scene *scene){
	float	eyes[3] = {scene->x_offset, scene->y_offset, scene->z_offset + -(scene->max_z - scene->min_z)};
	float	center[3] = {scene->x_offset,scene->y_offset,scene->z_offset};
	float	up[3] = {0, 1, 0};


	float	f[3] = {(center[0]-eyes[0]) * (1.0 / 3.0), (center[1]-eyes[1]) * (1.0 / 3.0), (center[2]-eyes[2]) * (1.0 / 3.0)};
	float	s[3] = {(f[1] * up[2] - f[2] * up[1]) * (1.0 / 3.0), (f[2] * up[0] - f[0] * up[2]) * (1.0 / 3.0), (f[0] * up[1] - f[1] * up[0]) * (1.0 / 3.0)}; //normalized cross product of f and up
	float	u[3] = {(s[1] * f[2] - s[2] * f[1]), (s[2] * f[0] - s[0] * f[2]), (s[0] * f[1] - s[1] * f[0])};

	scene->matrix_camera[0][0] = s[0];
	scene->matrix_camera[1][0] = s[1];
	scene->matrix_camera[2][0] = s[2];

	scene->matrix_camera[0][1] = u[0];
	scene->matrix_camera[1][1] = u[1];
	scene->matrix_camera[2][1] = u[2];

	scene->matrix_camera[0][2] = -f[0];
	scene->matrix_camera[1][2] = -f[1];
	scene->matrix_camera[2][2] = -f[2];

	scene->matrix_camera[3][0] = -(s[0] * eyes[0]) + (s[1] * eyes[1]) + (s[2] * eyes[2]);
	scene->matrix_camera[3][1] = -(u[0] * eyes[0]) + (u[1] * eyes[1]) + (u[2] * eyes[2]);
	scene->matrix_camera[3][2] = (f[0] * eyes[0]) + (f[1] * eyes[1]) + (f[2] * eyes[2]);
}

void	f4x4MatrixMult(float (*m)[4][4], float m1[4][4], float m2[4][4]){
	float	result[4][4];
	result[0][0] = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3];
	result[0][1] = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3];
	result[0][2] = m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3];
	result[0][3] = m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3];

	result[1][0] = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3];
	result[1][1] = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3];
	result[1][2] = m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3];
	result[1][3] = m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3];

	result[2][0] = m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3];
	result[2][1] = m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3];
	result[2][2] = m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3];
	result[2][3] = m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3];

	result[3][0] = m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3];
	result[3][1] = m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3];
	result[3][2] = m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3];
	result[3][3] = m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3];

	(*m)[0][0] = result[0][0];
	(*m)[0][1] = result[0][1];
	(*m)[0][2] = result[0][2];
	(*m)[0][3] = result[0][3];
	(*m)[1][0] = result[1][0];
	(*m)[1][1] = result[1][1];
	(*m)[1][2] = result[1][2];
	(*m)[1][3] = result[1][3];
	(*m)[2][0] = result[2][0];
	(*m)[2][1] = result[2][1];
	(*m)[2][2] = result[2][2];
	(*m)[2][3] = result[2][3];
	(*m)[3][0] = result[3][0];
	(*m)[3][1] = result[3][1];
	(*m)[3][2] = result[3][2];
	(*m)[3][3] = result[3][3];
}

void	setMatrix(t_scene *scene){
	float scale = 1 / tanf((scene->fov * 0.5)); 
	float	identity[4][4] = {	{1,0,0,0},
								{0,1,0,0},
								{0,0,1,0},
								{0,0,0,1}};
	for (int i =0;i < 4;i++){
		for (int j=0; j<4;j++){
			scene->matrix[i][j] = 0.0;
			scene->persepective_matrix[i][j] = 0.0;
			scene->matrix_camera[i][j] = 0.0;
		}
	}

	scene->persepective_matrix[0][0] = ((float)scene->height / (float)scene->width) * scale;

	scene->persepective_matrix[1][1] = scale;

	scene->persepective_matrix[2][2] = (scene->far_plane_distance) / (scene->near_plane_distance - scene->far_plane_distance);
	scene->persepective_matrix[2][3] = -1;

	scene->persepective_matrix[3][2] = -((scene->far_plane_distance) * scene->near_plane_distance) / (scene->far_plane_distance - scene->near_plane_distance);


	scene->matrix_camera[0][0] = 1;
	scene->matrix_camera[1][1] = 1;
	scene->matrix_camera[2][2] = 1;
	scene->matrix_camera[3][3] = 1;
	set_view_matrix(scene);
	f4x4MatrixMult(&scene->matrix, identity, scene->matrix_camera);
	f4x4MatrixMult(&scene->matrix, scene->persepective_matrix, scene->matrix);
	for (int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			scene->matrix_x_rotation[i][j] = 0;
			scene->matrix_y_rotation[i][j] = 0;
			scene->matrix_z_rotation[i][j] = 0;
			scene->scale_matrix[i][j] = 0;
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

	scene->scale_matrix[0][0] = scene->x_scale;
	scene->scale_matrix[1][1] = scene->y_scale;
	scene->scale_matrix[2][2] = scene->z_scale;
	scene->scale_matrix[3][3] = 1;
}

void	multiplyPointWithMatrix(t_scene *scene, t_vertices *p, float matrix[4][4]){
	(void)scene;
	t_vertices	tmp;
	(void)tmp;
	tmp.matrixed_x	= p->matrixed_x * matrix[0][0] + p->matrixed_y * matrix[1][0] + p->matrixed_z * matrix[2][0] + matrix[3][0];
    tmp.matrixed_y	= p->matrixed_x * matrix[0][1] + p->matrixed_y * matrix[1][1] + p->matrixed_z * matrix[2][1] + matrix[3][1];
    tmp.matrixed_z	= p->matrixed_x * matrix[0][2] + p->matrixed_y * matrix[1][2] + p->matrixed_z * matrix[2][2] + matrix[3][2];
	p->matrixed_x = tmp.matrixed_x;
	p->matrixed_y = tmp.matrixed_y;
	p->matrixed_z = tmp.matrixed_z;
    float w			= p->matrixed_x * matrix[0][3] + p->matrixed_y * matrix[1][3] + p->matrixed_z * matrix[2][3] + matrix[3][3]; 

    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 0) { 
        p->matrixed_x /= w; 
        p->matrixed_y /= w; 
        p->matrixed_z /= w; 
    }

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