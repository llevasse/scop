#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

t_scene *parse_scene(int fd){
 	t_scene *scene = malloc(sizeof(struct s_scene));
	add_to_garbage(scene);
 	t_scene_parser *scene_parser = malloc(sizeof(struct s_scene_parser));
	add_to_garbage(scene_parser);
 	t_obj *root_obj_list = 0x0;
	scene->objs_list = 0x0;
	scene_parser = 0x0;
	scene->vertices_root = 0x0;
	scene->vertices_current = 0x0;
	scene->vertices_tab = 0x0;
	scene->vertices_count = 0;
	scene->vertex_normals_root = 0x0;
	scene->vertex_normals_current = 0x0;
	scene->vertex_normals_tab = 0x0;
	scene->vertex_normals_count = 0;
	scene->texture_coordinates_root = 0x0;
	scene->texture_coordinates_current = 0x0;
	scene->texture_coordinates_tab = 0x0;
	scene->texture_coordinates_count = 0;
	char	*s = get_next_line(fd);
	char	**line_content = 0x0;
	size_t	line_content_size = 0;
	size_t	line_count = 1;
	while (s){
		line_content = ft_split(s, ' ');	//TODO add tab to garbage collector
		add_to_garbage(line_content);
		line_content_size = 0;
		for (; line_content[line_content_size]; line_content_size++){
			add_to_garbage(line_content[line_content_size]);
		}
		if (*s != '#' && line_content[0]){
			if (!line_content){
				dprintf(2, "Couldn't allocate memory\n");
				exit(1);
			}
			if (!ft_strcmp(line_content[0], "mtllib")){
				if (line_content[1]){
					scene->material_list = parse_mtl(line_content[1]);
					if (!scene->material_list){
						free(s);
						free_garbage();
					}

				}
				else{
					dprintf(2,"Missing path to .mtl file");
					free(s);
					free_garbage();
				}
			}
			else {
				parse_scene_line(line_content, line_content_size, scene, line_count);
				if (!root_obj_list && scene->objs_list)
					root_obj_list = scene->objs_list;
			}
		}
		if (line_content){
			line_content = 0x0;
			line_content_size = 0;
		}
		free(s);
		s = get_next_line(fd);
		line_count++;
	}
	return (scene);
}

void	parse_vertices(char **tab, int tab_size, t_scene *scene, int line_nb){
	if (tab_size < 4){
		dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
		free_garbage();
	}
	if (!scene->vertices_current){
		scene->vertices_current = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]));
		scene->vertices_root = scene->vertices_current;
	}
	else{
		scene->vertices_current->next = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]));
		scene->vertices_current = scene->vertices_current->next;
	}
	scene->vertices_count++;
}

