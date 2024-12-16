#include "../include/scop.h"

struct s_garbage	*g_garbage_collector_root = 0x0;
struct s_garbage	*g_garbage_collector = 0x0;
int					window_fd = -1;
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
	glutInitContextVersion(4, 0);
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
	t_scene *scene = parse_scene(fd);
	scene->fov = 90;
	scene->far_plane_distance = 100;
	scene->near_plane_distance = .1;

	glutMainLoop();
}
