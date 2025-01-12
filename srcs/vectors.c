#include "../include/scop.h"

t_vertices	get_vector_displacement(t_vertices *p1, t_vertices *p2){
	t_vertices p;
	p.x = p2->x - p1->x;
	p.y = p2->y - p1->y;
	p.z = p2->z - p1->z;
	return (p);
}

t_vertices	get_vector_cross_product(t_vertices *p1, t_vertices *p2){
	t_vertices p;
	p.x = (p1->y * p2->z) - (p1->z * p2->y);
	p.y = (p1->z * p2->x) - (p1->x * p2->z);
	p.z = (p1->x * p2->y) - (p1->y * p2->x);
	return (p);
}

float	get_vector_dot_product(t_vertices *p1, t_vertices *p2){
	return ((p1->x * p2->x) + (p1->y * p2->y));
}