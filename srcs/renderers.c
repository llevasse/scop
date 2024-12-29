#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;
extern GLfloat			*g_vertex_buffer_data;
extern GLfloat			*g_colour_buffer_data;

void	initVertexBuffer(const float *vertexPositions){
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	render_obj(t_scene *scene, t_obj *obj){
	(void)scene;
	(void)obj;
	size_t	i = 0, j = 0, count = 0;
	while (obj){
		t_faces	*face = obj->faces;
		while (face){
			i = 0;
			while (face->vertices[i]){
				multiplyPointWithMatrix(scene, face->vertices[i]);
				g_colour_buffer_data[j] = face->material->diffuse_color->r;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_x;
				
				g_colour_buffer_data[j] = face->material->diffuse_color->g;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_y;
				
				g_colour_buffer_data[j] = face->material->diffuse_color->b;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_z;
				
				i++;
			}
			face = face->next;
			count++;
		}
		obj = obj->next;
	}
}