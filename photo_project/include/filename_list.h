#ifndef __FILENAME_LSIT__H__
#define   __FILENAME_LSIT__H__

#define FILE_NAME_LEN 32
#define FILE_NUM 50 

typedef struct node{
	int index;
	char filename[FILE_NAME_LEN];
	struct node *next;
}file_list;


void init_list(file_list *list);

file_list *make_a_node(char * filename, int index);

file_list *delete_a_node(file_list *pre, file_list *cur);

void print_list(file_list * list);

int max_index(file_list *list);

file_list *get_start_pre(file_list *list);

file_list *to_circle(file_list * list);

file_list *off_circle(file_list * list);

file_list *read_file_name(char * path, file_list *list);

void free_list(file_list * list);

void destruct_array(char **array);

char **struct_a_array(char **file_names);

int jose_sort(char **file_names, file_list *list);

#endif // __FILENAME_LSIT__H__
