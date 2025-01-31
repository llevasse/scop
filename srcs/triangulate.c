#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;


// E = B-A = ( Bx-Ax, By-Ay )
// F = D-C = ( Dx-Cx, Dy-Cy ) 
// P = ( -Ey, Ex )
// h = ( (A-C) * P ) / ( F * P )

float	check_if_segment_cross(t_vertices v1, t_vertices v2, t_vertices v3, t_vertices v4){
	
	t_vertices	d1 = subtract_vectors(&v2, &v1);
	t_vertices	d2 = subtract_vectors(&v4, &v3);

	t_vertices E = vector_cross_product(&d1, &d2);
	t_vertices F = subtract_vectors(&d1, &d2);

	return (vector_dot_product(&E, &F));



	/*v1.x /= v1.z;
	v1.y /= v1.z;

	v2.x /= v2.z;
	v2.y /= v2.z;

	v3.x /= v3.z;
	v3.y /= v3.z;

	v4.x /= v4.z;
	v4.y /= v4.z;*/
	
	// t_vertices E = subtract_matrixed_vectors(&v2, &v1);
	// t_vertices F = subtract_matrixed_vectors(&v4, &v3);
	// t_vertices G = subtract_matrixed_vectors(&v3, &v1);
	/*
	t_vertices E = subtract_vectors(&v2, &v1);
	t_vertices F = subtract_vectors(&v4, &v3);
	t_vertices G = subtract_vectors(&v3, &v1);

	t_vertices P = E;
	P.y = -P.y;
	float	h = (vector_dot_product(&G, &P) / vector_dot_product(&F, &P));
	return (h);*/
}


// project to same plane
void	project_to_coplanarity(char **tab, size_t tab_size, t_scene *scene){
	t_vertices	*v1, *v2, *v3;
	size_t		index[tab_size - 1];
	for (size_t i = 1; i < tab_size; i++){
		index[i - 1] = atoi(tab[i]) - 1;
	}
	v1 = scene->vertices_tab[index[0]];
	v2 = scene->vertices_tab[index[1]];
	v3 = scene->vertices_tab[index[2]];

	v1->matrixed_x = v1->x;
	v1->matrixed_y = v1->y;
	v1->matrixed_z = v1->z;

	v2->matrixed_x = v2->x;
	v2->matrixed_y = v2->y;
	v2->matrixed_z = v2->z;

	v3->matrixed_x = v3->x;
	v3->matrixed_y = v3->y;
	v3->matrixed_z = v3->z;
	t_vertices	U = subtract_vectors(v2, v1), V = subtract_vectors(v3, v1); 

	t_vertices vn = vector_cross_product(&U, &V);
	normalize_vector(&vn);


	// project vectors onto the same plane
	for (size_t i = 3; i < tab_size - 1; i++){
		t_vertices tmp_vn = vn;
		t_vertices tmp = subtract_vectors(scene->vertices_tab[index[i]], v1);
		float scalar = vector_dot_product(&tmp, &tmp_vn);
		tmp_vn.x *= scalar;
		tmp_vn.y *= scalar;
		tmp_vn.z *= scalar;

		tmp = subtract_vectors(&tmp, &tmp_vn);
		scene->vertices_tab[index[i]]->matrixed_x = tmp.x;
		scene->vertices_tab[index[i]]->matrixed_y = tmp.y;
		scene->vertices_tab[index[i]]->matrixed_z = tmp.z;
	}
	

}


void	triangulate(char **tab, size_t tab_size, t_scene *scene, int line_nb){
	short	crossed[tab_size - 1][tab_size - 1];
	t_vertices	*v1, *v2, *v3, *v4;
	size_t		index[tab_size - 1];
	for (size_t i = 1; i < tab_size; i++){
		for (size_t j = 0; j < tab_size; j++){
			crossed[i - 1][j] = 0;
		}
		index[i - 1] = atoi(tab[i]) - 1;
	}

	//project_to_coplanarity(tab, tab_size, scene);

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
				if (k >= tab_size - 1)
					k = 0;
				if (k == l || k == i || k == j)
					continue;
				v4 = scene->vertices_tab[index[k]];

				

				float	value = check_if_segment_cross(*v1, *v2, *v3, *v4);
				printf("check %zu %zu cross %zu %zu (%f)\n", v1->id, v2->id, v3->id, v4->id, value);
				//float	value2 = check_if_segment_cross(*v3, *v4, *v1, *v2);
				if ((value > 0 && value < 1)){
					printf("\t %zu %zu cross %zu %zu (%f)\n", v1->id, v2->id, v3->id, v4->id, value);
					crossed[i][j] = 1;
					crossed[j][i] = 1;
				}
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
