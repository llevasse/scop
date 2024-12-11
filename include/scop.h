#ifndef SCOP_H
# define SCOP_H
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"

int open_obj(char *path);


typedef struct s_garbage{
	void 					*addr;
	struct s_garbage		*next;
}	t_garbage;

typedef struct s_vertices{
} t_vertices;

typedef struct s_vertex_normals{
} t_vertex_normals;

typedef struct s_texture_coordinates{
} t_texture_coordinates;

typedef struct s_material{
	char					*name;
	float 					ambient_color;	//said 'ka'
	float					diffuse_color;	//said 'kd'
	float					specular_color;	//said 'ks'
	float					shininess;		//said 'ns'
	float					dissolve;		//said 'd'
	
	t_vertices				**vertices;
	t_vertex_normals		**vertex_normals;
	t_texture_coordinates	**vertex_tertures;
}	t_material;

typedef struct s_material_list{
	t_material 				*material;
	struct s_material_list	*next;
}	t_material_list;

typedef struct s_obj{
	int						material_fd;
	char					*material_path;
	t_material_list			*material_list;
}	t_obj;

t_obj *parse_obj(int fd);
t_material_list *parse_material_file(int path);

void add_to_garbage(void *addr);
void free_garbage();

#endif
