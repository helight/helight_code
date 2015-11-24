#ifndef __XHASH__
#define __XHASH__
#include <stdlib.h>
#include <stdint.h>

//hash node
struct hnode
{
    void *key;
    void *value;
    struct hnode *next;
};

struct hash_t *create_hasht(unsigned int tsize, unsigned int ksize,
                            uint32_t (*hash_fun)(unsigned char * key, unsigned long len),
                            int isnode(const void *key1, const void *key2));
int hash_insert(struct hash_t *htable, struct hnode *node);
void *hash_get(struct hash_t *htable, void *key);
void *hash_del(struct hash_t *htable, void *key);
int hash_free(struct hash_t *htable);

#endif
