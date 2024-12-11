#include "../include/scop.h"

int main(int argc, char **argv){
	if (argc != 2){
		dprintf(2, "wrong number of arguments\n");
		exit (1);
	}
	int fd = open_obj(argv[1]);
	if (fd == -1)
		exit(1);
	
	(void)argv;
}
