#include "../include/scop.h"

int main(int argc, char **argv){
	if (argc != 2){
		dprintf(2, "wrong number of arguments\n");
		exit (1);
	}
	open_obj(argv[1]);
	(void)argv;
}
