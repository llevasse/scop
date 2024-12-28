#include "../include/scop.h"

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

int open_file(char *path, char *extension){
	char *trim_path = ft_strtrim(path, " \n\t\r");	//TODO handle relative path
	if (!trim_path)
		return (-1);
	if (ft_strcmp(trim_path + (ft_strlen(trim_path) - ft_strlen(extension)), extension)){
		dprintf(2, "Wrong file extension\n");
		free(trim_path);
		return (-1);
	}
	int fd = open(trim_path, O_RDONLY);
	free(trim_path);
	return (fd);
}