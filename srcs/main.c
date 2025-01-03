#include "../include/scop.h"

struct s_garbage	*g_garbage_collector_root = 0x0;
struct s_garbage	*g_garbage_collector = 0x0;
struct s_scene		*scene;
int					window_fd = -1;
GLuint 				programID = -1, VAO = -1;
GLuint				VBO, EBO;
GLfloat				*g_vertex_buffer_data;
GLuint				*g_element_buffer_data;

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

	for (size_t i=0; i<(scene->display_vertices_count * 2) - 1; i += 2){
		printf("%d(%f %f) %d(%f %f)\n", g_element_buffer_data[i], scene->vertices_tab[g_element_buffer_data[i]]->matrixed_x, scene->vertices_tab[g_element_buffer_data[i]]->matrixed_y, g_element_buffer_data[i + 1], scene->vertices_tab[g_element_buffer_data[i + 1]]->matrixed_x, scene->vertices_tab[g_element_buffer_data[i + 1]]->matrixed_y);
	}
	

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (scene->display_vertices_count * 6) * sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (scene->display_vertices_count * 2) * sizeof(unsigned int), g_element_buffer_data, GL_STATIC_DRAW);

	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram(programID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Draw the triangle !
	glDrawElements(GL_TRIANGLES, scene->display_vertices_count * 2, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, scene->display_vertices_count);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glutSwapBuffers();
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
	glutInitWindowSize(720, 720);
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
	g_vertex_buffer_data = malloc(sizeof(GLfloat) * ((scene->display_vertices_count * 6) + 1));
	add_to_garbage(g_vertex_buffer_data);
	g_element_buffer_data = malloc(sizeof(GLuint) * (scene->display_vertices_count * 2) + 1);
	add_to_garbage(g_element_buffer_data);
	for (size_t i = 0; i<=(scene->display_vertices_count * 6);i++){
		g_vertex_buffer_data[i] = 0.0;
	}
	scene->fov = 90;
	scene->far_plane_distance = 100;
	scene->near_plane_distance = .1;
	programID = loadShaders("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutMainLoop();
}
