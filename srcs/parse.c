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
		line_content = ft_split(s, ' ');
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
	return (list);
}