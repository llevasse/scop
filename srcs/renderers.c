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
	size_t	i = 0, j = 0, count = 0, pnt_nb = 0;
	while (obj){
		if (!ft_strncmp(obj->name, "Island", 6) || 1){
			t_faces	*face = obj->faces;
			while (face){
				i = 0;
				while (face->vertices[i]){
					face->vertices[i]->matrixed_x = face->vertices[i]->x;
					face->vertices[i]->matrixed_y = face->vertices[i]->y;
					face->vertices[i]->matrixed_z = face->vertices[i]->z;
					//multiplyPointWithMatrix(scene, face->vertices[i], scene->matrix_camera);
					multiplyPointWithMatrix(scene, face->vertices[i], scene->matrix);
					g_colour_buffer_data[j] = face->material->diffuse_color->r;
					g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_x;
					
					g_colour_buffer_data[j] = face->material->diffuse_color->g;
					g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_y;
					
					g_colour_buffer_data[j] = face->material->diffuse_color->b;
					g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_z;
					printf("point %zu xyz: %f %f %f\n", pnt_nb, face->vertices[i]->matrixed_x, face->vertices[i]->matrixed_y, face->vertices[i]->matrixed_z);
					pnt_nb++;
					i++;
				}
				face = face->next;
				count++;
			}
			break; 
		}
		obj = obj->next;
	}
}