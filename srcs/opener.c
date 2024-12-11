#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

int open_obj(char *path){
	if (ft_strcmp(path + (ft_strlen(path) - 4), ".obj")){
		dprintf(2, "Wrong file extension\n");
		return (-1);
	}
	int fd = open(path, O_RDONLY);
	if (fd == -1){
		perror("Error");
		exit(errno);
	}
	return (fd);
}
/*
int open_mtl(char *path){

}*/