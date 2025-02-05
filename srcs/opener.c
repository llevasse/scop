#include "../include/scop.h"

int open_file(char *path, char *extension){
	char *trim_path = ft_strtrim(path, " \n\t\r");
	if (!trim_path)
		return (-1);
	if (ft_strcmp(trim_path + (ft_strlen(trim_path) - ft_strlen(extension)), extension)){
		dprintf(2, "Wrong file extension\n");
		free(trim_path);
		return (-1);
	}
	int fd = open(trim_path, O_RDONLY);
	free(trim_path);
	if (fd < 0){
		dprintf(2, "Error : could not open %s\n", trim_path);
	}
	return (fd);
}