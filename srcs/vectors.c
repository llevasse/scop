#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
extern struct s_scene	*scene;
extern GLfloat			*g_vertex_buffer_data;

float	vector_dot_product(t_vertices *a, t_vertices *b){
	return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

float	vector_magnitude(t_vertices *v){
	return (sqrtf((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));
}

t_vertices	vector_cross_product(t_vertices *v1, t_vertices *v2){
	t_vertices v;
	v.x = (v1->y * v2->z) - (v1->z * v2->y);
	v.y = (v1->z * v2->x) - (v1->x * v2->z);
	v.z = (v1->x * v2->y) - (v1->y * v2->x);
	return (v);
}

t_vertices	subtract_vectors(t_vertices *v1, t_vertices *v2){
	t_vertices v;
	v.x = v1->x - v2->x;
	v.y = v1->y - v2->y;
	v.z = v1->z - v2->z;
	return (v);
}


void	normalize_vector(t_vertices *p){
	float magnitude = vector_magnitude(p);

	p->x /= magnitude;
	p->y /= magnitude;
	p->z /= magnitude;
}