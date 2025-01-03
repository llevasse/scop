#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;
extern GLfloat			*g_vertex_buffer_data;
extern GLuint			*g_element_buffer_data;

void	initVertexBuffer(const float *vertexPositions){
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	render_obj(t_scene *scene, t_obj *obj){
	(void)scene;
	(void)obj;
	size_t	i = 0, j = 0, count = 0, pnt_nb = 0, segment_idx = 0;
	while (obj){
		t_faces	*face = obj->faces;
		while (face){
			i = 0;
			while (face->vertices[i]){
				g_element_buffer_data[segment_idx++] = face->vertices[i]->id;
				face->vertices[i]->matrixed_x = face->vertices[i]->x;
				face->vertices[i]->matrixed_y = face->vertices[i]->y;
				face->vertices[i]->matrixed_z = face->vertices[i]->z;
				//multiplyPointWithMatrix(scene, face->vertices[i], scene->matrix_camera);
				multiplyPointWithMatrix(scene, face->vertices[i], scene->matrix);
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_x;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_y;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_z;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->r;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->g;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->b;
				
				
//					printf("point %zu xyz: %f %f %f\n", pnt_nb, face->vertices[i]->matrixed_x, face->vertices[i]->matrixed_y, face->vertices[i]->matrixed_z);
				pnt_nb++;
				i++;
				if (face->vertices[i])
					g_element_buffer_data[segment_idx++] = face->vertices[i]->id;
				else
					g_element_buffer_data[segment_idx++] = face->vertices[0]->id;
			}
			face = face->next;
			count++;
		}
		obj = obj->next;
	}
}