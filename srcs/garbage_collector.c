#include <../include/scop.h>

extern struct s_garbage	*g_garbage_collector_root;
extern struct s_garbage	*g_garbage_collector;

int count = 0;

void add_to_garbage(void *addr){
	if (!addr)
		free_garbage();
	count++;
	if (g_garbage_collector->addr){
		g_garbage_collector->next = malloc(sizeof(struct s_garbage));
		if (!g_garbage_collector->next){
			free_garbage();
		}
		g_garbage_collector = g_garbage_collector->next;
	}
	g_garbage_collector->addr = addr;
	g_garbage_collector->next = 0x0;
}

void free_garbage(){
	t_garbage *tmp = g_garbage_collector_root;
	while (g_garbage_collector_root){
		free(g_garbage_collector_root->addr);
		tmp = g_garbage_collector_root;
		g_garbage_collector_root = g_garbage_collector_root->next;
		free(tmp);
	}
	if (errno)
		perror("Error");
	exit(errno);
}