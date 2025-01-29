#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
extern short			key_press[348];

void	d4x4MatrixMult(float (*m)[4][4], float m1[4][4], float m2[4][4]){
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

void	inverseF4x4Matrix(float (*m)[4][4]){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			(*m)[i][j] = -((*m)[i][j]);
		}
	}
}

void	setMatrix(t_scene *scene){
	float	rotation[4][4];

	float	scale = 1 / tanf((scene->fov * 0.5) * RADIAN); 
	float	aspect_ratio = scene->width / scene->height;
	float	near_z = .01, far_z = 1000, z_range = near_z - far_z;
	float	A = (far_z + near_z) / z_range;
	float	B = (2.0f * far_z * near_z) / z_range;
	for (int i =0;i < 4;i++){
		for (int j=0; j<4;j++){	// set every matrix as identiy matrices
			scene->matrix[i][j] = i == j;
			scene->persepective_matrix[i][j] = i == j;
			scene->matrix_camera[i][j] = i == j;
			scene->scale_matrix[i][j] = i == j;
			scene->translation_matrix[i][j] = i == j;
			scene->matrix_x_rotation[i][j] = i == j;
			scene->matrix_y_rotation[i][j] = i == j;
			scene->matrix_z_rotation[i][j] = i == j;
			scene->model_matrix[i][j] = i == j;
			rotation[i][j] = i == j;
		}
	}

	scene->persepective_matrix[0][0] = scale;

	scene->persepective_matrix[1][1] = scale / aspect_ratio;

	scene->persepective_matrix[2][2] = A;
	scene->persepective_matrix[2][3] = -1;
	scene->persepective_matrix[3][2] = B;

	scene->persepective_matrix[3][3] = 0;
	
	d4x4MatrixMult(&scene->matrix, scene->persepective_matrix, scene->matrix_camera);
	scene->matrix_x_rotation[1][1] = cos(scene->x_angle * RADIAN);
	scene->matrix_x_rotation[1][2] = -sin(scene->x_angle * RADIAN);
	scene->matrix_x_rotation[2][1] = sin(scene->x_angle * RADIAN);
	scene->matrix_x_rotation[2][2] = cos(scene->x_angle * RADIAN);
		
	scene->matrix_y_rotation[0][0] = cos(scene->y_angle * RADIAN);
	scene->matrix_y_rotation[0][2] = sin(scene->y_angle * RADIAN);
	scene->matrix_y_rotation[2][0] = -sin(scene->y_angle * RADIAN);
	scene->matrix_y_rotation[2][2] = cos(scene->y_angle * RADIAN);
	
	scene->matrix_z_rotation[0][0] = cos(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[0][1] = -sin(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[1][0] = sin(scene->z_angle * RADIAN);
	scene->matrix_z_rotation[1][1] = cos(scene->z_angle * RADIAN);

	scene->scale_matrix[0][0] = scene->x_scale;
	scene->scale_matrix[1][1] = scene->y_scale;
	scene->scale_matrix[2][2] = scene->z_scale;

	scene->translation_matrix[3][0] = scene->x_offset;
	scene->translation_matrix[3][1] = scene->y_offset;
	scene->translation_matrix[3][2] = scene->z_offset;

	d4x4MatrixMult(&rotation, scene->matrix_x_rotation, scene->matrix_z_rotation);
	d4x4MatrixMult(&rotation, rotation, scene->matrix_y_rotation);


	d4x4MatrixMult(&scene->model_matrix, scene->translation_matrix, rotation);
	d4x4MatrixMult(&scene->model_matrix, scene->model_matrix, scene->scale_matrix);
	d4x4MatrixMult(&scene->matrix, scene->model_matrix, scene->matrix_camera);
}