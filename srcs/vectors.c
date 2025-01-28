#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
extern struct s_scene	*scene;
extern GLfloat			*g_vertex_buffer_data;

float	vector_dot_product(t_vertices *a, t_vertices *b){
	return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

float	matrixed_vector_dot_product(t_vertices *a, t_vertices *b){
	return ((a->matrixed_x * b->matrixed_x) + (a->matrixed_y * b->matrixed_y) + (a->matrixed_z * b->matrixed_z));
}

float	vector_magnitude(t_vertices *v){
	return (sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));
}

float	matrixed_vector_magnitude(t_vertices *v){
	return (sqrtf((v->matrixed_x * v->matrixed_x) + (v->matrixed_y * v->matrixed_y) + (v->matrixed_z * v->matrixed_z)));
}

t_vertices	vector_cross_product(t_vertices *v1, t_vertices *v2){
	t_vertices v;
	v.x = (v1->y * v1->z) - (v1->z * v2->y);
	v.y = (v1->z * v1->x) - (v1->x * v2->z);
	v.z = (v1->x * v1->y) - (v1->y * v2->x);
	return (v);
}

t_vertices	vector_matrixed_cross_product(t_vertices *v1, t_vertices *v2){
	t_vertices v;
	v.x = (v1->matrixed_y * v1->matrixed_z) - (v1->matrixed_z * v2->matrixed_y);
	v.y = (v1->matrixed_z * v1->matrixed_x) - (v1->matrixed_x * v2->matrixed_z);
	v.z = (v1->matrixed_x * v1->matrixed_y) - (v1->matrixed_y * v2->matrixed_x);
	return (v);
}

t_vertices	subtract_vectors(t_vertices *v1, t_vertices *v2){
	t_vertices v;
	v.x = v1->x - v2->x;
	v.y = v1->y - v2->y;
	v.z = v1->z - v2->z;
	return (v);
}

t_vertices	subtract_matrixed_vectors(t_vertices *v1, t_vertices *v2){
	t_vertices v;
	v.x = v1->matrixed_x - v2->matrixed_x;
	v.y = v1->matrixed_y - v2->matrixed_y;
	v.z = v1->matrixed_z - v2->matrixed_z;
	return (v);
}