#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

void	parse_vertices(char **tab, int tab_size, t_scene *scene, int line_nb){
	float	weight = 1.0;
	if (tab_size < 4){
		dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
		free_garbage();
	}
	if (tab[4]){
		weight = atof(tab[4]);
	}
	if (!scene->vertices_current){
		scene->vertices_current = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]), scene);
		scene->vertices_current->w = weight;
		scene->vertices_root = scene->vertices_current;
	}
	else{
		scene->vertices_current->next = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]), scene);
		scene->vertices_current = scene->vertices_current->next;
		scene->vertices_current->w = weight;
	}
	scene->vertices_current->id = scene->vertices_count++;
}

void	parse_vertex_normals(char **tab, int tab_size, t_scene *scene, int line_nb){
	if (tab_size < 4){
		dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
		free_garbage();
	}
	if (!scene->vertex_normals_current){
		scene->vertex_normals_current = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]), 0x0);
		scene->vertex_normals_root = scene->vertex_normals_current;
	}
	else{
		scene->vertex_normals_current->next = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]), 0x0);
		scene->vertex_normals_current = scene->vertex_normals_current->next;
	}
	scene->vertex_normals_count++;
}

void	parse_texture_coordinates(char **tab, int tab_size, t_scene *scene, int line_nb){
		if (tab_size < 2){
			dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
			free_garbage();
		}
		if (!scene->texture_coordinates_current){
			scene->texture_coordinates_current = create_texture_coordinates(tab, tab_size);
			scene->texture_coordinates_root = scene->texture_coordinates_current;
		}
		else{
			scene->texture_coordinates_current->next = create_texture_coordinates(tab, tab_size);
			scene->texture_coordinates_current = scene->texture_coordinates_current->next;
		}
	scene->texture_coordinates_count++;
}

t_vertices *create_vertices(float x, float y, float z, t_scene *scene){
	t_vertices *p = malloc(sizeof(struct s_vertices));
	add_to_garbage(p);
	p->x = x;
	p->y = y;
	p->z = z;
	p->w = 1.0;
	p->next = 0x0;
	p->focused = 0;
	p->magnetude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	if (scene){
		scene->max_x = x > scene->max_x ? x : scene->max_x;
		scene->min_x = x < scene->min_x ? x : scene->min_x;

		scene->max_y = y > scene->max_y ? y : scene->max_y;
		scene->min_y = y < scene->min_y ? y : scene->min_y;

		scene->max_z = z > scene->max_z ? z : scene->max_z;
		scene->min_z = z < scene->min_z ? z : scene->min_z;
	}
	return (p);
}

t_texture_coordinates *create_texture_coordinates(char **tab, int tab_size){
	t_texture_coordinates *t = malloc(sizeof(struct s_texture_coordinates));
	add_to_garbage(t);
	t->u = atof(tab[1]);
	if (tab_size >= 3)
		t->v = atof(tab[2]);
	if (tab_size >= 4)
		t->w = atof(tab[3]);
	t->next = 0x0;
	return (t);
}
