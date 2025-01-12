#include "../include/scop.h"

short	are_line_coplanar(t_vertices *p1, t_vertices *p2, t_vertices *p3, t_vertices *p4){
	t_vertices l1_d = get_vector_displacement(p1, p2);
	t_vertices l2_d = get_vector_displacement(p3, p4);
	
	t_vertices l1_m = get_vector_cross_product(p1, p2);
	t_vertices l2_m = get_vector_cross_product(p3, p4);
	
	return (!(get_vector_dot_product(&l1_d, &l2_m) + get_vector_dot_product(&l2_d, &l1_m)));
}
