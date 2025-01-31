#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

void	add_triangle(char **tab, t_scene *scene, int line_nb);
void	add_quad(char **tab, t_scene *scene, int line_nb);

t_faces	*face_contructor(){
	t_faces *face = malloc(sizeof(struct s_faces));
	add_to_garbage(face);
	face->next = 0x0;
	face->vertices[3] = 0x0;
	face->vertex_normals[3] = 0x0;
	face->texture_coordinates[3] = 0x0;
	face->material = 0x0;
	face->focused = 0;
	face->direction = 0;
	return (face);
}

void	parse_face(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	if (tab_size < 3){
		dprintf(2,"Face on line %d is not a complete face\n", line_nb);
		free_garbage();
	}
	if (tab_size == 4){
		add_triangle(tab, scene, line_nb);
	}
	else if (tab_size == 5){
		add_quad(tab, scene, line_nb);
	}
	else{
		dprintf(2,"Face on line %d has too many vertex :(\n", line_nb);
		free_garbage();
	}
}

void	map_face_uv(t_faces *face){
	t_vertices u = subtract_vectors(face->vertices[1], face->vertices[0]);
	t_vertices v = subtract_vectors(face->vertices[2], face->vertices[0]);
	face->normal = vector_cross_product(&u, &v);

	normalize_vector(&face->normal);

	if (fabsf(face->normal.x) < fabsf(face->normal.z))
		face->direction = 'x';
	else
		face->direction = 'z';
	if (fabsf(face->normal.y) >= .9)
		face->direction = 'y';
}

void	add_triangle(char **tab, t_scene *scene, int line_nb){
	t_faces *face = face_contructor();
	face->material = scene->objs_list->material;

	scene->objs_list->material->nb_faces++;
	
	scene->display_vertices_count += 3;
	

	for (size_t i=0; i< 3 ; i++){
		face->vertices[i] = 0x0;
		face->vertex_normals[i] = 0x0;
		face->texture_coordinates[i] = 0x0;
		size_t index = atoi(tab[i+1]);
		size_t texture_index = 0;
		size_t vertex_normal_index = 0;
		if (index > scene->vertices_count){
			dprintf(2,"Face on line %d call un declared vertex :(\n", line_nb);
			free_garbage();
		}
		face->vertices[i] = scene->vertices_tab[index - 1];
		if (tab[i+1][get_int_len(index)] == '/'){
			texture_index = atoi(tab[i+1] + get_int_len(index) + 1);
			if (texture_index == 0){
				face->texture_coordinates[i] = 0x0;
				if (tab[i+1][get_int_len(index)+1] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + 2);
					if (vertex_normal_index > scene->vertex_normals_count){
						dprintf(2,"Face on line %d call un declared vertex normal :(\n", line_nb);
						free_garbage();
					}
					face->vertex_normals[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
			else {
				if (texture_index > scene->texture_coordinates_count){
					dprintf(2,"Face on line %d call un declared texture coordinate :(\n", line_nb);
					free_garbage();
				}
				face->texture_coordinates[i] = scene->texture_coordinates_tab[texture_index - 1];
				if (tab[i + 1][get_int_len(index) + 1 + get_int_len(texture_index)] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + get_int_len(texture_index) + 2);
					if (vertex_normal_index > scene->vertex_normals_count){
						dprintf(2,"Face on line %d call un declared vertex normal :(\n", line_nb);
						free_garbage();
					}
					face->vertex_normals[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
		}
	}

	map_face_uv(face);

	if (!scene->objs_list->faces)
		scene->objs_list->faces = face;
	else{
		t_faces *tmp = scene->objs_list->faces;
		while (tmp->next){
			tmp = tmp->next;
		}
		tmp->next = face;
	}
//	printf("Added face with vertex %zu %zu %zu\n", face->vertices[0]->id, face->vertices[1]->id, face->vertices[2]->id);
}

void	add_quad(char **tab, t_scene *scene, int line_nb){
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
	size_t index = 0, texture_index = 0, vertex_normal_index = 0;
	for (int i =0; i < 4; i++){
		index = atoi(tab[i + 1]);
		if (index > scene->vertices_count){
			dprintf(2,"Face on line %d call un declared vertex :(\n", line_nb);
			free_garbage();
		}
		vs[i] = scene->vertices_tab[index - 1];

		if (tab[i+1][get_int_len(index)] == '/'){
			texture_index = atoi(tab[i+1] + get_int_len(index) + 1);
			if (texture_index == 0){
				vts[i] = 0x0;
				if (tab[i+1][get_int_len(index)+1] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + 2);
					if (vertex_normal_index > scene->vertex_normals_count){
						dprintf(2,"Face on line %d call un declared vertex normal :(\n", line_nb);
						free_garbage();
					}
					vns[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
			else {
				if (texture_index > scene->texture_coordinates_count){
					dprintf(2,"Face on line %d call un declared texture coordinate :(\n", line_nb);
					free_garbage();
				}
				vts[i] = scene->texture_coordinates_tab[texture_index - 1];
				if (tab[i + 1][get_int_len(index) + 1 + get_int_len(texture_index)] == '/'){
					vertex_normal_index = atoi(tab[i+1] + get_int_len(index) + get_int_len(texture_index) + 2);
					if (vertex_normal_index > scene->vertex_normals_count){
						dprintf(2,"Face on line %d call un declared vertex normal :(\n", line_nb);
						free_garbage();
					}
					vns[i] = scene->vertex_normals_tab[vertex_normal_index - 1];
				}
			}
		}
	}
	f1->vertices[0] = vs[0];
	f1->texture_coordinates[0] = vts[0];
	f1->vertex_normals[0] = vns[0];
	f1->vertices[1] = vs[1];
	f1->texture_coordinates[1] = vts[1];
	f1->vertex_normals[1] = vns[2];
	f1->vertices[2] = vs[2];
	f1->texture_coordinates[2] = vts[2];
	f1->vertex_normals[2] = vns[1];

	f2->vertices[0] = vs[3];
	f2->texture_coordinates[0] = vts[0];
	f2->vertex_normals[0] = vns[0];
	f2->vertices[1] = vs[0];
	f2->texture_coordinates[1] = vts[2];
	f2->vertex_normals[1] = vns[2];
	f2->vertices[2] = vs[2];
	f2->texture_coordinates[2] = vts[3];
	f2->vertex_normals[2] = vns[3];

	map_face_uv(f1);
	map_face_uv(f2);

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
