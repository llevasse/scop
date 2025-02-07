#include "../include/scop.h"

t_scene	*init_scene(){
 	t_scene *scene = malloc(sizeof(struct s_scene));
	add_to_garbage(scene);

	scene->objs_list = 0x0;
	scene->material_list = 0x0;

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
	scene->normal_mode = 0;
	scene->material_normal_mode = 0;
	scene->material_mode = 0;
	scene->material_relief_mode = 0;
	scene->colour_oppacity = 1;

	scene->x_auto_rotate = 0;
	scene->y_auto_rotate = 1;
	scene->z_auto_rotate = 0;

	scene->focus = 0x0;

	scene->programID = -1;
	scene->VAO = -1;
	scene->texture_change = 0;

	scene->default_material = create_default_material();

	return (scene);
}

t_scene *parse_scene(int fd, char *obj_path){
	t_scene *scene = init_scene();
 	t_obj *root_obj_list = 0x0;
	char	*s = get_next_line(fd), *trim;
	char	**line_content = 0x0;
	size_t	line_content_size = 0;
	size_t	line_count = 1;
	while (s){
		trim = ft_strtrim(s, " \n\t\r");
		if (!trim){
			free(s);
			free_garbage();
		}
		if (*trim != '#'){
			if (SCOP_DEBUG)
				printf("Parse line %zu: %s\n", line_count, trim);
			line_content = ft_split(trim, ' ');
			if (!line_content){
				free(trim);
				free(s);
				free_garbage();
			}
			line_content_size = 0;
			for (; line_content[line_content_size]; line_content_size++){}
			if (line_content[0]){
				if (!ft_strcmp(line_content[0], "mtllib")){
					if (line_content[1]){
						scene->material_list = parse_mtl(line_content[1], obj_path);
					}
					else{
						dprintf(2,"\033[0;33mWarning\033[0m : Missing path to .mtl file\nDefault material will be used\n");
					}
				}
				else {
					if (!parse_scene_line(line_content, line_content_size, scene, line_count)){
						free(s);
						free(trim);
						for (line_content_size = 0; line_content[line_content_size]; line_content_size++){
							free(line_content[line_content_size]);
						}
						free(line_content);
						free_garbage();
					}
					if (!root_obj_list && scene->objs_list)
						root_obj_list = scene->objs_list;
				}
			}
			for (line_content_size = 0; line_content[line_content_size]; line_content_size++){
				free(line_content[line_content_size]);
			}
			free(line_content);
			line_content = 0x0;
			line_content_size = 0;
		}
		free(s);
		free(trim);
		s = get_next_line(fd);
		line_count++;
	}
	
	
	scene->objs_list = root_obj_list;
	link_faces_to_materials(scene);	
	
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

void	*pass_obj_list_to_tab(t_scene *scene){
	size_t i=0;
	if (scene->vertices_root){
		if (scene->vertices_tab){
			t_vertices	**tmp = malloc((scene->vertices_count + 1) * sizeof(t_vertices *));
			if (!tmp)
				return (0x0);
			add_to_garbage(tmp);
			while (scene->vertices_tab[i]){
				tmp[i] = scene->vertices_tab[i];
				i++;
			}
			scene->vertices_tab = tmp;
		}
		else{
			scene->vertices_tab = malloc((scene->vertices_count + 1) * sizeof(t_vertices *));
			if (!scene->vertices_tab)
				return (0x0);
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
			if (!tmp)
				return (0x0);
			add_to_garbage(tmp);
			while (scene->vertex_normals_tab[i]){
				tmp[i] = scene->vertex_normals_tab[i];
				i++;
			}
			scene->vertex_normals_tab = tmp;
		}
		else{
			scene->vertex_normals_tab = malloc((scene->vertex_normals_count + 1) * sizeof(t_vertices *));
			if (!scene->vertex_normals_tab)
				return (0x0);
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
			if (!tmp)
				return (0x0);
			add_to_garbage(tmp);
			while (scene->texture_coordinates_tab[i]){
				tmp[i] = scene->texture_coordinates_tab[i];
				i++;
			}
			scene->texture_coordinates_tab = tmp;
		}
		else{
			scene->texture_coordinates_tab = malloc((scene->texture_coordinates_count + 1) * sizeof(t_texture_coordinates *));
			if (!scene->texture_coordinates_tab)
				return (0x0);
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
	return (scene);
}

void *parse_scene_line(char **tab, int tab_size, t_scene *scene, size_t line_nb){
	if (!scene->objs_list){
		scene->objs_list = malloc(sizeof(struct s_obj));
		if (!scene->objs_list)
			return (0x0);
		add_to_garbage((scene->objs_list));
		scene->objs_list->name	= 0x0;
		scene->objs_list->next = 0x0;
		scene->objs_list->faces = 0x0;
		scene->objs_list->material = 0x0;
		scene->vertices_root = 0x0;
		scene->vertex_normals_root = 0x0;
		scene->texture_coordinates_root = 0x0;
	}

	if (!ft_strcmp(tab[0], "v")){
		if (!parse_vertices(tab, tab_size, scene, line_nb)){
			return (0x0);
		}
	}
	else if (!ft_strcmp(tab[0], "vn")){
		if (!parse_vertex_normals(tab, tab_size, scene, line_nb)){
			return (0x0);
		}
	}
	else if (!ft_strcmp(tab[0], "vt")){
		if (!parse_texture_coordinates(tab, tab_size, scene, line_nb)){
			return (0x0);
		}
	}
	else if (!ft_strcmp(tab[0], "o") || !ft_strcmp(tab[0], "g")){
		if (tab_size < 2){
			dprintf(2,"\033[0;33mWarning\033[0m : %s on line %zu is missing a name value\n", tab[0], line_nb);
		}
		if (scene->objs_list->name){ // if not first object
			scene->objs_list->next = malloc(sizeof(struct s_obj));
			if (!scene->objs_list->next)
				return (0x0);
			add_to_garbage(scene->objs_list->next);
			scene->objs_list = scene->objs_list->next;
			scene->objs_list->name	= 0x0;
			scene->objs_list->next = 0x0;
			scene->objs_list->faces = 0x0;
			scene->objs_list->material = 0x0;
		}
		scene->objs_list->name = ft_strdup(tab[1]);
		if (!scene->objs_list->name)
			return (0x0);
		add_to_garbage(scene->objs_list->name);
	}
	else if (!ft_strcmp(tab[0], "usemtl")){
		if (tab_size < 2){
			dprintf(2,"\033[0;33mWarning\033[0m : %s on line %zu is missing a name value. Default material will be used\n", tab[0], line_nb);
			return (tab[0]);
		}
		char	*name = ft_strtrim(tab[1], " \t\r\n");
		if (!name)
			return (0x0);
		add_to_garbage(name);
		t_material *tmp = scene->material_list;
		while (tmp && tmp->name && ft_strcmp(name, tmp->name))
			tmp = tmp->next;
		if (!tmp){
			dprintf(2,"\033[0;33mWarning\033[0m : Material name on line %zu does not exist\nDefault material will be used\n", line_nb);
			tmp = scene->default_material;
		}
		scene->objs_list->material = tmp;
	}
	else if (!ft_strcmp(tab[0], "f")){
		if (!scene->objs_list->material){
			scene->objs_list->material = scene->default_material;
		}
		if (!pass_obj_list_to_tab(scene))
			return (0x0);
		if (!parse_face(tab, tab_size, scene, line_nb))
			return (0x0);
	}
	return (tab[0]);
}