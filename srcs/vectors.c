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

float	vector_angle(t_vertices *a, t_vertices *b){
	return (vector_dot_product(a, b) / (vector_magnitude(a) * vector_magnitude(b)));
}