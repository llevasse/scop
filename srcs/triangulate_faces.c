#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;


short do_segments_intersect(t_vertices *v1, t_vertices *v2, t_vertices *v3, t_vertices *v4, int print){
	t_vertices E, F, P, S;
	E = get_vector_displacement(v1, v2);
	F = get_vector_displacement(v3, v4);
	S = get_vector_displacement(v1, v3);
	P = E;
	P.y = -E.y;
	double scale = get_vector_dot_product(&S, &P) / get_vector_dot_product(&F, &P);
	if (print)
		printf("scale %f\n", scale);
	return (scale > 0 && scale < 1);
}

short	is_segment_in_polygon(char **tab, size_t tab_size, t_scene *scene, t_vertices *diag_start, t_vertices *diag_end){
	unsigned int	indeces[tab_size], max_idx = 0;
	float			max_x = 0, max_y = 0, max_z = 0;
	for (size_t i = 1; i < tab_size; i++){
		indeces[i-1] = atoi(tab[i]) - 1;
		max_idx = indeces[i - 1] > max_idx ? indeces[i - 1] : max_idx;
		max_x = fabsf(scene->vertices_tab[indeces[i - 1]]->x) > max_x ? fabsf(scene->vertices_tab[indeces[i - 1]]->x) : max_y;
		max_y = fabsf(scene->vertices_tab[indeces[i - 1]]->y) > max_y ? fabsf(scene->vertices_tab[indeces[i - 1]]->y) : max_y;
		max_z = fabsf(scene->vertices_tab[indeces[i - 1]]->z) > max_z ? fabsf(scene->vertices_tab[indeces[i - 1]]->z) : max_y;
	}
//	printf("check if diagonal (v%zu v%zu) is in polygon\n", diag_start->id, diag_end->id);
	unsigned int cross_count_x = 0, cross_count_y = 0, cross_count_z = 0;
	t_vertices mid_point = get_vector_at_distance(diag_start, diag_end, .5);
	t_vertices mid_point_x = mid_point;
	t_vertices mid_point_y = mid_point;
	t_vertices mid_point_z = mid_point;
	mid_point_x.x += max_x;
	mid_point_y.y += max_y;
	mid_point_z.z += max_z;
	for (size_t vertex_idx = 0; vertex_idx < tab_size - 1; vertex_idx++){
		t_vertices s1 = *scene->vertices_tab[indeces[vertex_idx]];
		t_vertices s2 = *scene->vertices_tab[indeces[vertex_idx == tab_size - 2 ? 0 : vertex_idx + 1]];
//		printf("\tcheck if diagonal cross (v%zu v%zu)\n", s1.id, s2.id);
		if (do_segments_intersect(&mid_point, &mid_point_x, &s1, &s2, 1)){
			cross_count_x++;
			printf("\t diagonal cross X (v%zu v%zu)\n", s1.id, s2.id);
		}
		if (do_segments_intersect(&mid_point, &mid_point_y, &s1, &s2, 1)){
			cross_count_y++;
			printf("\t diagonal cross Y (v%zu v%zu)\n", s1.id, s2.id);
		}
		if (do_segments_intersect(&mid_point, &mid_point_z, &s1, &s2, 1)){
			cross_count_z++;
			printf("\t diagonal cross Z (v%zu v%zu)\n", s1.id, s2.id);
		}
	}
	printf("\033[0;32m\tdiagonal (v%zu v%zu) possible (crossX = %u, crossY = %u, crossZ = %u)\033[0m\n", diag_start->id, diag_end->id, cross_count_x, cross_count_y, cross_count_z);
	if (/*cross_count_x % 2 || cross_count_y % 2 || */cross_count_z % 2){
		return (1);
	}
	return (0);
}

void	split_polygon(char **tab, size_t tab_size, t_scene *scene, short **possible_diag_tab, t_vertices *diag_start, t_vertices *diag_end){
	size_t	new_face_vertex_count = 0;
	short	is_new_face = 0;
	size_t current_idx;
	for (size_t i=1; i<tab_size; i++){
		current_idx = atoi(tab[i]) - 1;
		if (current_idx == diag_start->id || current_idx == diag_end->id){
			new_face_vertex_count++;
			is_new_face = is_new_face ? 0 : 1;
		}
		else if (is_new_face){
			new_face_vertex_count++;
		}
	}
	
	char	*face1[new_face_vertex_count + 1], *face2[((tab_size - 1) - new_face_vertex_count + 2) + 1];
	face1[0] = "f";
	face2[0] = "f";
	size_t face1_idx = 1, face2_idx = 1;
	for (size_t i=1; i<tab_size; i++){
		current_idx = atoi(tab[i]) - 1;
		if (current_idx == diag_start->id || current_idx == diag_end->id){
			face1[face1_idx++] = tab[i];
			is_new_face = is_new_face ? 0 : 1;
			face2[face2_idx++] = tab[i];
		}
		else if (is_new_face){
			face1[face1_idx++] = tab[i];
		}
		else{
			face2[face2_idx++] = tab[i];
		}
	}
	(void)scene;
	parse_face(face1, face1_idx, scene, 0, possible_diag_tab);
	parse_face(face2, face2_idx, scene, 0, possible_diag_tab);
}

