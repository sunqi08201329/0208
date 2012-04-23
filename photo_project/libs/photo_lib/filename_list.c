#include "filename_list.h"
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

file_list *make_a_node(char * filename, int index)
{
	file_list *new;

	new = malloc(sizeof(file_list));	

	if(new == NULL){
		fprintf(stderr, "malloc memory fialed!\n");
		exit(1);
	}

	strcpy(new->filename,filename);

	new->next = NULL;
	new->index = index;

	return new;
}

file_list *delete_a_node(file_list *pre, file_list *cur)
{
	file_list *post;

	post = cur->next;
	free(cur);
	pre->next = post;
	cur = post;

	return cur;
}

void free_list(file_list * list)
{
	file_list *move;
	move = list;

	while(move != NULL)
	{
		move = move->next;
		free(list);
		list = move;
	}

	list = NULL;
}
void print_list(file_list * list)
{
	file_list *move;
	move = list;

	while(move != NULL)
	{
		fprintf(stdout, "filename%d: %s\n", move->index, move->filename);
		move = move->next;
	}
}

file_list *read_file_name(char * path, file_list *list)
{
	DIR * dirp;

	struct dirent *dir_entry;

	int index = 1;
	file_list *move, *new;
	new = move = list;

	char whole_name[FILE_NAME_LEN];

	strcpy(whole_name, path);

	if((dirp = opendir(path)) == NULL)	
	{
		fprintf(stderr, "open directory %s error %s\n", path, strerror(errno));
		exit(1);
	}

	while((dir_entry = readdir(dirp)) != NULL)
	{
		if(!strcmp(dir_entry->d_name, "."))
			continue;
		else if(!strcmp(dir_entry->d_name, ".."))
			continue;
		else {
			strcat(whole_name, dir_entry->d_name);	
			whole_name[strlen(whole_name)] = '\0';
			//printf("sunqi9999999999999999999999999999999      %d\n", strlen(whole_name));
			
			if(list == NULL)
			{
				list = make_a_node(whole_name, index);
				new = move = list;
			} else {
				new = make_a_node(whole_name, index);		
				move->next = new;
				move = new;
			}
			index++;
			strcpy(whole_name, path);
		}
	}

	return list;
}

file_list *to_circle(file_list * list)
{
	file_list *move;
	move = list;

	while(move->next != NULL)
	{
		move = move->next;
	}

	move->next = list;

	return list;
}

file_list *off_circle(file_list * list)
{
	file_list *next;

	next = list->next;
	list->next = NULL;

	return next;
}

char **struct_a_array(char **file_names)
{
	int i;
	
	file_names = (char **)malloc(sizeof(char *) * FILE_NUM);		

	for (i = 0; i < FILE_NUM; i++) 
	{
		file_names[i] = (char *)malloc(sizeof(char) * FILE_NAME_LEN);
	}
	return file_names;
}

void destruct_array(char **array)
{
	int i;
	for (i = 0; i < FILE_NUM; i++) 
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
}

int jose_sort(char **file_names, file_list *list)
{
	file_list *cur, *pre;
	pre = list = to_circle(list);
	cur = pre->next;

	int key = time(NULL) % 9 + 1;

	int i = 0;
	int cnt = 1;

	while(cur->index != cur->next->index)
	{
		printf("key = %d\n", key);
		if(cnt % key == 0)
		{
			strcpy(file_names[i], cur->filename);
			cur = delete_a_node(pre, cur);
			i++;
			cnt = 1;
		} else {
			pre = cur;
			cur = cur->next;

			cnt++;
		}
	}
	strcpy(file_names[i], cur->filename);
	free(cur);
	cur = NULL;

	return i + 1;

}
