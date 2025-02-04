#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;


t_material	*create_default_material(){
	t_material *tmp = malloc(sizeof(struct s_material));
	add_to_garbage(tmp);
	t_litle_rgb *ac = malloc(sizeof(struct s_litle_rgb));
	add_to_garbage(ac);
	ac->r = .5;
	ac->g = .5;
	ac->b = .5;
	tmp->name = "";
	tmp->ambient_colour = ac;
	tmp->diffuse_colour = ac;
	tmp->specular_colour = ac;
	tmp->shininess = 1.0;
	tmp->optical_density =1.0;
	tmp->dissolve = 1.0;
	tmp->illum = 0;
	tmp->nb_faces = 0;
	tmp->faces = 0x0;
	tmp->face_index = 0;
	return (tmp);
}

void use_default_mtl(t_scene *scene){
	t_material *tmp = scene->material_list;
	while (tmp && tmp->name){
		if (!ft_strcmp(tmp->name, "")){
			scene->objs_list->material = tmp;
			return ;
		}
	}
	tmp = scene->default_material;
	scene->material_list = tmp;
	scene->objs_list->material = tmp;
	scene->default_material = tmp;
}

void parse_mtl_line(char **tab, int tab_size, t_material *material, int line_nb){
	t_litle_rgb *rgb = 0x0;
	float		float_value = 1;
	float		min = 0, max = 1;
	char		*possible_float_key[4] = {"Ns", "Ni", "d", "illum"};
	float		possible_float_min[4] = {0.0, 0.001, 0.0, 0.0};
	float		possible_float_max[4] = {1000.0, 10.0, 1.0, 10.0};
	char		*possible_rgb_key[3] = {"Ka", "Kd", "Ks"};
	
	for (int i=0;i<3;i++){
		if (!ft_strcmp(possible_rgb_key[i], tab[0])){
			if (tab_size < 4){
				dprintf(2,"\033[0;33mWarning\033[0m : %s on line %d is missing a float value. This line will be ignored\n", tab[0], line_nb);
				return;
			}
			rgb = malloc(sizeof(struct s_litle_rgb));
			add_to_garbage(rgb);
			rgb->r = atof(tab[1]);
			rgb->g = atof(tab[2]);
			rgb->b = atof(tab[3]);
			min = 0.0;
			max = 1.0;		
			if (rgb && ((rgb->r < min || rgb->r > max) || (rgb->g < min || rgb->g > max) || (rgb->b < min || rgb->b > max))){
				dprintf(2,"\033[0;33mWarning\033[0m : %s on line %d is missing a float value. This line will be ignored\n", tab[0], line_nb);
				return ;
			}

			if (i == 0){
				if (!material->ambient_colour)
					material->ambient_colour = rgb;
				else{
					dprintf(2, "\033[0;33mWarning\033[0m : Material '%s' ambient colour is already set\n", material->name);
					return ;
				}
			}
			if (i == 1){
				if (!material->diffuse_colour)
					material->diffuse_colour = rgb;
				else{
					dprintf(2, "\033[0;33mWarning\033[0m : Material '%s' diffuse colour is already set\n", material->name);
					return ;
				}
			}
			if (i == 2){
				if (!material->specular_colour)
					material->specular_colour = rgb;
				else{
					dprintf(2, "\033[0;33mWarning\033[0m : Material '%s' specular colour is already set\n", material->name);
					return ;
				}
			}
			break ;
		}
	}
	for (int i=0; i<4;i++){
		if (!ft_strcmp(possible_float_key[i], tab[0])){
			if (tab_size < 2){
				dprintf(2,"\033[0;33mWarning\033[0m : %s on line %d is missing a float value. This line will be ignored\n", tab[0], line_nb);
				return ;
			}
			float_value = atof(tab[1]);
			min = possible_float_min[i];
			max = possible_float_max[i];
			if (float_value < min || float_value > max){
				dprintf(2, "\033[0;33mWarning\033[0m : %s on line %d value out of bound (should be between %.2f and %.2f). This line will be ignored\n", tab[0], line_nb, min, max);
				return ;
			}
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
}

void print_material(t_material *material){
	printf("\t%s material\n", material->name);
	if (material->ambient_colour)
		printf("\t\tr: %f g: %f b: %f ka\n", material->ambient_colour->r, material->ambient_colour->g, material->ambient_colour->b);
	if (material->diffuse_colour)
		printf("\t\tr: %f g: %f b: %f kd\n", material->diffuse_colour->r, material->diffuse_colour->g, material->diffuse_colour->b);
	if (material->specular_colour)
		printf("\t\tr: %f g: %f b: %f ks\n", material->specular_colour->r, material->specular_colour->g, material->specular_colour->b);
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
		dprintf(2, "\033[0;33mWarning\033[0m : Can't open %s\nDefault material will be used\n", path);
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
					if (!tab[1]){
						dprintf(2,"\033[0;33mWarning\033[0m : newmtl on line %d is missing a name, it will be ignored\n", line);
					}
					else{
						material->next = malloc(sizeof(struct s_material));
						add_to_garbage(material->next);
						material = material->next;
						material->next = 0x0;
						material->name = ft_strdup(tab[1]);
						add_to_garbage(material->name);
						material->ambient_colour = 0x0;
						material->diffuse_colour = 0x0;
						material->specular_colour = 0x0;
						material->shininess = 1.0;
						material->optical_density =1.0;
						material->dissolve = 1.0;
						material->illum = 0;
						material->nb_faces = 0;
						material->faces = 0x0;
						material->face_index = 0;
					}
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