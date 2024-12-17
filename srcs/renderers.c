#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;

void	initVertexBuffer(const float *vertexPositions){
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	(void)vertexPositions;
}

void	render_obj(t_scene *scene, t_obj *obj){
	t_faces		*current_face = obj->faces;
	size_t		i = 0, j =0;
	printf("%s\n", obj->name);
	while (current_face){
		while (current_face->vertices[i]){
			multiplyPointWithMatrix(scene, current_face->vertices[i++]);
		}
		float  vertexPositions[i];
		while (j + 3 < i){
			vertexPositions[j] = current_face->vertices[j]->matrixed_x;
			vertexPositions[j+1] = current_face->vertices[j]->matrixed_y;
			vertexPositions[j+2] = current_face->vertices[j]->matrixed_z;
			vertexPositions[j+3] = current_face->vertices[j]->w;
			j += 4;
		}
		initVertexBuffer(vertexPositions);

		current_face = current_face->next;
	}
}