void	parse_vertex_normals(char **tab, int tab_size, t_scene *scene, int line_nb){
	if (tab_size < 4){
		dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
		free_garbage();
	}
	if (!scene->vertex_normals_current){
		scene->vertex_normals_current = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]));
		scene->vertex_normals_root = scene->vertex_normals_current;
	}
	else{
		scene->vertex_normals_current->next = create_vertices(atof(tab[1]), atof(tab[2]), atof(tab[3]));
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

t_vertices *create_vertices(float x, float y, float z){
	t_vertices *p = malloc(sizeof(struct s_vertices));
	add_to_garbage(p);
	p->x = x;
	p->y = y;
	p->z = z;
	p->next = 0x0;
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

void	pass_obj_list_to_tab(t_scene *scene){
	size_t i=0;
	if (scene->vertices_root){
		if (scene->vertices_tab){
			t_vertices	**tmp = malloc((scene->vertices_count + 1) * sizeof(t_vertices *));
			add_to_garbage(tmp);
			while (scene->vertices_tab[i]){
				tmp[i] = scene->vertices_tab[i];
				i++;
			}
			scene->vertices_tab = tmp;
		}
		else{
			scene->vertices_tab = malloc((scene->vertices_count + 1) * sizeof(t_vertices *));
			add_to_garbage(scene->vertices_tab);
		}
		t_vertices *tmp = scene->vertices_root;
		scene->vertices_root = 0x0;
		scene->vertices_current = 0x0;
		while (tmp && i < scene->vertices_count){
			scene->vertices_tab[i++] = tmp;
			tmp = tmp->next;
		}
		scene->vertices_tab[i++] = 0x0;
		i = 0;
	}
	if (scene->vertex_normals_root){
		if (scene->vertex_normals_tab){
			t_vertices	**tmp = malloc((scene->vertex_normals_count + 1) * sizeof(t_vertices *));
			add_to_garbage(tmp);
			while (scene->vertex_normals_tab[i]){
				tmp[i] = scene->vertex_normals_tab[i];
				i++;
			}
			scene->vertex_normals_tab = tmp;
		}
		else{
			scene->vertex_normals_tab = malloc((scene->vertex_normals_count + 1) * sizeof(t_vertices *));
			add_to_garbage(scene->vertex_normals_tab);
		}
		t_vertices *tmp = scene->vertex_normals_root;
		scene->vertex_normals_root = 0x0;
		scene->vertex_normals_current = 0x0;
		while (tmp && i < scene->vertex_normals_count){
			scene->vertex_normals_tab[i++] = tmp;
			tmp = tmp->next;
		}
		scene->vertex_normals_tab[i++] = 0x0;
		i = 0;
	}
	if (scene->texture_coordinates_root){
		if (scene->texture_coordinates_tab){
			t_texture_coordinates	**tmp = malloc((scene->texture_coordinates_count + 1) * sizeof(t_texture_coordinates *));
			add_to_garbage(tmp);
			while (scene->texture_coordinates_tab[i]){
				tmp[i] = scene->texture_coordinates_tab[i];
				i++;
			}
			scene->texture_coordinates_tab = tmp;
		}
		else{
			scene->texture_coordinates_tab = malloc((scene->texture_coordinates_count + 1) * sizeof(t_texture_coordinates *));
			add_to_garbage(scene->texture_coordinates_tab);
		}
		t_texture_coordinates *tmp = scene->texture_coordinates_root;
		scene->texture_coordinates_root = 0x0;
		scene->texture_coordinates_current = 0x0;
		while (tmp && i < scene->texture_coordinates_count){
			scene->texture_coordinates_tab[i++] = tmp;
			tmp = tmp->next;
		}
		scene->texture_coordinates_tab[i++] = 0x0;
	}
}

void	parse_face(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	if (tab_size < 3){
		dprintf(2,"Face on line %d is not a complete face\n", line_nb);
		free_garbage();
	}
	t_faces *face = malloc(sizeof(struct s_faces));
	add_to_garbage(face);
	face->vertices = malloc((tab_size) * sizeof(t_vertices *));
	add_to_garbage(face->vertices);
	face->vertex_normals = malloc((tab_size) * sizeof(t_vertices *));
	add_to_garbage(face->vertex_normals);
	face->texture_coordinates = malloc((tab_size) * sizeof(t_texture_coordinates *));
	add_to_garbage(face->texture_coordinates);
	face->material = scene->objs_list->material;
	for (size_t i=0; i< tab_size - 1 ; i++){
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
}

void parse_scene_line(char **tab, int tab_size, t_scene *scene, int line_nb){
	// TODO handle line continuation char '\'
	if (!scene->objs_list){
		scene->objs_list = malloc(sizeof(struct s_obj));
		add_to_garbage((scene->objs_list));
		scene->objs_list->next = 0x0;
		scene->vertices_root = 0x0;
		scene->vertex_normals_root = 0x0;
		scene->texture_coordinates_root = 0x0;
	}
	
	if (!ft_strcmp(tab[0], "v")){
		parse_vertices(tab, tab_size, scene, line_nb);
	}
	else if (!ft_strcmp(tab[0], "vn")){
		parse_vertex_normals(tab, tab_size, scene, line_nb);
	}
	else if (!ft_strcmp(tab[0], "vt")){
		parse_texture_coordinates(tab, tab_size, scene, line_nb);
	}
	else if (!ft_strcmp(tab[0], "o")){
		if (tab_size < 2){
			dprintf(2,"%s on line %d is missing a name value\n", tab[0], line_nb);
			free_garbage();
		}
		else {
			scene->objs_list->name = ft_strdup(tab[1]);
			add_to_garbage(scene->objs_list->name);
		}
	}
	else if (!ft_strcmp(tab[0], "usemtl")){
		if (tab_size < 2){
			dprintf(2,"%s on line %d is missing a name value\n", tab[0], line_nb);
			free_garbage();
		}
		char	*name = ft_strtrim(tab[1], " \t\r\n");
		add_to_garbage(name);
		t_material *tmp = scene->material_list;
		while (tmp && tmp->name && ft_strcmp(name, tmp->name))
			tmp = tmp->next;
		if (!tmp){
			dprintf(2,"Material name on line %d does not exist\n", line_nb);
			free_garbage();
		}
		scene->objs_list->material = tmp;
	}
	else if (!ft_strcmp(tab[0], "f")){	// f v/vt/vn
		pass_obj_list_to_tab(scene);
		parse_face(tab, tab_size, scene, line_nb);
	}
}

void parse_mtl_line(char **tab, int tab_size, t_material *material, int line_nb){
	t_litle_rgb *rgb = 0x0;						// TODO also parse map_Kd and map_Ks
	float		float_value = 1;
	float		min = 0, max = 1;
	char		*possible_float_key[4] = {"Ns", "Ni", "d", "illum"};
	float		possible_float_min[4] = {0.0, 0.001, 0.0, 0.0};
	float		possible_float_max[4] = {1000.0, 10.0, 1.0, 10.0};
	char		*possible_rgb_key[3] = {"Ka", "Kd", "Ks"};
	
	for (int i=0;i<3;i++){
		if (!ft_strcmp(possible_rgb_key[i], tab[0])){
			rgb = malloc(sizeof(struct s_litle_rgb));
			add_to_garbage(rgb);
			if (tab_size < 4){
				dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
				free_garbage();
			}
			rgb->r = atof(tab[1]);
			rgb->g = atof(tab[2]);
			rgb->b = atof(tab[3]);
			min = 0.0;
			max = 1.0;
			if (i == 0)
				material->ambient_color = rgb;
			if (i == 1)
				material->diffuse_color = rgb;
			if (i == 2)
				material->specular_color = rgb;
			break ;
		}
	}
	for (int i=0; i<4;i++){
		if (!ft_strcmp(possible_float_key[i], tab[0])){
			if (tab_size < 2){
				dprintf(2,"%s on line %d is missing a float value\n", tab[0], line_nb);
				free_garbage();
			}
			float_value = atof(tab[1]);
			min = possible_float_min[i];
			max = possible_float_max[i];
			if (i == 0)
				material->shininess = float_value;
			else if (i == 1)
				material->optical_density = float_value;
			else if (i == 2)
				material->dissolve = float_value;
			else
				material->illum = float_value;
			break;
		}
	}
	
	if ((rgb && ((rgb->r < min || rgb->r > max) || (rgb->g < min || rgb->g > max) || (rgb->b < min || rgb->b > max))) 
		|| (float_value < min || float_value > max)){
		dprintf(2, "%s on line %d value out of bound (should be between %.2f and %.2f)\n", tab[0], line_nb, min, max);
		free_garbage();
	}
}

void print_material(t_material *material){
	printf("\t%s material\n", material->name);
	if (material->ambient_color)
		printf("\t\tr: %f g: %f b: %f ka\n", material->ambient_color->r, material->ambient_color->g, material->ambient_color->b);
	if (material->diffuse_color)
		printf("\t\tr: %f g: %f b: %f kd\n", material->diffuse_color->r, material->diffuse_color->g, material->diffuse_color->b);
	if (material->specular_color)
		printf("\t\tr: %f g: %f b: %f ks\n", material->specular_color->r, material->specular_color->g, material->specular_color->b);
	printf("\t\t%f ns\n", material->shininess);
	printf("\t\t%f ni\n", material->optical_density);
	printf("\t\t%f d\n", material->dissolve);
	printf("\t\t%d illum\n", material->illum);
}

t_material	*parse_mtl(char *path){
	int fd = open_file(path, ".mtl");
	if (fd < 0)
		return (0x0);
	t_material *material = malloc(sizeof(struct s_material));
	add_to_garbage(material);
	t_material *root = material;
	material->next = 0x0;
	char	*s = get_next_line(fd);
	char	*s_trim = ft_strtrim(s, " \n\t\r");
	free(s);
	add_to_garbage(s_trim);
	char	**tab;
	int		tab_size;
	int		line = 1;
	while (s_trim){
		if (*s_trim != '#'){
			tab = ft_split(s_trim, ' ');
			add_to_garbage(tab);
			tab_size = 0;
			for (; tab[tab_size];tab_size++){
				add_to_garbage(tab[tab_size]);
			}
			if (tab[0]){
				if (!ft_strcmp(tab[0], "newmtl")){
					material->next = malloc(sizeof(struct s_material));
					add_to_garbage(material->next);
					material = material->next;
					material->next = 0x0;
					if (tab[1]){
						material->name = ft_strdup(tab[1]);
						add_to_garbage(material->name);
					}
					else{
						dprintf(2,"newmtl on line %d is missing a name\n", line);
					}
					material->ambient_color = 0x0;
					material->diffuse_color = 0x0;
					material->specular_color = 0x0;
					material->shininess = 1.0;
					material->optical_density =1.0;
					material->dissolve = 1.0;
					material->illum = 0;
				}
				else {
					parse_mtl_line(tab, tab_size, material, line);
				}
			}
		}
		s = get_next_line(fd);
		s_trim = ft_strtrim(s, " \t\r\n");
		free(s);
		if (s_trim)
			add_to_garbage(s_trim);
		line++;
	}

	material->next = 0x0;
	return (root->next);
}