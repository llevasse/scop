#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;


// E = B-A = ( Bx-Ax, By-Ay )
// F = D-C = ( Dx-Cx, Dy-Cy ) 
// P = ( -Ey, Ex )
// h = ( (A-C) * P ) / ( F * P )

void	triangulate(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	short	crossed[tab_size - 1][tab_size - 1];
	t_vertices	*point[tab_size];
	size_t		index[tab_size - 1];
	for (size_t i = 1; i < tab_size; i++){
		for (size_t j = 0; j < tab_size; j++){
			crossed[i - 1][j] = 0;
		}
		point[i - 1] = 0x0;
		index[i - 1] = atoi(tab[i]);
	}

	printf("%c", ' ');
	for (size_t i = 0; i < tab_size - 1; i++){
		printf("%4zu", index[i]);
	}
	printf("\n");
	for (size_t i = 0; i < tab_size - 1; i++){
		printf("%zu", index[i]);
		for (size_t j = 0; j < tab_size - 1; j++){
			printf("%4d", crossed[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	//t_vertices E = subtract_vectors()
	(void)scene;
	(void)line_nb;
}
