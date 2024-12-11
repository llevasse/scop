#ifndef SCOP_H
# define SCOP_H
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"

int open_obj(char *path);

typedef struct s_obj{
	t_material **material_list;
}	t_obj;

typedef struct s_material{
	int						material_fd;
	char					*material_path;
	t_vertices				**vertices;
	t_verctex_normals		**vertex_normals
	t_texture_coordinates	**vertex_tertures
}	t_material;
#endif
