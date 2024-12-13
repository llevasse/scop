#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

t_obj *parse_obj(int fd){
 	t_obj *obj = malloc(sizeof(struct s_obj));
	add_to_garbage(obj);
	char	*s = get_next_line(fd);
	char	**line_content = 0x0;
	size_t	line_content_size = 0;
	while (s){
		line_content = ft_split(s, ' ');	//TODO add tab to garbage collector
		add_to_garbage(line_content);
		line_content_size = 0;
		for (; line_content[line_content_size]; line_content_size++){
			add_to_garbage(line_content[line_content_size]);
		}
		if (*s != '#'){
			if (!line_content){
				dprintf(2, "Couldn't allocate memory\n");
				exit(1);
			}
			if (!ft_strcmp(line_content[0], "mtllib")){
				if (line_content[1]){
					obj->material_list = parse_mtl(line_content[1]);
					if (!obj->material_list){
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

		}
		if (line_content){
			line_content = 0x0;
			line_content_size = 0;
		}
		free(s);
		s = get_next_line(fd);
	}
	return (obj);
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

t_material_list	*parse_mtl(char *path){
	int fd = open_file(path, ".mtl");
	if (fd < 0)
		return (0x0);
	t_material_list *list = malloc(sizeof(struct s_material_list));
	add_to_garbage(list);
	t_material_list *root = list;
	list->material = 0x0;
	list->next = 0x0;
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
					if (list->material){
						list->next = malloc(sizeof(struct s_material_list));
						add_to_garbage(list->next);
						list = list->next;
						list->next = 0x0;
						list->material = 0x0;
					}
					list->material = malloc(sizeof(struct s_material));
					add_to_garbage(list->material);
					if (tab[1]){
						list->material->name = ft_strdup(tab[1]);
						add_to_garbage(list->material->name);
					}
					else{
						dprintf(2,"newmtl on line %d is missing a name\n", line);
					}
					list->material->ambient_color = 0x0;
					list->material->diffuse_color = 0x0;
					list->material->specular_color = 0x0;
					list->material->shininess = 1.0;
					list->material->optical_density =1.0;
					list->material->dissolve = 1.0;
					list->material->illum = 0;
					list->material->vertices = 0x0;
					list->material->vertex_normals = 0x0;
					list->material->vertex_tertures = 0x0;
				}
				else {
					parse_mtl_line(tab, tab_size, list->material, line);
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

	list->next = 0x0;
	return (root);
}