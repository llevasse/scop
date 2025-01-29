#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

t_scene	*init_scene(){
 	t_scene *scene = malloc(sizeof(struct s_scene));
	add_to_garbage(scene);

	scene->fov = 45;
	scene->far_plane_distance = 1000;
	scene->near_plane_distance = 1;
	scene->wireframe_view = 0;
	scene->zoom = 1;
	scene->x_angle = 0;
	scene->y_angle = 0;
	scene->z_angle = 0;
	scene->x_offset = 0;
	scene->y_offset = 0;
	scene->z_offset = 0;

	scene->max_x = 0;
	scene->min_x = 0;
	scene->max_y = 0;
	scene->min_y = 0;
	scene->max_z = 0;
	scene->min_z = 0;

	scene->height = 600;
	scene->width = 800;

	scene->objs_list = 0x0;
	scene->vertices_root = 0x0;
	scene->vertices_current = 0x0;
	scene->vertices_tab = 0x0;
	scene->vertices_count = 0;
	scene->display_vertices_count = 0;
	scene->vertex_normals_root = 0x0;
	scene->vertex_normals_current = 0x0;
	scene->vertex_normals_tab = 0x0;
	scene->vertex_normals_count = 0;
	scene->texture_coordinates_root = 0x0;
	scene->texture_coordinates_current = 0x0;
	scene->texture_coordinates_tab = 0x0;
	scene->texture_coordinates_count = 0;

	scene->x_scale = 1;
	scene->y_scale = 1;
	scene->z_scale = 1;

	scene->movement_mode = 0;
	scene->colour_mode = 0;
	scene->texture_mode = 0;
	scene->colour_oppacity = 1;

	scene->auto_rotate = 1;

	scene->focus = 0x0;
	return (scene);
}

t_scene *parse_scene(int fd, char *obj_path){
	t_scene *scene = init_scene();
 	t_obj *root_obj_list = 0x0;
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
					scene->material_list = parse_mtl(line_content[1], obj_path);
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
	
	scene->objs_list = root_obj_list;
	link_faces_to_materials(scene);
	/*printf("unique vertices : %zu, displaied vertices : %zu\n", scene->vertices_count, scene->display_vertices_count);

	for (size_t i=0; i<scene->vertices_count;i++){
		printf("\tvertices[%zu] : id : %zu; xyz : %f %f %f\n", i, scene->vertices_tab[i]->id, scene->vertices_tab[i]->x, scene->vertices_tab[i]->y, scene->vertices_tab[i]->z);
	}*/
	
	
	scene->translation_step = (scene->max_x - scene->min_x) / 100;
	float tmp = (scene->max_y - scene->min_y) / 100;
	scene->translation_step = scene->translation_step > tmp ? scene->translation_step : tmp;
	tmp = (scene->max_z - scene->min_z) / 100;
	scene->translation_step = scene->translation_step > tmp ? scene->translation_step : tmp;

	scene->z_offset = -(scene->translation_step * 200);

	scene->origin.x = (scene->max_x + scene->min_x) / 2;
	scene->origin.y = (scene->max_y + scene->min_y) / 2;
	scene->origin.z = (scene->max_z + scene->min_z) / 2;
	return (scene);
}

void	link_faces_to_materials(t_scene *scene){
	t_obj	*obj = scene->objs_list;
	t_faces	*face = 0x0;
	
	while (obj){
		face = obj->faces;
		while (face){
			if (!face->material->faces){
				face->material->faces = malloc(sizeof(struct s_faces) * (face->material->nb_faces + 1));
				add_to_garbage(face->material->faces);
				face->material->faces[face->material->nb_faces] = 0x0;
			}
			face->material->faces[face->material->face_index++] = face;
			face = face->next;
		}
		obj = obj->next;
	}
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

void parse_scene_line(char **tab, int tab_size, t_scene *scene, int line_nb){
	// TODO handle line continuation char '\'
	if (!scene->objs_list){
		scene->objs_list = malloc(sizeof(struct s_obj));
		add_to_garbage((scene->objs_list));
		scene->objs_list->name	= 0x0;
		scene->objs_list->next = 0x0;
		scene->objs_list->faces = 0x0;
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
			if (scene->objs_list->name){
				scene->objs_list->next = malloc(sizeof(struct s_obj));
				add_to_garbage(scene->objs_list->next);
				scene->objs_list = scene->objs_list->next;
				scene->objs_list->next = 0x0;
				scene->objs_list->faces = 0x0;
			}
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
		if (!scene->objs_list->material){
			use_default_mtl(scene);
		}
		pass_obj_list_to_tab(scene);
		parse_face(tab, tab_size, scene, line_nb);
	}
}

void use_default_mtl(t_scene *scene){
	t_material *tmp = scene->material_list;
	while (tmp && tmp->name){
		if (!ft_strcmp(tmp->name, "")){
			scene->objs_list->material = tmp;
			return ;
		}
	}
	tmp = malloc(sizeof(struct s_material));
	add_to_garbage(tmp);
	t_litle_rgb *ac = malloc(sizeof(struct s_litle_rgb));
	add_to_garbage(ac);
	ac->r = .5;
	ac->g = .5;
	ac->b = .5;
	tmp->name = "";
	tmp->ambient_color = ac;
	tmp->diffuse_color = ac;
	tmp->specular_color = ac;
	tmp->shininess = 1.0;
	tmp->optical_density =1.0;
	tmp->dissolve = 1.0;
	tmp->illum = 0;
	tmp->nb_faces = 0;
	tmp->faces = 0x0;
	tmp->face_index = 0;
	tmp->next = scene->material_list;
	scene->material_list = tmp;
	scene->objs_list->material = tmp;
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

size_t ft_strrchr_pos(char *s, char c){
	return (ft_strrchr(s, c) - s);
}

t_material	*parse_mtl(char *path, char *obj_path){
	if (path[0] != '/'){
		char *s_path = ft_substr(obj_path, 0, ft_strrchr_pos(obj_path, '/') + 1);
		add_to_garbage(s_path);
		path = ft_strjoin(s_path, path);
		add_to_garbage(path);
	}
	int fd = open_file(path, ".mtl");

	if (fd < 0){
		dprintf(2, "Can't open %s\n", path);
		return (0x0);
	}
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
					material->nb_faces = 0;
					material->faces = 0x0;
					material->face_index = 0;
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