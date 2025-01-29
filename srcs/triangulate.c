#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;


// E = B-A = ( Bx-Ax, By-Ay )
// F = D-C = ( Dx-Cx, Dy-Cy ) 
// P = ( -Ey, Ex )
// h = ( (A-C) * P ) / ( F * P )

short	check_if_segment_cross(t_vertices *v1, t_vertices *v2, t_vertices *v3, t_vertices *v4){
	t_vertices E = subtract_vectors(v2, v1);
	t_vertices F = subtract_vectors(v4, v3);
	t_vertices G = subtract_vectors(v3, v1);
	t_vertices P = E;
	P.y = -P.y;
	float	h = (vector_dot_product(&G, &P) / vector_dot_product(&F, &P));
	return (h >= 0 && h <= 1);
}

void	triangulate(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	short	crossed[tab_size - 1][tab_size - 1];
	t_vertices	*v1, *v2, *v3, *v4;
	size_t		index[tab_size - 1];
	for (size_t i = 1; i < tab_size; i++){
		for (size_t j = 0; j < tab_size; j++){
			crossed[i - 1][j] = 0;
		}
		index[i - 1] = atoi(tab[i]);
	}


	for (size_t i = 0; i < tab_size - 1; i++){
		for (size_t j = 0; j < tab_size - 1; j++){
			if (j == i)
				continue;
			v1 = scene->vertices_tab[index[i]];
			v2 = scene->vertices_tab[index[j]];
			for (size_t l = 0; l < tab_size - 1; l++){
				if (l == j || l == i)
					continue;
				v3 = scene->vertices_tab[index[l]];
				size_t k = l + 1;
				if (k >= tab_size)
					k = 0;
				if (k == l || k == i || k == j)
					continue;
				v4 = scene->vertices_tab[index[k]];
				crossed[i][j] = check_if_segment_cross(v1, v2, v3, v4);
			}
		}
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

	(void)scene;
	(void)line_nb;
}
