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
	int						material_fd;
	char					*material_path;
	t_material **material_list;
}	t_obj;

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

typedef struct s_vertices{
} t_vertices;

typedef struct s_vertex_normals{
} t_vertex_normals;

typedef struct s_texture_coordinates{
} t_texture_coordinates;
#endif
