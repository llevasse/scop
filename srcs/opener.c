#include "../include/scop.h"

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