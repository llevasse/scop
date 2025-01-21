#include "../include/scop.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

struct s_garbage	*g_garbage_collector_root = 0x0;
struct s_garbage	*g_garbage_collector = 0x0;
struct s_scene		*scene;
int					window_fd = -1;
unsigned int		texture;
short				key_press[348];
GLuint 				programID = -1, programTextureID, VAO = -1;
GLuint				VBO, EBO;
unsigned long long	iteration = 0;

GLfloat				*g_vertex_buffer_data;
short				*g_matrixed_vertices_check;

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
	scene = parse_scene(fd, argv[1]);
	printf("max/min x : %f %f\n", scene->max_x, scene->min_x);
	printf("max/min y : %f %f\n", scene->max_y, scene->min_y);
	printf("max/min z : %f %f\n", scene->max_z, scene->min_z);
	scene->origin.x = (scene->max_x + scene->min_x) / 2;
	scene->origin.y = (scene->max_y + scene->min_y) / 2;
	scene->origin.z = (scene->max_z + scene->min_z) / 2;

	printf("scene origin : %f %f %f\n", scene->origin.x, scene->origin.y, scene->origin.z);
	g_vertex_buffer_data = malloc(sizeof(GLfloat) * ((scene->display_vertices_count * 8) + 1));
	add_to_garbage(g_vertex_buffer_data);
	g_matrixed_vertices_check = malloc(sizeof(short) * (scene->vertices_count + 1));
	add_to_garbage(g_matrixed_vertices_check);
	for (size_t i = 0; i<scene->vertices_count;i++)
		g_matrixed_vertices_check[i] = 0;
	for (size_t i = 0; i<=(scene->display_vertices_count * 8);i++){
		g_vertex_buffer_data[i] = 0.0;
	}
	for (int i =0;i<348;i++){
		key_press[i] = 0;
	}
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(scene->width, scene->height, "Scop", NULL, NULL);
	if (window == NULL)
	{
		dprintf(2, "Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeViewport);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		dprintf(2, "Failed to initialize GLAD\n");
        return -1;
    }
	programID = loadShaders("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");
	programTextureID = loadShaders("./shaders/vertexShader.glsl", "./shaders/textureShader.glsl");
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	use_texture();


	while (!glfwWindowShouldClose(window)){
		input_handler(window);
		openglObjInit();
		render(window);

		//glfwPollEvents();
		glfwWaitEvents();
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

void	use_texture(){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("texture/minecraft_bee.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
		dprintf(2, "Failed to load default texture\n");
		add_to_garbage(0x0);
    }
    stbi_image_free(data);
}

void	print_error(const char *fmt, va_list ap){
	vdprintf(2, fmt, ap);
}

void	input_handler(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)){
		glfwSetWindowShouldClose(window, 1);
	}
	if (glfwGetKey(window, GLFW_KEY_W) && !key_press[GLFW_KEY_W]){
		scene->wireframe_view = scene->wireframe_view == 1 ? 0 : 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_R)){
		scene->x_angle = 0;
		scene->y_angle = 0;
		scene->z_angle = 0;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN)){
		if (glfwGetKey(window, GLFW_KEY_X)){
			scene->x_angle--;
			if (scene->x_angle < 0)
				scene->x_angle = 360;
		}
		if (glfwGetKey(window, GLFW_KEY_Y)){
			scene->y_angle--;
			if (scene->y_angle < 0)
				scene->y_angle = 360;
		}
		if (glfwGetKey(window, GLFW_KEY_Z)){
			scene->z_angle--;
			if (scene->z_angle < 0)
				scene->z_angle = 360;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_UP)){
		if (glfwGetKey(window, GLFW_KEY_X)){
			scene->x_angle++;
			if (scene->x_angle > 360)
				scene->x_angle = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_Y)){
			scene->y_angle++;
			if (scene->y_angle > 360)
				scene->y_angle = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_Z)){
			scene->z_angle++;
			if (scene->z_angle > 360)
				scene->z_angle = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_KP_ADD)){
		if (glfwGetKey(window, GLFW_KEY_Y)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->y_scale += .1;
			}	
			else
				scene->y_offset += .1;
		}
		else if (glfwGetKey(window, GLFW_KEY_X)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->x_scale += .1;
			}	
			else
				scene->x_offset += .1;
		}
		else if (glfwGetKey(window, GLFW_KEY_Z)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->z_scale += .1;
			}	
			else
				scene->z_offset += .1;
		}
		else if (glfwGetKey(window, GLFW_KEY_S)){
			scene->x_scale += .1;
			scene->y_scale += .1;
			//scene->z_scale += .1;
		}
		else if (!key_press[GLFW_KEY_KP_ADD]){
			scene->fov--;
		}
		// printf("rotation x:%f y:%f z:%f\n", scene->x_angle, scene->y_angle, scene->z_angle);
		// printf("offset x:%f y:%f z:%f\n", scene->x_offset, scene->y_offset, scene->z_offset);
		// printf("scale : %f %f %f\n", scene->x_scale, scene->y_scale, scene->z_scale);
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT)){
		if (glfwGetKey(window, GLFW_KEY_Y)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->y_scale -= .1;
			}	
			else
				scene->y_offset -= .1;
		}
		else if (glfwGetKey(window, GLFW_KEY_X)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->x_scale -= .1;
			}	
			else
				scene->x_offset -= .1;
		}
		else if (glfwGetKey(window, GLFW_KEY_Z)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->z_scale -= .1;
			}	
			else
				scene->z_offset -= .1;
		}
		else if (glfwGetKey(window, GLFW_KEY_S)){
			scene->x_scale -= .1;
			scene->y_scale -= .1;
			//scene->z_scale -= .1;
		}
		else if (!key_press[GLFW_KEY_KP_SUBTRACT]){
			scene->fov++;
		}
		// printf("rotation x:%f y:%f z:%f\n", scene->x_angle, scene->y_angle, scene->z_angle);
		// printf("offset x:%f y:%f z:%f\n", scene->x_offset, scene->y_offset, scene->z_offset);
		// printf("scale : %f %f %f\n", scene->x_scale, scene->y_scale, scene->z_scale);
	}
	else if (glfwGetKey(window, GLFW_KEY_P)){
		printf("rotation x:%f y:%f z:%f\n", scene->x_angle, scene->y_angle, scene->z_angle);
		printf("offset x:%f y:%f z:%f\n", scene->x_offset, scene->y_offset, scene->z_offset);
		printf("scale : %f %f %f\n", scene->x_scale, scene->y_scale, scene->z_scale);
	}
	
	if (glfwGetKey(window, GLFW_KEY_T) && !key_press[GLFW_KEY_T]){
		scene->texture_mode = !scene->texture_mode;
	}
	
	for (int i =0; i<348;i++){
		key_press[i] = glfwGetKey(window, i);
	}
}