//check for all possible diags at first iteration to avoid unnecessary calculation
short	**get_all_possible_diags(char **tab, size_t tab_size, t_scene *scene, unsigned int *indeces, unsigned int max_index){
	t_vertices		*diag_start, *diag_end, *seg_start, *seg_end;
	short			**possible_diag_tab = malloc(sizeof(short *) * (max_index + 1));
	add_to_garbage(possible_diag_tab);
	for (size_t i=0; i <= max_index; i++){
		possible_diag_tab[i] = malloc(sizeof(short) * max_index);
		add_to_garbage(possible_diag_tab[i]);
		for (size_t j=0; j < max_index; j++){
			possible_diag_tab[i][j] = 0;
		}	
	}
	unsigned int adjacent_point_left_idx, adjacent_point_right_idx;
	short			cross = 0, found = 0;
	for (size_t i = 0; i < tab_size - 1; i++){
		if (i == 0){
			adjacent_point_left_idx = indeces[tab_size - 2];
			adjacent_point_right_idx = indeces[i + 1];
		}
		else if (i == tab_size - 2){
			adjacent_point_left_idx = indeces[i - 1];
			adjacent_point_right_idx = indeces[0];
		}
		else{
			adjacent_point_left_idx = indeces[i - 1];
			adjacent_point_right_idx = indeces[i + 1];
		}
		//printf("point %u, left : %u, right : %u\n", indeces[i], adjacent_point_left_idx, adjacent_point_right_idx);
		for (size_t j = 0; j < tab_size - 1; j++){
			if (i == j || indeces[j] == adjacent_point_left_idx || indeces[j] == adjacent_point_right_idx)
				continue;
			// test diagonal IJ against every segement
			diag_start = scene->vertices_tab[indeces[i]];
			diag_end = scene->vertices_tab[indeces[j]];
			cross = 0;
			for (size_t vertex_idx = 0; vertex_idx < tab_size - 1; vertex_idx++){
				seg_start = scene->vertices_tab[indeces[vertex_idx]];
				seg_end = scene->vertices_tab[indeces[vertex_idx == tab_size - 2 ? 0 : vertex_idx + 1]];

				if (seg_start->id == diag_start->id || seg_start->id == diag_end->id || seg_end->id == diag_start->id || seg_end->id == diag_end->id)
					continue;

//				printf("check if diagonal (v%zu v%zu) cross (v%zu v%zu)\n", diag_start->id, diag_end->id, seg_start->id, seg_end->id);
				if (do_segments_intersect(diag_start, diag_end, seg_start, seg_end, 0)){
//					printf("\033[0;31m\tdiagonal (v%zu v%zu) cross (v%zu v%zu)\033[0m\n", diag_start->id, diag_end->id, seg_start->id, seg_end->id);
					cross = 1;
					break ;
				}
			}
			if (!cross){
				found = is_segment_in_polygon(tab, tab_size, scene, diag_start, diag_end);
				if (found){
					printf("\033[0;32m\tdiagonal (v%zu v%zu) possible\033[0m\n", diag_start->id, diag_end->id);
					possible_diag_tab[diag_start->id][diag_end->id] += 1;
				}
			}
		}
	}
	return (possible_diag_tab);
}

void	triangulate_polygone(char **tab, size_t tab_size, t_scene *scene, short **possible_diag_tab, int line_nb){
	//search possible diagonal
	unsigned int	indeces[tab_size];
	t_vertices		*p1 = 0x0, *p2 = 0x0;
	size_t			max_idx = 0;
	for (size_t i = 1; i < tab_size; i++){
		indeces[i-1] = atoi(tab[i]) - 1;
		max_idx = indeces[i - 1] > max_idx ? indeces[i - 1] : max_idx;
	}
	if (!possible_diag_tab)
		possible_diag_tab = get_all_possible_diags(tab, tab_size, scene, indeces, max_idx);
	printf("\n  ");
	for (size_t i = 0; i<= max_idx; i++){
		printf("%zu ", i);
	}
	printf("\n");
	for (size_t i=0; i <= max_idx; i++){
		for (size_t j=0; j <= max_idx; j++){
			if (j == 0)
				printf("%zu ", i);
			printf("%d ", possible_diag_tab[i][j]);
			if (possible_diag_tab[i][j] && !p1){
				p1 = scene->vertices_tab[i];
				p2 = scene->vertices_tab[j];
				possible_diag_tab[i][j] = 0;
				possible_diag_tab[j][i] = 0;
			}
		}	
		printf("\n");
	}
	printf("\n");

	if (p1)
		split_polygon(tab, tab_size, scene, possible_diag_tab, p1, p2);
	else{
		dprintf(2, "C'ant triangulate face on line %d\n", line_nb);
		add_to_garbage(0x0);
	}
}
