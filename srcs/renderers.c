#include "../include/scop.h"

unsigned int			positionBufferObject;
extern GLfloat			*g_vertex_buffer_data;

void	focus_render(t_scene *scene, t_faces *face, float colour_offset, int palette_idx, size_t count, size_t *j, size_t i){
	float colour_palette[10] = {.4, .8, .15, .16, .23, .42, .24, .32, .61, .51,};
	float r, g, b;
	if (scene->normal_mode){
		r = face->normal.x;
		g = face->normal.y;
		b = face->normal.z;
	}
	else if (scene->material_normal_mode){
		r = face->material->diffuse_colour->r + face->normal.x;
		g = face->material->diffuse_colour->g + face->normal.y;
		b = face->material->diffuse_colour->b + face->normal.z;
	}
	else if (scene->material_mode){
		r = face->material->diffuse_colour->r;
		g = face->material->diffuse_colour->g;
		b = face->material->diffuse_colour->b;
	}
	else if (scene->material_relief_mode){
		r = face->material->diffuse_colour->r + colour_offset > 1 ? face->material->diffuse_colour->r - colour_offset : face->material->diffuse_colour->r + colour_offset;
		g = face->material->diffuse_colour->g + colour_offset > 1 ? face->material->diffuse_colour->g - colour_offset : face->material->diffuse_colour->g + colour_offset;
		b = face->material->diffuse_colour->b + colour_offset > 1 ? face->material->diffuse_colour->b - colour_offset : face->material->diffuse_colour->b + colour_offset;
	}
	else{
		r = colour_palette[palette_idx];
		g = colour_palette[palette_idx];
		b = colour_palette[palette_idx];
	}
	printf("%zu %f %f %f %f %f %f\n", count, face->vertices[i]->x - scene->origin.x, face->vertices[i]->y - scene->origin.y, face->vertices[i]->z - scene->origin.z, r, g, b);
	g_vertex_buffer_data[*j ] = 1;
	g_vertex_buffer_data[*j + 1] = 0;
	g_vertex_buffer_data[*j + 2] = 0;
	*j += 3;
}

void	render_obj(t_scene *scene, t_obj *obj){
	size_t	i = 0, j = 0, count = 0, pnt_nb = 0;
	double colour_offset;
	float colour_palette[10] = {.4, .8, .15, .16, .23, .42, .24, .32, .61, .51,};
	int	palette_idx = 0;
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
				
				if (!face->material->diffuse_colour){
					dprintf(2,"Material %s is missing it's ambient colours, default will be used\n", face->material->name);
					face->material->diffuse_colour = scene->default_material->ambient_colour;
				}

				// colours
				if (face->focused && SCOP_DEBUG){
					focus_render(scene, face, colour_offset, palette_idx, count, &j, i);
				}
				else{
					if (scene->normal_mode){
						g_vertex_buffer_data[j++] = face->normal.x;
						g_vertex_buffer_data[j++] = face->normal.y;
						g_vertex_buffer_data[j++] = face->normal.z;
					}
					else if (scene->material_normal_mode){
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->r + face->normal.x;
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->g + face->normal.y;
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->b + face->normal.z;
					}
					else if (scene->material_mode){
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->r;
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->g;
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->b;
					}
					else if (scene->material_relief_mode){
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->r + colour_offset > 1 ? face->material->diffuse_colour->r - colour_offset : face->material->diffuse_colour->r + colour_offset;
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->g + colour_offset > 1 ? face->material->diffuse_colour->g - colour_offset : face->material->diffuse_colour->g + colour_offset;
						g_vertex_buffer_data[j++] = face->material->diffuse_colour->b + colour_offset > 1 ? face->material->diffuse_colour->b - colour_offset : face->material->diffuse_colour->b + colour_offset;
					}
					else{
						g_vertex_buffer_data[j++] = colour_palette[palette_idx];
						g_vertex_buffer_data[j++] = colour_palette[palette_idx];
						g_vertex_buffer_data[j++] = colour_palette[palette_idx];
					}
				}
				if (palette_idx >= 9)
					palette_idx = 0;

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
			palette_idx++;
			count++;
		}
		obj = obj->next;
	}
}