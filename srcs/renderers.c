#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;
extern GLfloat			*g_vertex_buffer_data;

void	initVertexBuffer(const float *vertexPositions){
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	render_obj(t_scene *scene, t_obj *obj){
	(void)scene;
	(void)obj;
	size_t	i = 0, j = 0;
	while (scene->vertices_tab[i] && j < scene->vertices_count * 3){
		multiplyPointWithMatrix(scene, scene->vertices_tab[i]);
		g_vertex_buffer_data[j++] = scene->vertices_tab[i]->matrixed_x;
		g_vertex_buffer_data[j++] = scene->vertices_tab[i]->matrixed_y;
		g_vertex_buffer_data[j++] = scene->vertices_tab[i]->matrixed_z;
		i++;
	}
}