#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;
extern GLfloat			*g_vertex_buffer_data;
extern GLuint			*g_element_buffer_data;
extern short			*g_matrixed_vertices_check;


void	initVertexBuffer(const float *vertexPositions){
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	set_quaternions(t_scene *scene, t_quaternions *qx, t_quaternions *qy, t_quaternions *qz){
	qx->w = cos((scene->x_angle * RADIAN) / 2);
	qx->x = sin((scene->x_angle * RADIAN) / 2) * 1;
	qx->y = 0;
	qx->z = 0;


	qy->w = cos((scene->y_angle * RADIAN) / 2);
	qy->x = 0;
	qy->y = sin((scene->y_angle * RADIAN) / 2) * 1;
	qy->z = 0;


	qz->w = cos((scene->z_angle * RADIAN) / 2);
	qz->x = 0;
	qz->y = 0;
	qz->z = sin((scene->z_angle * RADIAN) / 2) * 1;
}

t_quaternions	get_quaternions_product(t_quaternions r, t_quaternions s){
	t_quaternions q;
	q.w = (r.w * s.w) - (r.x * s.x) - (r.y * s.y) - (r.z * s.z);
	q.x = (r.w * s.x) + (r.x * s.w) - (r.y * s.z) + (r.z * s.y);
	q.y = (r.w * s.y) + (r.x * s.z) + (r.y * s.w) - (r.z * s.x);
	q.z = (r.w * s.z) - (r.x * s.y) + (r.y * s.x) + (r.z * s.w);
	return (q);
}

t_quaternions	get_quaternions_inversion(t_quaternions r){
	t_quaternions q = r;
	q.x = -q.x;
	q.y = -q.y;
	q.z = -q.z;
	return (q);
}

void	rotate_point(t_vertices *v, t_quaternions qx, t_quaternions qy, t_quaternions qz){
	t_quaternions p;
	p.w = 0;
	p.x = v->matrixed_x;
	p.y = v->matrixed_y;
	p.z = v->matrixed_z;

	p = get_quaternions_product(get_quaternions_product(get_quaternions_inversion(qx), p), qx);
	p = get_quaternions_product(get_quaternions_product(get_quaternions_inversion(qy), p), qy);
	p = get_quaternions_product(get_quaternions_product(get_quaternions_inversion(qz), p), qz);

	v->matrixed_x = p.x;
	v->matrixed_y = p.y;
	v->matrixed_z = p.z;
}

void	render_obj(t_scene *scene, t_obj *obj){
	(void)scene;
	(void)obj;
	t_quaternions qx, qy, qz;
	set_quaternions(scene, &qx, &qy, &qz);
	size_t	i = 0, j = 0, count = 0, pnt_nb = 0, segment_idx = 0;
	while (obj){
		t_faces	*face = obj->faces;
		while (face){
			i = 0;
			while (face->vertices[i]){
				//printf("g_vertex_buffer_data position : %zu, v id : %zu\n", j, face->vertices[i]->id);
				//printf ("\t%f %f %f\n", face->vertices[i]->x, face->vertices[i]->y, face->vertices[i]->z);
				face->vertices[i]->matrixed_x = face->vertices[i]->x - scene->origin.x + scene->x_offset;
				face->vertices[i]->matrixed_y = face->vertices[i]->y - scene->origin.y + scene->y_offset;
				face->vertices[i]->matrixed_z = face->vertices[i]->z - scene->origin.z + scene->z_offset;
				multiplyPointWithMatrix(scene, face->vertices[i], scene->matrix_camera);
				multiplyPointWithMatrix(scene, face->vertices[i], scene->matrix);
				rotate_point(face->vertices[i], qx, qy, qz);

				//multiplyPointWithRotationsMatrixes(scene, face->vertices[i]);
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_x;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_y;
				g_vertex_buffer_data[j++] = face->vertices[i]->matrixed_z;
				
				g_vertex_buffer_data[j++] = face->material->diffuse_color->r;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->g;
				g_vertex_buffer_data[j++] = face->material->diffuse_color->b;				
				g_matrixed_vertices_check[face->vertices[i]->id] = 1;
				pnt_nb++;
				i++;
			}
			g_element_buffer_data[segment_idx++] = face->vertices[0]->id;
			g_element_buffer_data[segment_idx++] = face->vertices[1]->id;
			g_element_buffer_data[segment_idx++] = face->vertices[2]->id;
			face = face->next;
			count++;
		}
		obj = obj->next;
	}

	for (size_t i = 0; i<scene->vertices_count;i++)
		g_matrixed_vertices_check[i] = 0;
}