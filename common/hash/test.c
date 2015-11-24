#include <stdio.h>
#include <string.h>

#include "hash.h"

int cmp(const void *p1, const void *p2)
{
	return strcmp((const char * ) p1, (const char * ) p2);
}

char* key[]={"name","address","phone","door","city","sex","major"};
char* value[]={"netboy","china","263788","110#","guangzhou","male","information"};
char *tchar[]={"helightxu"};

void print_htable(struct hash_t *htable)
{
	int i = 0;
	struct hnode *tmp;

	for(i = 0; i< 7; i++) {
		tmp = (struct hnode *)hash_get(htable, key[i]);
		if(tmp != NULL)
			printf("%s ---> %s\n", (char *)tmp->key, (char *)tmp->value);
		else
			printf("xxxxxxxxxxxxxxxxxxxxxxxhello world\n");
	}

	return;
}


int main(int argc, char *argv[])
{
	int i = 0;
	struct hnode *tmp;

	struct hash_t *htable = create_hasht(5, 7, NULL, cmp);

	for(i = 0; i < 7; i++) {
		tmp = malloc(sizeof(struct hnode));
		tmp->key = key[i];
		tmp->value = value[i];
		tmp->next = NULL;
		if(hash_insert(htable, tmp) == 0){
			printf("tmp->key:%s  tmp->value:%s \n", (char *)tmp->key, (char *)tmp->value);
		}
	}
	print_htable(htable);

	tmp = (struct hnode *)hash_del(htable, key[2]);
	if (tmp != NULL)
		free(tmp);
	print_htable(htable);
	tmp = (struct hnode *)hash_get(htable, key[3]);
	if (tmp != NULL)
		tmp->value = tchar[0];
	print_htable(htable);

	hash_free(htable);

	printf("hello world\n");
	return 0;
}
