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
		if (*s != '#'){
			if (!line_content){
				free(obj);
				dprintf(2, "Couldn't allocate memory\n");
				exit(1);
			}
			for (; line_content[line_content_size]; line_content_size++);
			if (!ft_strcmp(line_content[0], "mtllib")){

			}

		}
		if (line_content){
			for (int i=0; line_content[i]; i++){
				free(line_content[i]);
				line_content[i] = 0x0;
			}
			free(line_content);
			line_content = 0x0;
		}
		free(s);
		s = get_next_line(fd);
	}
	return (obj);
}
