#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;
extern GLfloat			*g_vertex_buffer_data;
extern struct s_scene	*scene;
extern short			key_press[348];
extern float			texture_change;


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
				scene->z_offset += .01;
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
				scene->z_offset -= .01;
		}
		else if (glfwGetKey(window, GLFW_KEY_S)){
			scene->x_scale -= .1;
			scene->y_scale -= .1;
			//scene->z_scale -= .1;
		}
		else if (!key_press[GLFW_KEY_KP_SUBTRACT]){
			scene->fov++;
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_P)){
		printf("rotation x:%f y:%f z:%f\n", scene->x_angle, scene->y_angle, scene->z_angle);
		printf("offset x:%f y:%f z:%f\n", scene->x_offset, scene->y_offset, scene->z_offset);
		printf("scale : %f %f %f\n", scene->x_scale, scene->y_scale, scene->z_scale);

		t_faces *tmp = scene->objs_list->faces;
		while (tmp){
			for (int i =0 ;i<3;i++){
				printf("%f %f %f\n", tmp->vertices[i]->x, tmp->vertices[i]->y, tmp->vertices[i]->z + scene->z_offset);
			}
			printf("\n");
			tmp = tmp->next;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_T) && !key_press[GLFW_KEY_T]){
		texture_change = 1;
	}
	
	for (int i =0; i<348;i++){
		key_press[i] = glfwGetKey(window, i);
	}
}
