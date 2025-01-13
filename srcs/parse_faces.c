#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

void	add_triangle(char **tab, t_scene *scene);
void	add_quad(char **tab, t_scene *scene);
void	triangulate_polygone(char **tab, size_t tab_size, t_scene *scene);

t_faces	*face_contructor(){
	t_faces *face = malloc(sizeof(struct s_faces));
	add_to_garbage(face);
	face->next = 0x0;
	face->vertices[3] = 0x0;
	face->vertex_normals[3] = 0x0;
	face->texture_coordinates[3] = 0x0;
	face->material = 0x0;
	return (face);
}

void	parse_face(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	if (tab_size < 3){
		dprintf(2,"Face on line %d is not a complete face\n", line_nb);
		free_garbage();
	}
	if (tab_size == 4){
		add_triangle(tab, scene);
	}
	else if (tab_size == 5){
		add_quad(tab, scene);
	}
	else{
		triangulate_polygone(tab, tab_size, scene);
	}
}

void	add_triangle(char **tab, t_scene *scene){
	t_faces *face = face_contructor();
	face->material = scene->objs_list->material;

	scene->objs_list->material->nb_faces++;
	
	scene->display_vertices_count += 3;
	

	for (size_t i=0; i< 3 ; i++){
		face->vertices[i] = 0x0;
		face->vertex_normals[i] = 0x0;
		face->texture_coordinates[i] = 0x0;
		int index = atoi(tab[i+1]);
		int texture_index = 0;
		int vertex_normal_index = 0;
		face->vertices[i] = scene->vertices_tab[index - 1];
		if (tab[i+1][get_int_len(index)] == '/'){
			texture_index = atoi(tab[i+1] + get_int_len(index) + 1);
			if (texture_index == 0){
				face->texture_coordinates[i] = 0x0;
				if (tab[i+1][get_int_len(index)+1] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + 2);
					face->vertex_normals[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
			else {
				face->texture_coordinates[i] = scene->texture_coordinates_tab[texture_index - 1];
				if (tab[i + 1][get_int_len(index) + 1 + get_int_len(texture_index)] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + get_int_len(texture_index) + 2);
					face->vertex_normals[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
		}
	}
	
	if (!scene->objs_list->faces)
		scene->objs_list->faces = face;
	else{
		t_faces *tmp = scene->objs_list->faces;
		while (tmp->next){
			tmp = tmp->next;
		}
		tmp->next = face;
	}
}

void	add_quad(char **tab, t_scene *scene){
	t_faces *f1 = face_contructor();
	t_faces *f2 = face_contructor();
	f1->material = scene->objs_list->material;
	f2->material = scene->objs_list->material;

	scene->objs_list->material->nb_faces += 2;
	scene->display_vertices_count += 6;


	t_vertices *vs[4];
	t_texture_coordinates *vts[4];
	t_vertices *vns[4];
	for (int i = 0; i < 4; i++){
		vts[i] = 0x0;
		vns[i] = 0x0;
	}
	int index = 0, texture_index = 0, vertex_normal_index = 0;
	for (int i =0; i < 4; i++){
		index = atoi(tab[i + 1]);
		vs[i] = scene->vertices_tab[index - 1];

		if (tab[i+1][get_int_len(index)] == '/'){
			texture_index = atoi(tab[i+1] + get_int_len(index) + 1);
			if (texture_index == 0){
				vts[i] = 0x0;
				if (tab[i+1][get_int_len(index)+1] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + 2);
					vns[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
			else {
				vts[i] = scene->texture_coordinates_tab[texture_index - 1];
				if (tab[i + 1][get_int_len(index) + 1 + get_int_len(texture_index)] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + get_int_len(texture_index) + 2);
					vns[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
		}
	}
	f1->vertices[0] = vs[0];
	f1->texture_coordinates[0] = vts[0];
	f1->vertex_normals[0] = vns[0];
	f1->vertices[1] = vs[2];
	f1->texture_coordinates[1] = vts[1];
	f1->vertex_normals[1] = vns[2];
	f1->vertices[2] = vs[1];
	f1->texture_coordinates[2] = vts[2];
	f1->vertex_normals[2] = vns[1];

	f2->vertices[0] = vs[0];
	f2->texture_coordinates[0] = vts[0];
	f2->vertex_normals[0] = vns[0];
	f2->vertices[1] = vs[2];
	f2->texture_coordinates[1] = vts[2];
	f2->vertex_normals[1] = vns[2];
	f2->vertices[2] = vs[3];
	f2->texture_coordinates[2] = vts[3];
	f2->vertex_normals[2] = vns[3];

	f1->next = f2;
	if (!scene->objs_list->faces){
		scene->objs_list->faces = f1;
	}
	else{
		t_faces *tmp = scene->objs_list->faces;
		while (tmp->next){
			tmp = tmp->next;
		}
		tmp->next = f1;
	}
}


short do_segments_intersect(t_vertices *v1, t_vertices *v2, t_vertices *v3, t_vertices *v4){
	t_vertices d1 = get_vector_displacement(v1, v2);
	t_vertices b;
	b.x = v3->x - v1->x;
	b.y = v3->y - v1->y;
	b.z = v3->z - v1->z;
	t_vertices d2 = get_vector_displacement(v3, v4);
	t_vertices product = get_vector_cross_product(&d1, &d2);
	if (product.x != 0 || product.y != 0 || product.z != 0){
		if (get_vector_dot_product(&b, &product)){
			t_vertices scale_product = get_vector_cross_product(&b, &d2);
			double scale = get_vector_dot_product(&scale_product, &product) / (get_vector_dot_product(&product, &product));
			printf("scale %f\n", scale);
			return (scale >= 0 && scale <= 1);
		}
	}
	return (0);
}

void	triangulate_polygone(char **tab, size_t tab_size, t_scene *scene){
	//search possible diagonal
	unsigned int indeces[tab_size];
	t_vertices *diag_start, *diag_end, *seg_start, *seg_end;
	for (size_t i = 1; i < tab_size; i++){
		indeces[i-1] = atoi(tab[i]) - 1;
	}

	unsigned int adjacent_point_left_idx, adjacent_point_right_idx;
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
			for (size_t vertex_idx = 0; vertex_idx < tab_size - 1; vertex_idx++){
				seg_start = scene->vertices_tab[indeces[vertex_idx]];
				seg_end = scene->vertices_tab[indeces[vertex_idx == tab_size - 1 ? 0 : vertex_idx + 1]];
				//check if IJ and current segment are parallels
				if (do_segments_intersect(diag_start, diag_end, seg_start, seg_end))
					continue;
				printf("diagonal possible with point %zu and %zu\n", diag_start->id, diag_end->id);
				break ;
			}
		}
	}
	(void)scene;
}
