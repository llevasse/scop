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
		line_content_size = 0;
		for (; line_content[line_content_size]; line_content_size++);
		if (*s != '#'){
			if (!line_content){
				free(obj);
				dprintf(2, "Couldn't allocate memory\n");
				exit(1);
			}
			if (!ft_strcmp(line_content[0], "mtllib")){
				if (line_content[1]){
					obj->material_list = parse_mtl(line_content[1]);
					if (!obj->material_list){
						free_tab(line_content, line_content_size);
						free(s);
						free_garbage();
					}

				}
				else{
					dprintf(2,"Missing path to .mtl file");
					free(s);
					free_tab(line_content, line_content_size);
					free_garbage();
				}
			}

		}
		if (line_content){
			line_content = free_tab(line_content, line_content_size);
			line_content_size = 0;
		}
		free(s);
		s = get_next_line(fd);
	}
	return (obj);
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
	char	*s = ft_strtrim(get_next_line(fd), " \n\t\r");
	add_to_garbage(s);
	char	**tab;
	int		tab_size;
	int		line = 1;
	while (s){
		if (*s != '#'){
			tab = ft_split(s);
			add_to_garbage(tab);
			tab_size = 0;
			for (; tab[tab_size];tab_size++){
				add_to_garbage(tab[tab_size]);
			}
			if (!ft_strcmp(tab[0], "newmtl")){
				list->material = malloc(sizeof(struct s_material));
				add_to_garbage(list->material);
				if (tab_size >= 2)
					list->material->name = ft_strdup(tab[1]);
				else{
					dprintf(2,"newmtl on line %d is missing a name\n", line);
				}
			}
			else if (!ft_strcmp(tab[0], "Ka")){
				if (tab_size >= 4){
					t_little_rgb *rgb = malloc(sizeof(struct s_litle_rgb));
					add_to_garbage(rgb);
					rgb->r = atof(tab[1]);
					rgb->g = atof(tab[2]);
					rgb->b = atof(tab[3]);
					if ((rgb->r < 0.0 || rgb.r > 1.0) || (rgb->g < 0.0 || rgb.g > 1.0) || (rgb->b < 0.0 || rgb.b > 1.0)){
						dprintf(2, "%s on line %d value out of bound (should be between 0.0 and 1.0)\n", tab[0], line);
						free_garbage();
					}
				}
				else{
					dprintf(2,"Ka on line %d is missing a float value\n", line);
					free_garbage();
				}
			}
			else if (!ft_strcmp(tab[0], "Kd")){
				if (tab_size >= 4){
					t_little_rgb *rgb = malloc(sizeof(struct s_litle_rgb));
					add_to_garbage(rgb);
					rgb->r = atof(tab[1]);
					rgb->g = atof(tab[2]);
					rgb->b = atof(tab[3]);
					if ((rgb->r < 0.0 || rgb.r > 1.0) || (rgb->g < 0.0 || rgb.g > 1.0) || (rgb->b < 0.0 || rgb.b > 1.0)){
						dprintf(2, "%s on line %d value out of bound (should be between 0.0 and 1.0)\n", tab[0], line);
						free_garbage();
					}
				}
				else{
					dprintf(2,"Kd on line %d is missing a float value\n", line);
					free_garbage();
				}
			}
			else if (!ft_strcmp(tab[0], "Ks")){
				if (tab_size >= 4){
					t_little_rgb *rgb = malloc(sizeof(struct s_litle_rgb));
					add_to_garbage(rgb);
					rgb->r = atof(tab[1]);
					rgb->g = atof(tab[2]);
					rgb->b = atof(tab[3]);
					if ((rgb->r < 0.0 || rgb.r > 1.0) || (rgb->g < 0.0 || rgb.g > 1.0) || (rgb->b < 0.0 || rgb.b > 1.0)){
						dprintf(2, "%s on line %d value out of bound (should be between 0.0 and 1.0)\n", tab[0], line);
						free_garbage();
					}
				}
				else{
					dprintf(2,"Ka on line %d is missing a float value\n", line);
					free_garbage();
				}
			}
			else if (!ft_strcmp(tab[0], "Ns")){
				if (tab_size >= 2){
					list->material->shininess = atof(tab[1]);
					if (list->material->shininess < 0.0 || list->material->shininess > 1000.0){
						dprintf(2, "Ns on line %d value out of bound (should be between 0.0 and 1000.0)\n", line);
						free_garbage();
					}
				}
				else{
					dprintf(2,"Ka on line %d is missing a float value\n", line);
				}
			}
		}
		s = ft_strtrim(get_next_line(fd));
		add_to_garbage(s);
		line++;
	}
	list->next = 0x0;
	return (list);
}