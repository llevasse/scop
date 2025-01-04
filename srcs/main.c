#include "../include/scop.h"

struct s_garbage	*g_garbage_collector_root = 0x0;
struct s_garbage	*g_garbage_collector = 0x0;
struct s_scene		*scene;
int					window_fd = -1;
GLuint 				programID = -1, VAO = -1;
GLuint				VBO, EBO;
GLfloat				*g_vertex_buffer_data;
GLuint				*g_element_buffer_data;

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

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		dprintf(2, "Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeViewport);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		dprintf(2, "Failed to initialize GLAD\n");
        return -1;
    }
	programID = loadShaders("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

	while (!glfwWindowShouldClose(window)){
		input_handler(window);

		render(window);

		glfwPollEvents();
	}
	glfwTerminate();
	t_garbage *tmp = g_garbage_collector_root;
	while (g_garbage_collector_root){
		free(g_garbage_collector_root->addr);
		tmp = g_garbage_collector_root;
		g_garbage_collector_root = g_garbage_collector_root->next;
		free(tmp);
	}
	return (0);

}

void	print_error(const char *fmt, va_list ap){
	vdprintf(2, fmt, ap);
}

void	input_handler(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, 1);
	}
}

void	render(GLFWwindow *window){
	setMatrix(scene);
	
	render_obj(scene, scene->objs_list);

	/*for (size_t i=0; i<(scene->display_vertices_count * 2) - 1; i += 2){
		printf("%d(%f %f) %d(%f %f)\n", g_element_buffer_data[i], scene->vertices_tab[g_element_buffer_data[i]]->matrixed_x, scene->vertices_tab[g_element_buffer_data[i]]->matrixed_y, g_element_buffer_data[i + 1], scene->vertices_tab[g_element_buffer_data[i + 1]]->matrixed_x, scene->vertices_tab[g_element_buffer_data[i + 1]]->matrixed_y);
	}*/
	

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

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
	glDrawElements(GL_LINES, scene->display_vertices_count * 2, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, scene->display_vertices_count);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glfwSwapBuffers(window);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void	reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void	resizeViewport(GLFWwindow *window, int width, int height){
	(void)window;
	glViewport(0, 0, width, height);
}