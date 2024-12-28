#include "../include/scop.h"

struct s_garbage	*g_garbage_collector_root = 0x0;
struct s_garbage	*g_garbage_collector = 0x0;
struct s_scene		*scene;
int					window_fd = -1;
GLuint 				programID = -1, vertexArrayId = -1;
GLuint				vertexbuffer;
GLfloat				*g_vertex_buffer_data;

void	print_error(const char *fmt, va_list ap){
	vdprintf(2, fmt, ap);
}

void	input_handler(unsigned char key, int x, int y){
	printf("%d %d %d\n", key, x, y);
	if (key == 27){
		t_garbage *tmp = g_garbage_collector_root;
		while (g_garbage_collector_root){
			free(g_garbage_collector_root->addr);
			tmp = g_garbage_collector_root;
			g_garbage_collector_root = g_garbage_collector_root->next;
			free(tmp);
		}
		glutLeaveMainLoop();
		glutDestroyWindow(window_fd);
		exit(0);
	}
}

void	render(){
	setMatrix(scene);
	
	render_obj(scene, scene->objs_list);
	for (size_t i=0; i<scene->vertices_count * 3; ){
		printf("%f ", g_vertex_buffer_data[i++]);
		printf("%f ", g_vertex_buffer_data[i++]);
		printf("%f\n", g_vertex_buffer_data[i++]);
	}
	

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->vertices_count * 3, g_vertex_buffer_data, GL_STATIC_DRAW);

	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram(programID);
	glEnableVertexAttribArray(0);
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
	glDrawArrays(GL_TRIANGLES, 0, scene->vertices_count * 3); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);
	glutSwapBuffers();

	/*glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glUseProgram(programID);
	glutSwapBuffers();*/
}

void	reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

int main(int argc, char **argv){
	if (argc != 2){
		dprintf(2, "wrong number of arguments\n");
		exit (1);
	}
	if (!g_garbage_collector_root){
		g_garbage_collector_root = malloc(sizeof(struct s_garbage));
		if (!g_garbage_collector_root){
			dprintf(2, "Couldn't allocate memory\n");
			exit(1);	
		}
		g_garbage_collector = g_garbage_collector_root;
		g_garbage_collector->next = 0x0;
		g_garbage_collector->addr = 0x0;
	}
	(void)argv;


	int foo = 1;
	char	*bar[1]= {" "};
	glutInit(&foo, bar);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(700, 700);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitErrorFunc(print_error);
	glutInitWarningFunc(print_error);
	window_fd = glutCreateWindow("Scop");
	glutKeyboardFunc(input_handler);
	GLenum err = glewInit();
	if (err != GLEW_OK){
		dprintf(2, "%s\n", glewGetErrorString(err));
	}
	int fd = open_file(argv[1], ".obj");
	if (fd < 0)
		free_garbage();
	scene = parse_scene(fd);
	g_vertex_buffer_data = malloc(sizeof(GLfloat) * ((scene->vertices_count * 3) + 1));
	add_to_garbage(g_vertex_buffer_data);
	scene->fov = 90;
	scene->far_plane_distance = 100;
	scene->near_plane_distance = .1;
	programID = loadShaders("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}
