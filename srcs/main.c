#include "../include/scop.h"

struct s_garbage	*g_garbage_collector_root = 0x0;
struct s_garbage	*g_garbage_collector = 0x0;
struct s_scene		*scene;
int					window_fd = -1;
unsigned int		texture;
GLuint 				programID = -1, programTextureID, VAO = -1;
GLuint				VBO, EBO;
unsigned long long	iteration = 0;

GLfloat				*g_vertex_buffer_data;

short				key_press[348];
float				texture_change;


GLFWwindow	*init_opengl(){
	if (glfwInit() == GLFW_FALSE){
		free_garbage();
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(scene->width, scene->height, "Scop", NULL, NULL);
	if (window == NULL)
	{
		dprintf(2, "Failed to create GLFW window\n");
		glfwTerminate();
		return 0x0;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeViewport);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		dprintf(2, "Failed to initialize GLAD\n");
		glfwTerminate();
        return 0x0;
    }
	programID = loadShaders("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	use_texture();
	return (window);
}

void	print_command(){
	printf("translation :\n");
	printf("\t [x|y|z] + [keypad add || keypad minus]\n");
	printf("scale :\n");
	printf("\t 's' + [x|y|z] + [keypad add || keypad minus]\n");
	printf("rotation :\n");
	printf("\t [x|y|z] + [down arrow || up arrow]\n");
	printf("auto rotation toggle :\n");
	printf("\t 'o' + [x|y|z]\n");
	printf("change colour mode :\n");
	printf("\t 'v'\n");
	printf("toggle wireframe mode :\n");
	printf("\t 'w'\n");
	printf("toggle texture mode :\n");
	printf("\t 't'\n");
	printf("zoom in/out :\n");
	printf("\t only [keypad add || keypad minus]\n");
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

	int fd = open_file(argv[1], ".obj");
	if (fd < 0)
		free_garbage();
	scene = parse_scene(fd, argv[1]);

	g_vertex_buffer_data = malloc(sizeof(GLfloat) * ((scene->display_vertices_count * 8) + 1));
	add_to_garbage(g_vertex_buffer_data);
	for (size_t i = 0; i<=(scene->display_vertices_count * 8);i++){
		g_vertex_buffer_data[i] = 0.0;
	}
	for (int i =0;i<348;i++){
		key_press[i] = 0;
	}

	GLFWwindow *window = init_opengl();
	if (!window){
		t_garbage *tmp = g_garbage_collector_root;
		while (g_garbage_collector_root){
			free(g_garbage_collector_root->addr);
			tmp = g_garbage_collector_root;
			g_garbage_collector_root = g_garbage_collector_root->next;
			free(tmp);
		}
	}

	print_command();
	while (!glfwWindowShouldClose(window)){
		render(window);
		glfwPollEvents();
	}
	t_garbage *tmp = g_garbage_collector_root;
	while (g_garbage_collector_root){
		free(g_garbage_collector_root->addr);
		tmp = g_garbage_collector_root;
		g_garbage_collector_root = g_garbage_collector_root->next;
		free(tmp);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);

}

void	use_texture(){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width = 0, height = 0;
	//unsigned char	*data = parse_xpm("texture/NewCats.xpm", &width, &height);
	unsigned char	*data = parse_xpm("texture/beehive_front_honey.xpm", &width, &height);
	if (!data){
		glfwTerminate();
		add_to_garbage(0x0);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void	print_error(const char *fmt, va_list ap){
	vdprintf(2, fmt, ap);
}

void	render(GLFWwindow *window){
	input_handler(window);
	openglObjInit();

	setMatrix(scene);
	
	render_obj(scene, scene->objs_list);

	glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glDepthFunc(GL_LESS);
	float	colour_oppacity_location = glGetUniformLocation(programID, "textureOppacity");

	GLuint modelMatrix = glGetUniformLocation(programID, "model");
	GLuint viewMatrix = glGetUniformLocation(programID, "view");
	GLuint projectionMatrix = glGetUniformLocation(programID, "projection");

	glUseProgram(programID);

	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &scene->model_matrix[0][0]);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &scene->matrix_camera[0][0]);
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, &scene->persepective_matrix[0][0]);

	
	if (texture_change){
		if (scene->colour_mode){
			scene->colour_oppacity += .05;
			if (scene->colour_oppacity >= 1){
				texture_change = 0;
				scene->colour_mode = 0;
				scene->texture_mode = 1;
				scene->colour_oppacity = 1;
			}
		}
		else{
			scene->colour_oppacity -= .05;
			if (scene->colour_oppacity <= 0){
				texture_change = 0;
				scene->colour_mode = 1;
				scene->texture_mode = 0;
				scene->colour_oppacity = 0;
			}
		}
	}

	if (scene->x_auto_rotate){
		scene->x_angle++;
		if (scene->x_angle > 359)
			scene->x_angle = 0;
	}
	if (scene->y_auto_rotate){
		scene->y_angle++;
		if (scene->y_angle > 359)
			scene->y_angle = 0;
	}
	if (scene->z_auto_rotate){
		scene->z_angle++;
		if (scene->z_angle > 359)
			scene->z_angle = 0;
	}

	glUniform1f(colour_oppacity_location, scene->colour_oppacity);


	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, scene->wireframe_view ? GL_LINE : GL_FILL);

	glDrawArrays(GL_TRIANGLES, 0, scene->display_vertices_count);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glfwSwapBuffers(window);
}

void	resizeViewport(GLFWwindow *window, int width, int height){
	(void)window;
	glViewport(0, 0, width, height);
	openglObjInit();
}

void	openglObjInit(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (scene->display_vertices_count * 8) * sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}