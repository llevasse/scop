#ifndef SCOP_H
# define SCOP_H
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"
# include "../freeglut-3.6.0/include/GL/freeglut.h"
# include <GL/glut.h>

	int open_file(char *path, char *extension);


	typedef struct s_garbage{
		void 					*addr;
		struct s_garbage		*next;
	}	t_garbage;

	typedef struct s_vertices{
		float					x;
		float					y;
		float					z;
		struct s_vertices		*next;
	}	t_vertices;

	typedef struct s_texture_coordinates{	// values between 0 and 1
		float							u;
		float							v;	//optional, default is 1
		float							w;	//optional, default is 1
		struct s_texture_coordinates	*next;
	}	t_texture_coordinates;

	typedef struct s_litle_rgb{
		float					r;
		float					g;
		float					b;
	}	t_litle_rgb;

	typedef struct s_faces{
		t_vertices				**vertices;
		t_vertices				**vertex_normals;
		t_texture_coordinates	**texture_coordinates;
		struct s_faces			*next;
		struct s_material		*material;
	}	t_faces;

	typedef struct s_material{
		char					*name;
		t_litle_rgb				*ambient_color;      //said 'ka'
		t_litle_rgb				*diffuse_color;      //said 'kd'
		t_litle_rgb				*specular_color;     //said 'ks'
		float					shininess;		     //said 'ns'
		float					optical_density;	 //said 'Ni',1.0 is default
		float					dissolve;		     //said 'd'
		int						illum;				 //from 0 to 10

		t_faces					*faces;
		struct s_material		*next;
	}	t_material;

	typedef struct s_obj{
		char					*name;
		t_material				*material;
		t_faces					*faces;
		struct s_obj			*next;
	}	t_obj;

	typedef struct s_scene{
		int						material_fd;
		char					*material_path;
		t_material				*material_list;
		t_obj					*objs_list;

		t_vertices				*vertices_current;
		t_vertices				*vertices_root;
		t_vertices				**vertices_tab;

		t_vertices				*vertex_normals_current;
		t_vertices				*vertex_normals_root;
		t_vertices				**vertex_normals_tab;

		t_texture_coordinates	*texture_coordinates_current;
		t_texture_coordinates	*texture_coordinates_root;
		t_texture_coordinates	**texture_coordinates_tab;

		size_t					vertices_count;
		size_t					vertex_normals_count;
		size_t					texture_coordinates_count;
	}	t_scene;

	typedef struct s_scene_parser{
		t_vertices				*current_vertices;
	}	t_scene_parser;

	t_scene *parse_scene(int fd);
	t_vertices *create_vertices(float x, float y, float z);
	t_texture_coordinates *create_texture_coordinates(char **tab, int tab_size);
	void	parse_vertices(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_vertex_normals(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_texture_coordinates(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_scene_line(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_mtl_line(char **tab, int tab_size, t_material *material, int line_nb);
	t_material	*parse_mtl(char *path);
	void	pass_obj_list_to_tab(t_scene *scene);

	void add_to_garbage(void *addr);
	void free_garbage();

	void print_material(t_material *material);

#endif
