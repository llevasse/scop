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
	scene->matrix[2][2] = -scene->far_plane_distance / (scene->far_plane_distance - scene->near_plane_distance);
	scene->matrix[2][3] = -1;

	scene->matrix[3][0] = 0;
	scene->matrix[3][1] = 0;
	scene->matrix[3][2] = -scene->far_plane_distance * scene->near_plane_distance / (scene->far_plane_distance - scene->near_plane_distance);
	scene->matrix[3][3] = 0;
}

void	multiplyPointWithMatric(t_scene *scene, t_vertices *p){
	t_vertices out;
	out.x   = p->x * scene->matrix[0][0] + p->y * scene->matrix[1][0] + p->z * scene->matrix[2][0] + scene->matrix[3][0]; 
    out.y   = p->x * scene->matrix[0][1] + p->y * scene->matrix[1][1] + p->z * scene->matrix[2][1] + scene->matrix[3][1]; 
    out.z   = p->x * scene->matrix[0][2] + p->y * scene->matrix[1][2] + p->z * scene->matrix[2][2] + scene->matrix[3][2]; 
    float w = p->x * scene->matrix[0][3] + p->y * scene->matrix[1][3] + p->z * scene->matrix[2][3] + scene->matrix[3][3]; 
 
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1) { 
        out.x /= w; 
        out.y /= w; 
        out.z /= w; 
    } 
	p->x = out.x;
	p->y = out.y;
	p->z = out.z;
}