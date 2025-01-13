#include "../include/scop.h"

t_vertices	get_vector_at_distance(t_vertices *start, t_vertices *end, float dist){
	t_vertices p;
	p.x = ((1 - dist) * start->x) + (dist * end->x);
	p.y = ((1 - dist) * start->y) + (dist * end->y);
	p.z = ((1 - dist) * start->z) + (dist * end->z);
	return (p);
}

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
	//printf("p1 %f %f %f | p2 %f %f %f\n", p1->x, p1->y, p1->z, p2->x, p2->y, p2->z);
	return((p1->x * p2->x) + (p1->y * p2->y) + (p1->z * p2->z));
}