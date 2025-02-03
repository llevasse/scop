#include "../include/scop.h"

void	parse_xpm_defines(char *s, int *width, int *height, size_t *nb_colours, size_t *char_per_pixel){
	*width = atoi(ft_strsep(&s, " \""));
	*height = atoi(ft_strsep(&s, " \""));
	*nb_colours = atoi(ft_strsep(&s, " \""));
	*char_per_pixel = atoi(ft_strsep(&s, " \""));
}

unsigned char	***alloc_xpm_map(size_t nb_colours, size_t char_per_pixel){
	unsigned char	***map = malloc(sizeof(char **) * (nb_colours + 1));
	if (!map)
		return (0x0);
	for (size_t i=0;i < nb_colours; i++){
		map[i] = malloc(sizeof(char *) * 4);
		if (map[i]){
			map[i][0] = malloc(sizeof(char) * (char_per_pixel + 1));
		}
		if (!map[i] || !map[i][0]){
			for (size_t j=0; j<i; j++){
				free(map[j][0]);
				free(map[j]);
			}
			if (map[i])
				free(map[i]);
			free(map);
			return (0x0);
		}
		ft_memset(map[i][0], 0, char_per_pixel + 1);
		for (int j=1;j<4;j++){
			map[i][j]= malloc(sizeof (unsigned char));
			if (!map[i][j]){
				for (size_t k=0; k<i; k++){
					free(map[k][0]);
					free(map[k][1]);
					free(map[k][2]);
					free(map[k][3]);
					free(map[k]);
				}
				for (int k=0; k<j; k++){
					free(map[i][k]);
				}			
				free(map[i]);
				free(map);
				return (0x0);
			}
		}
	}
	return (map);
}

unsigned char hex_to_byte(unsigned char byte){
	if (byte >= '0' && byte <= '9')
		byte = (byte - '0');
	else if (byte >= 'a' && byte <= 'f')
		byte = (byte - 'a' + 10);
	else if (byte >= 'A' && byte <= 'F')
		byte = (byte - 'A' + 10);
	return (byte);
}

short	is_hex(char c){
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

short	parse_map(char *s, unsigned char ***map, size_t mapping_idx, size_t char_per_pixel){
	short	warning=0;
	for (size_t i = 0; i< char_per_pixel; i++)
		map[mapping_idx][0][i] = s[i];
	s+= char_per_pixel;
	ft_strsep(&s, "#");
	if (!is_hex(*s) && !warning)
		warning = 1;
	*map[mapping_idx][1] = hex_to_byte(*s++) * 16;
	if (!is_hex(*s) && !warning)
		warning = 1;
	*map[mapping_idx][1] += hex_to_byte(*s++);

	if (!is_hex(*s) && !warning)
		warning = 1;
	*map[mapping_idx][2] = hex_to_byte(*s++) * 16;
	if (!is_hex(*s) && !warning)
		warning = 1;
	*map[mapping_idx][2] += hex_to_byte(*s++);

	if (!is_hex(*s) && !warning)
		warning = 1;
	*map[mapping_idx][3] = hex_to_byte(*s++) * 16;
	if (!is_hex(*s) && !warning)
		warning = 1;
	*map[mapping_idx][3] += hex_to_byte(*s++);
	return (warning);
}

unsigned char	*parse_xpm(char *path, int *width, int *height){
	int				fd = open_file(path, ".xpm");
	*width = -1;
	size_t			nb_colours = 0, char_per_pixel = 0, mapping_idx = 0, line_nb = 1;
	unsigned char	*data = 0x0, *data_ptr;
	unsigned char	***map = 0x0;
	char			*s, *tmp;
	short			begin_mapping = 0, finished_mapping = 0;
	if (fd < 0){
		return(0x0);
	}
	s = get_next_line(fd);
	while (s){
		if (*s == '"'){
			tmp = s + 1;
			if (*width < 0){
				begin_mapping = 1;
				parse_xpm_defines(tmp, width, height, &nb_colours, &char_per_pixel);
				data = malloc(sizeof(unsigned char) * ((*height * (*width * 3)) + 1));
				if (!data){
					close(fd);
					free(s);
					return(0x0);
				}
				map = alloc_xpm_map(nb_colours, char_per_pixel);
				if (!map){
					close(fd);
					free(s);
					free(data);
					return(0x0);
				}
				data_ptr = data;
			}
			else if (begin_mapping && !finished_mapping){
				if (parse_map(tmp, map, mapping_idx++, char_per_pixel)){
					dprintf(2, "Warning : invalid hex value on line %zu of file %s\n", line_nb, path);
				}
				//handle incomplete hex string
			}
			else if (finished_mapping){
				while (tmp && *tmp != '"'){
					for (mapping_idx = 0; mapping_idx < nb_colours; mapping_idx++){
						if (!ft_strncmp((const char *)map[mapping_idx][0], tmp, char_per_pixel))
							break;
					}
					if (mapping_idx == nb_colours){
						char	*subtr = ft_substr(tmp, 0, char_per_pixel);
						if (subtr)
							dprintf(2, "Error : Use of id '%s', not mapped to a rgb value on line %zu of file %s\n", subtr, line_nb, path);
						else
							dprintf(2, "Error : Use of id not mapped to a rgb value on line %zu of file %s\n", line_nb, path);
						for (size_t i = 0; i < nb_colours; i++){
							for (int j = 0; j < 4; j++){
								free(map[i][j]);
							}
							free(map[i]);
						}
						free(map);
						free(subtr);
						free(s);
						close(fd);
						free(data);
						return (0x0);
					}
					*data_ptr++ = *map[mapping_idx][1];
					*data_ptr++ = *map[mapping_idx][2];
					*data_ptr++ = *map[mapping_idx][3];
					tmp += char_per_pixel;
				}
			}
		}
		else if (begin_mapping){
			begin_mapping = 0;
			finished_mapping = 1;
			mapping_idx = 0;
		}
		free(s);
		s = get_next_line(fd);
		line_nb++;
	}

	for (size_t i = 0; i < nb_colours; i++){
		for (int j = 0; j < 4; j++){
			free(map[i][j]);
		}
		free(map[i]);
	}
	free(map);
	free(s);
	close(fd);
	return (data);
}