void	render(GLFWwindow *window){
	setMatrix(scene);
	
	render_obj(scene, scene->objs_list);

	//printf("wireframe view : %d\n", scene->wireframe_view);
	//printf("zoom : %f\n", scene->zoom);
	/*for (size_t i=0; i<number_of_segment_to_display; i += 2){
		printf("%d %d\n", g_element_buffer_data[i], g_element_buffer_data[i + 1]);
	}
	printf("\n\n");*/
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glDepthFunc(GL_LESS);
	if (scene->texture_mode)
		glUseProgram(programTextureID);
	else
		glUseProgram(programID);
	
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, scene->wireframe_view ? GL_LINE : GL_FILL);
	/*printf("draw %llu\n", iteration++);
	printf("size : %zu\n", scene->display_vertices_count);*/
	//printf("Segment to display : %u\n", number_of_segment_to_display);
	
	// for (size_t i=0; i<scene->display_vertices_count; i+= 3){
	// 	for (int j = 0; j < 3;j++){
	// 		printf("vertex %u : ", g_element_buffer_data[i + j]);
	// 		for (int l=0;l < 8;l++){
	// 			printf("%f ", g_vertex_buffer_data[(g_element_buffer_data[i + j] * 8) + l]);
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("\n");
	// }

	//glDrawElements(GL_TRIANGLES, number_of_segment_to_display, GL_UNSIGNED_INT, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, scene->display_vertices_count);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glfwSwapBuffers(window);
	
}

void	reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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