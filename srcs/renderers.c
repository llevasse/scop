#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;
extern GLfloat			*g_vertex_buffer_data;

void	render_obj(t_scene *scene, t_obj *obj){
	size_t	i = 0, j = 0, count = 0, pnt_nb = 0;
	double colour_offset;
	while (obj){
		t_faces	*face = obj->faces;
		while (face){
			i = 0;
			colour_offset = (count % 3 ? .04 : count % 5 ? .08 : count % 7 ? .15 : count % 9 ? .19 : count % 4 ? .23 : count % 6 ? .42 : count % 8 ? .0118 : count % 2 ? .0815 : 0);
			while (face->vertices[i]){
				// coordinates
				g_vertex_buffer_data[j++] = face->vertices[i]->x - scene->origin.x;
				g_vertex_buffer_data[j++] = face->vertices[i]->y - scene->origin.y;
				g_vertex_buffer_data[j++] = face->vertices[i]->z - scene->origin.z;
				
				// colours
				if (!face->focused){
					if (scene->normal_mode){
						g_vertex_buffer_data[j++] = face->material->diffuse_color->r + face->normal.x;
						g_vertex_buffer_data[j++] = face->material->diffuse_color->g + face->normal.y;
						g_vertex_buffer_data[j++] = face->material->diffuse_color->b + face->normal.z;
					}
					else{
						g_vertex_buffer_data[j++] = face->material->diffuse_color->r + colour_offset > 1 ? face->material->diffuse_color->r - colour_offset : face->material->diffuse_color->r + colour_offset;
						g_vertex_buffer_data[j++] = face->material->diffuse_color->g + colour_offset > 1 ? face->material->diffuse_color->g - colour_offset : face->material->diffuse_color->g + colour_offset;
						g_vertex_buffer_data[j++] = face->material->diffuse_color->b + colour_offset > 1 ? face->material->diffuse_color->b - colour_offset : face->material->diffuse_color->b + colour_offset;
					}
				}
				else{
					float r, g, b;
					if (scene->normal_mode){
						r = face->material->diffuse_color->r + face->normal.x;
						g = face->material->diffuse_color->g + face->normal.y;
						b = face->material->diffuse_color->b + face->normal.z;
					}
					else{
						r = face->material->diffuse_color->r + colour_offset > 1 ? face->material->diffuse_color->r - colour_offset : face->material->diffuse_color->r + colour_offset;
						g = face->material->diffuse_color->g + colour_offset > 1 ? face->material->diffuse_color->g - colour_offset : face->material->diffuse_color->g + colour_offset;
						b = face->material->diffuse_color->b + colour_offset > 1 ? face->material->diffuse_color->b - colour_offset : face->material->diffuse_color->b + colour_offset;
					}
				printf("%zu %f %f %f %f %f %f\n", count, face->vertices[i]->x - scene->origin.x, face->vertices[i]->y - scene->origin.y, face->vertices[i]->z - scene->origin.z, r, g, b);
					g_vertex_buffer_data[j++] = 1;
					g_vertex_buffer_data[j++] = 0;
					g_vertex_buffer_data[j++] = 0;
				}

				// textures
				if (face->texture_coordinates[i]){
					g_vertex_buffer_data[j++] = face->texture_coordinates[i]->u;
					g_vertex_buffer_data[j++] = face->texture_coordinates[i]->v;
				}
				else{
					if (face->direction == 'x'){
						g_vertex_buffer_data[j++] = face->vertices[i]->x;
						g_vertex_buffer_data[j++] = -face->vertices[i]->y;
					}
					else if (face->direction == 'z'){
						g_vertex_buffer_data[j++] = face->vertices[i]->z;
						g_vertex_buffer_data[j++] = -face->vertices[i]->y;
					}
					else{
						g_vertex_buffer_data[j++] = face->vertices[i]->x;
						g_vertex_buffer_data[j++] = -face->vertices[i]->z;
					}
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