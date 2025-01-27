#ifndef SCOP_H
# define SCOP_H
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
//# include <GL/glew.h>
# include <math.h>
# include "../glad/include/glad/glad.h"
# include <GLFW/glfw3.h>

# include "../libft/libft.h"
# include "../freeglut-3.6.0/include/GL/freeglut.h"
# define PI 3.1415926535
# define RADIAN 0.017453292519444445

	int open_file(char *path, char *extension);


	typedef struct s_garbage{
		void 					*addr;
		struct s_garbage		*next;
	}	t_garbage;

	typedef struct s_vertices{
		float					x;
		float					y;
		float					z;
		float					w;			//optional, weight default is 1.0;

		float					matrixed_x;
		float					matrixed_y;
		float					matrixed_z;

		double					magnetude;
		size_t					id;
		struct s_vertices		*next;
	}	t_vertices;

	typedef struct s_quaternions{
		float					w;
		float					x;
		float					y;
		float					z;
	}	t_quaternions;

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
		t_vertices				*vertices[4];
		t_vertices				*vertex_normals[4];
		t_texture_coordinates	*texture_coordinates[4];
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

		t_faces					**faces;
		size_t					nb_faces;
		size_t					face_index;
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
		size_t					display_vertices_count;		//if 1 vertices is used by two faces, it will count as 2 vertices
		size_t					vertex_normals_count;
		size_t					texture_coordinates_count;

		float					fov;
		float					near_plane_distance;
		float					far_plane_distance;
		float					matrix[4][4];
		float					persepective_matrix[4][4];
		float					matrix_camera[4][4];
		float					model_matrix[4][4];
		
		double					x_angle, y_angle, z_angle;
		double					x_offset, y_offset, z_offset, translation_step;
		double					x_scale, y_scale, z_scale;
		float					scale_matrix[4][4], translation_matrix[4][4];
		float					matrix_x_rotation[4][4], matrix_y_rotation[4][4], matrix_z_rotation[4][4];

		float					max_x, min_x;
		float					max_y, min_y;
		float					max_z, min_z;

		t_vertices				origin;

		int						height, width;

		short					texture_mode, colour_mode;
		float					colour_oppacity;

		short					movement_mode;
		short					wireframe_view;
		float					zoom;

	}	t_scene;

	void	use_texture();
	t_scene *parse_scene(int fd, char *obj_path);
	t_vertices *create_vertices(float x, float y, float z, t_scene *scene);
	t_texture_coordinates *create_texture_coordinates(char **tab, int tab_size);
	void	link_faces_to_materials(t_scene *scene);
	void	parse_vertices(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_vertex_normals(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_texture_coordinates(char **tab, int tab_size, t_scene *scene, int line_nb);
	void use_default_mtl(t_scene *scene);
	void	parse_face(char **tab, size_t tab_size, t_scene *scene, int line_nb);
	void	parse_scene_line(char **tab, int tab_size, t_scene *scene, int line_nb);
	void	parse_mtl_line(char **tab, int tab_size, t_material *material, int line_nb);
	t_material	*parse_mtl(char *path, char *obj_path);
	void	pass_obj_list_to_tab(t_scene *scene);

	void	add_to_garbage(void *addr);
	void	free_garbage();

	void	print_material(t_material *material);

	void	setMatrix(t_scene *scene);
	void	multiplyPointWithMatrix(t_vertices *p, float matrix[4][4]);
	void	multiplyPointWithRotationsMatrixes(t_scene *scene, t_vertices *p);
	
	void	render_obj(t_scene *scene, t_obj *obj);
	
	GLuint	loadShaders(const char *vertex_path, const char *fragment_path);

	void	print_error(const char *fmt, va_list ap);
	void	input_handler(GLFWwindow *window);
	void	render(GLFWwindow *window);
	void	resizeViewport(GLFWwindow *window, int width, int height);
	void	scroll_handler(GLFWwindow *window, double xOffset, double yOffset);
	void	openglObjInit();
	
#endif
