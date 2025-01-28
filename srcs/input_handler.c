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
				scene->y_offset += scene->translation_step;
		}
		else if (glfwGetKey(window, GLFW_KEY_X)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->x_scale += .1;
			}	
			else
				scene->x_offset += scene->translation_step;
		}
		else if (glfwGetKey(window, GLFW_KEY_Z)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->z_scale += .1;
			}	
			else
				scene->z_offset += scene->translation_step;
		}
		else if (glfwGetKey(window, GLFW_KEY_S)){
			scene->x_scale += .05;
			scene->y_scale += .05;
			scene->z_scale += .05;
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
				scene->y_offset -= scene->translation_step;
		}
		else if (glfwGetKey(window, GLFW_KEY_X)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->x_scale -= .1;
			}	
			else
				scene->x_offset -= scene->translation_step;
		}
		else if (glfwGetKey(window, GLFW_KEY_Z)){
			if (glfwGetKey(window, GLFW_KEY_S)){
				scene->z_scale -= .1;
			}	
			else
				scene->z_offset -= scene->translation_step;
		}
		else if (glfwGetKey(window, GLFW_KEY_S)){
			scene->x_scale -= .05;
			scene->y_scale -= .05;
			scene->z_scale -= .05;
		}
		else if (!key_press[GLFW_KEY_KP_SUBTRACT]){
			scene->fov++;
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_P) && !key_press[GLFW_KEY_P]){
		printf("rotation x:%f y:%f z:%f\n", scene->x_angle, scene->y_angle, scene->z_angle);
		printf("offset x:%f y:%f z:%f\n", scene->x_offset, scene->y_offset, scene->z_offset);
		printf("translation step %f\n", scene->translation_step);
		printf("scale : %f %f %f\n", scene->x_scale, scene->y_scale, scene->z_scale);
		printf("\tsize x : %f\n", scene->max_x - scene->min_x);
		printf("\tsize y : %f\n", scene->max_y - scene->min_y);
		printf("\tsize z : %f\n", scene->max_z - scene->min_z);

		for (size_t i = 0; i < scene->display_vertices_count * 8; i += 24){
			for (size_t j = i; j < i + 24; j += 8)
				printf("%f %f %f\t\t%f %f %f\n", g_vertex_buffer_data[i], g_vertex_buffer_data[i + 1], g_vertex_buffer_data[i + 2], g_vertex_buffer_data[i + 3], g_vertex_buffer_data[i + 4], g_vertex_buffer_data[i + 5]);
			printf("\n");
		}
		if (scene->focus){
			for (int i =0 ;i<3;i++){
				printf("%f %f %f\n", scene->focus->vertices[i]->x, scene->focus->vertices[i]->y, scene->focus->vertices[i]->z);
			}

			printf("normal : %f %f %f\n", scene->focus->normal.x, scene->focus->normal.y, scene->focus->normal.z);

			printf("direction : %c\n", scene->focus->direction);
			printf("\n");
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) && !key_press[GLFW_KEY_TAB]){
		if (scene->focus){
			scene->focus->focused = 0;
			if (scene->focus->next){
				scene->focus = scene->focus->next;
				scene->focus->focused = 1;
			}
			else{
				scene->focus = 0x0;
			}
		}
		else{
			scene->focus = scene->objs_list->faces;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_T) && !key_press[GLFW_KEY_T]){
		texture_change = 1;
	}
	
	for (int i =0; i<348;i++){
		key_press[i] = glfwGetKey(window, i);
	}
}
