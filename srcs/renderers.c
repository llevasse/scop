#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
unsigned int			positionBufferObject;

static const GLfloat g_vertex_buffer_data[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};

void	initVertexBuffer(const float *vertexPositions){
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	render_obj(t_scene *scene, t_obj *obj){
	(void)scene;
	(void)obj;
/*	t_faces		*current_face = obj->faces;
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
	}*/
	
	// This will identify our vertex buffer
	initVertexBuffer(g_vertex_buffer_data);
	
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	/*
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);	
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);*/
}