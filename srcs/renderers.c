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
	size_t	i = 0, j = 0, count = 0, pnt_nb = 0;
	double colour_offset;
	while (obj){
		t_faces	*face = obj->faces;
		while (face){
			i = 0;
			colour_offset = (count % 3 ? .04 : count % 5 ? .08 : count % 7 ? .15 : count % 9 ? .19 : count % 4 ? .23 : count % 6 ? .42 : count % 8 ? .0118 : count % 2 ? .0815 : 0);
			while (face->vertices[i]){
				face->vertices[i]->matrixed_x = face->vertices[i]->x - scene->origin.x;
				face->vertices[i]->matrixed_y = face->vertices[i]->y - scene->origin.y;
				face->vertices[i]->matrixed_z = face->vertices[i]->z - scene->origin.z;

				multiplyPointWithMatrix(face->vertices[i], scene->model_matrix);
				multiplyPointWithMatrix(face->vertices[i], scene->persepective_matrix);

				// coordinates
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_x;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_y;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_z;
				
				// colours
				g_vertex_buffer_data[j++] = face->material->diffuse_color->r + colour_offset;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->g + colour_offset;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->b + colour_offset;

				// textures
				if (face->texture_coordinates[i]){
					g_vertex_buffer_data[j++] = face->texture_coordinates[i]->u;
					g_vertex_buffer_data[j++] = face->texture_coordinates[i]->v;
				}
				else{
					g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_x;
					g_vertex_buffer_data[j++] = -face->vertices[i]->matrixed_y;
				}

				pnt_nb++;
				i++;
			}
			face = face->next;
			count++;
		}
		obj = obj->next;
	}
}