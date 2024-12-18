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
	size_t		i = 0, j =0, l=0;
//	printf("%s\n", obj->name);
	while (current_face){
		i = 0;
		while (current_face->vertices[i]){
//			printf("process vertex %zu :\n", i);
			multiplyPointWithMatrix(scene, current_face->vertices[i++]);
		}
		float  vertexPositions[i*4];
		j = 0;
		l = 0;
//		printf("%zu vertices to render\n", i - 1);
		while (j < i){
			vertexPositions[l] = current_face->vertices[j]->matrixed_x;
			vertexPositions[l+1] = current_face->vertices[j]->matrixed_y;
			vertexPositions[l+2] = current_face->vertices[j]->matrixed_z;
//			printf("vertex %zu :\n", j);
//			printf("\t%f\n",current_face->vertices[j]->matrixed_x); 
//			printf("\t%f\n",current_face->vertices[j]->matrixed_y); 
//			printf("\t%f\n",current_face->vertices[j]->matrixed_z);
			vertexPositions[l+3] = current_face->vertices[j]->w;
			l += 4;
			j++;
		}
		initVertexBuffer(vertexPositions);
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, i);
		current_face = current_face->next;
	}
}