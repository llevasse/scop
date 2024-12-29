#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

void	parse_face(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	if (tab_size < 3){
		dprintf(2,"Face on line %d is not a complete face\n", line_nb);
		free_garbage();
	}
	t_faces *face = malloc(sizeof(struct s_faces));
	face->next = 0x0;
	add_to_garbage(face);
	face->vertices = malloc((tab_size) * sizeof(t_vertices *));
	scene->display_vertices_count += tab_size;
	add_to_garbage(face->vertices);
	face->vertex_normals = malloc((tab_size) * sizeof(t_vertices *));
	add_to_garbage(face->vertex_normals);
	face->texture_coordinates = malloc((tab_size) * sizeof(t_texture_coordinates *));
	add_to_garbage(face->texture_coordinates);
	face->material = scene->objs_list->material;
	scene->objs_list->material->nb_faces++;
	
	face->vertices[tab_size-1] = 0x0;
	face->vertex_normals[tab_size-1] = 0x0;
	face->texture_coordinates[tab_size-1] = 0x0;
	
	for (size_t i=0; i< tab_size - 1 ; i++){
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
