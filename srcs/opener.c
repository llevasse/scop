#include "../include/scop.h"

int open_obj(char *path){
	int fd = open(path, O_RDONLY);
	if (fd == -1){
		perror("Error");
		exit(errno);
	}
	return (fd);
}