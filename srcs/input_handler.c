#include "../include/scop.h"

extern GLfloat			*g_vertex_buffer_data;
extern struct s_scene	*scene;


void	print_scop(t_scene *scene){
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

void	set_auto_rotate(short x, short y, short z){
	scene->x_auto_rotate = x;
	scene->y_auto_rotate = y;
	scene->z_auto_rotate = z;
}

void	rotate_input(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_DOWN)){
		if (glfwGetKey(window, GLFW_KEY_X)){
			set_auto_rotate(0, 0, 0);
			scene->x_angle--;
			if (scene->x_angle < 0)
				scene->x_angle = 360;
		}
		if (glfwGetKey(window, GLFW_KEY_Y)){
			set_auto_rotate(0, 0, 0);
			scene->y_angle--;
			if (scene->y_angle < 0)
				scene->y_angle = 360;
		}
		if (glfwGetKey(window, GLFW_KEY_Z)){
			set_auto_rotate(0, 0, 0);
			scene->z_angle--;
			if (scene->z_angle < 0)
				scene->z_angle = 360;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_UP)){
		if (glfwGetKey(window, GLFW_KEY_X)){
			set_auto_rotate(0, 0, 0);
			scene->x_angle++;
			if (scene->x_angle > 360)
				scene->x_angle = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_Y)){
			set_auto_rotate(0, 0, 0);
			scene->y_angle++;
			if (scene->y_angle > 360)
				scene->y_angle = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_Z)){
			set_auto_rotate(0, 0, 0);
			scene->z_angle++;
			if (scene->z_angle > 360)
				scene->z_angle = 0;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_O)){
		if (glfwGetKey(window, GLFW_KEY_X) && !scene->key_press[GLFW_KEY_X])
			scene->x_auto_rotate = !scene->x_auto_rotate;
		if (glfwGetKey(window, GLFW_KEY_Y) && !scene->key_press[GLFW_KEY_Y])
			scene->y_auto_rotate = !scene->y_auto_rotate;
		if (glfwGetKey(window, GLFW_KEY_Z) && !scene->key_press[GLFW_KEY_Z])
			scene->z_auto_rotate = !scene->z_auto_rotate;
	}
}

void	translate_scale_zoom_input(GLFWwindow *window){
	short	moved = 0;
	if (glfwGetKey(window, GLFW_KEY_Y)){
		if (glfwGetKey(window, GLFW_KEY_S)){
			scene->y_scale += glfwGetKey(window, GLFW_KEY_KP_ADD) ? .1 : -.1;
		}	
		else{
			scene->y_offset += glfwGetKey(window, GLFW_KEY_KP_ADD) ? scene->translation_step : -scene->translation_step;
			moved = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_X)){
		if (glfwGetKey(window, GLFW_KEY_S)){
			scene->x_scale += glfwGetKey(window, GLFW_KEY_KP_ADD) ? .1 : -.1;
		}	
		else{
			scene->x_offset += glfwGetKey(window, GLFW_KEY_KP_ADD) ? scene->translation_step : -scene->translation_step;
			moved = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Z)){
		if (glfwGetKey(window, GLFW_KEY_S)){
			scene->z_scale += glfwGetKey(window, GLFW_KEY_KP_ADD) ? .1 : -.1;
		}	
		else{
			scene->z_offset += glfwGetKey(window, GLFW_KEY_KP_ADD) ? scene->translation_step : -scene->translation_step;
			moved = 1;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_S)){
		scene->x_scale += glfwGetKey(window, GLFW_KEY_KP_ADD) ? .05 : -.05;
		scene->y_scale += glfwGetKey(window, GLFW_KEY_KP_ADD) ? .05 : -.05;
		scene->z_scale += glfwGetKey(window, GLFW_KEY_KP_ADD) ? .05 : -.05;
	}
	if (!moved){
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) && !scene->key_press[GLFW_KEY_KP_ADD]){
			scene->fov--;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) && !scene->key_press[GLFW_KEY_KP_SUBTRACT]){
			scene->fov++;
		}
	}
}

void	input_handler(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, 1);

	if (glfwGetKey(window, GLFW_KEY_W) && !scene->key_press[GLFW_KEY_W])
		scene->wireframe_view = scene->wireframe_view == 1 ? 0 : 1;

	rotate_input(window);

	if (glfwGetKey(window, GLFW_KEY_KP_ADD) || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT))
		translate_scale_zoom_input(window);

	if (glfwGetKey(window, GLFW_KEY_P) && !scene->key_press[GLFW_KEY_P] && SCOP_DEBUG){
		print_scop(scene);
	}

	else if ((glfwGetKey(window, GLFW_KEY_TAB) && (scene->key_press[GLFW_KEY_TAB] == 1 || scene->key_press[GLFW_KEY_TAB] == 42))){
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
		
		render_obj(scene, scene->objs_list);
		void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		ft_memcpy(ptr, g_vertex_buffer_data, (scene->display_vertices_count * 8) * sizeof(float));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	if (glfwGetKey(window, GLFW_KEY_T) && !scene->key_press[GLFW_KEY_T]){
		scene->texture_change = 1;
	}
	
	if (glfwGetKey(window, GLFW_KEY_V) && !scene->key_press[GLFW_KEY_V]){
		if (scene->material_mode){
			scene->material_mode = 0;
			scene->material_relief_mode = 1;
		}
		else if (scene->material_relief_mode){
			scene->material_relief_mode = 0;
			scene->normal_mode = 1;
		}
		else if (scene->normal_mode){
			scene->normal_mode = 0;
			scene->material_normal_mode = 1;
		}
		else if (scene->material_normal_mode){
			scene->material_normal_mode = 0;
		}
		else{
			scene->material_mode = 1;
		}
		render_obj(scene, scene->objs_list);
		void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		ft_memcpy(ptr, g_vertex_buffer_data, (scene->display_vertices_count * 8) * sizeof(float));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
	for (int i =0; i<348;i++){
		if (glfwGetKey(window, i)){
			if (scene->key_press[i] != 42)
				scene->key_press[i]++;
		}
		else
			scene->key_press[i] = 0;
	}
}
