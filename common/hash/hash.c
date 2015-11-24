/****************************************************
                  hash table
-------------------------------------------------
| idx | idx | idx | idx | idx | idx | idx | idx |
-------------------------------------------------
   |                 |                 |
 ------           ------             ------
 |node|           |node|             |node|
 ------           ------             ------
   |
 ------
 |node|
 ------


***************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#include "hash.h"

uint32_t default_hash_fun(unsigned char *key, unsigned long length);
uint32_t oneatatime_hash(unsigned char *key, unsigned long len);
int isame(const void *p1, const void *p2);

//the head of hash table
struct hash_t
{
	unsigned int tsize;	//length of the hash table
	unsigned int nsize;	//number of the node in this hash table
	unsigned int ksize;	//length of the hash key
	uint32_t  (*hash_idx)(unsigned char *, unsigned long );
				//hash fun to get the index
	int (*isnode)(const void *, const void *);
				//check the node if there is already in
	unsigned int (*index)(const void *);
				//get the index of table from key
	struct hash_ops *ops;	//operations of hash table
	struct hnode **ntable;	//hash table
};

//operations of hash table
struct hash_ops
{
	unsigned int (*index)(struct hash_t *, void *);
	int (*insert)(struct hash_t *, struct hnode *);
	void *(*get)(struct hash_t *, void *);
	void *(*del)(struct hash_t *, void *);
};

static int h_insert(struct hash_t *htable, struct hnode *node);
static void *h_get(struct hash_t *htable, void *key);
static void *h_del(struct hash_t *, void *key);

struct hash_ops h_ops = {
	.insert	= h_insert,
	.get	= h_get,
	.del	= h_del
};

static int h_insert(struct hash_t *htable, struct hnode *node)
{
	unsigned int idx;
	struct hnode *cursor;
	/* Hash the key and get the index of  table */
	idx = (htable->hash_idx(node->key, htable->ksize)) & (htable->tsize-1);

	//no collision
	if (htable->ntable[idx] == NULL)
		htable->ntable[idx] = node;
	//collision happends insert at the end of this chain
	else {
		cursor = htable->ntable[idx];
		while (1)
        {
			if (htable->isnode(cursor->key, node->key) == 0)
            {
				errno = EINVAL; return -1;
			}
			if (cursor->next == NULL)
				break;
			cursor = cursor->next;
		}
		cursor->next = node;
	}
	htable->nsize++;
	return 0;
}

static void *h_get(struct hash_t *htable, void *key)
{
	unsigned idx;
	struct hnode *cursor;

	idx = (htable->hash_idx(key, htable->ksize)) & (htable->tsize-1);
	cursor = htable->ntable[idx];
	while (cursor != NULL)
    {
		if(htable->isnode(cursor->key, key) == 0)
			return cursor;
		cursor = cursor->next;
	}

	errno = EFAULT;
	return NULL;
}

static void *h_del(struct hash_t *htable, void *key)
{
	unsigned idx;
	struct hnode *cursor, *tmp;

	idx = (htable->hash_idx(key, htable->ksize)) & (htable->tsize-1);
	cursor = htable->ntable[idx];
	if (cursor != NULL)
    {
		if (htable->isnode(cursor->key, key) == 0)
        {
			htable->ntable[idx] = cursor->next;
			htable->nsize--;
			return cursor;
		}
        else
        {
			while (cursor->next != NULL)
            {
				if (htable->isnode(cursor->next->key, key) == 0)
                {
					tmp = cursor->next;
					cursor->next = tmp->next;
					htable->nsize--;
					return tmp;
				}
				cursor = cursor->next;
			}
		}
	}

	errno = EFAULT;
	return NULL;
}

struct hash_t *create_hasht(unsigned int tsize, unsigned int ksize,
		uint32_t (*hash_fun)(unsigned char * key, unsigned long len),
		int isnode(const void *key1, const void *key2))
{
	struct hash_t *htable;

	if (isnode == NULL)
    {
		errno = EINVAL;
		return NULL;
	}
	/* Create hash table */
	htable = malloc(sizeof(struct hash_t));
	if (!htable)
    {
		errno = ENOMEM;
		return NULL;
	}

	htable->ops = &h_ops;
	htable->ntable=malloc(tsize*sizeof(struct hnode *));
	if (!htable->ntable)
    {
		free(htable);
		errno = ENOMEM;
		return NULL;
	}

	memset(htable->ntable, '\0', tsize*sizeof(struct hnode *));
	htable->tsize = tsize;
	htable->ksize = ksize;
	htable->nsize = 0;

	if (hash_fun == NULL)
		htable->hash_idx = default_hash_fun;
	else
		htable->hash_idx = hash_fun;

	if (isnode == NULL)
		htable->isnode =isame;
	else
		htable->isnode = isnode;

	return htable;
}

int hash_insert(struct hash_t *htable, struct hnode *node)
{
	assert(htable);
	assert(node);

	return htable->ops->insert(htable, node);
}

void *hash_get(struct hash_t *htable, void *key)
{
	assert(htable);
	assert(key);

	return htable->ops->get(htable, key);
}

void *hash_del(struct hash_t *htable, void *key)
{
	assert(htable);
	assert(key);

	return htable->ops->del(htable, key);
}

int hash_free(struct hash_t *htable)
{
	unsigned int idx;
	struct hnode *node;

	for (idx = 0; idx < htable->tsize; idx++)
    {
		node = htable->ntable[idx];
		while (node != NULL)
        {
			struct hnode *tmp = node;
			node = tmp->next;
			free(tmp);
		}
	}
	free(htable->ntable);
	free(htable);

	return 0;
}

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c >> 13); \
  b -= c; b -= a; b ^= (a << 8); \
  c -= a; c -= b; c ^= (b >> 13); \
  a -= b; a -= c; a ^= (c >> 12);  \
  b -= c; b -= a; b ^= (a << 16); \
  c -= a; c -= b; c ^= (b >> 5); \
  a -= b; a -= c; a ^= (c >> 3);  \
  b -= c; b -= a; b ^= (a << 10); \
  c -= a; c -= b; c ^= (b >> 15); \
}

//this hash function has been written by Bob Jenkins
//See http://burtleburtle.net/bob/hash/doobs.html
uint32_t default_hash_fun(unsigned char *k, unsigned long length)
{
	unsigned long a,b,c,len;

	/* Set up the internal state */
	len = length;
	a = b = c = 0x9e3779b9;  /* the golden ratio; an arbitrary value */

	/* Handle most of the key */
	while (len >= 12)
    {
		a += (k[0] + ((unsigned long)k[1] << 8) + ((unsigned long)k[2] << 16)
              + ((unsigned long)k[3] << 24));
		b += (k[4] + ((unsigned long)k[5] << 8) + ((unsigned long)k[6] << 16)
              + ((unsigned long)k[7] << 24));
		c += (k[8] + ((unsigned long)k[9] << 8) + ((unsigned long)k[10]<< 16)
              + ((unsigned long)k[11] << 24));
		mix(a,b,c);
		k += 12; len -= 12;
	}

	/* Handle the last 11 bytes */
	c += length;
	switch(len)
    {              /* all the case statements fall through */
		case 11: c += ((unsigned long)k[10] << 24);
		case 10: c += ((unsigned long)k[9]  << 16);
		case 9 : c += ((unsigned long)k[8]  << 8);
			 /* the first byte of c is reserved for the length */
		case 8 : b += ((unsigned long)k[7] << 24);
		case 7 : b += ((unsigned long)k[6] << 16);
		case 6 : b += ((unsigned long)k[5] << 8);
		case 5 : b += k[4];
		case 4 : a += ((unsigned long)k[3] << 24);
		case 3 : a += ((unsigned long)k[2] << 16);
		case 2 : a += ((unsigned long)k[1] << 8);
		case 1 : a += k[0];
	}
	mix(a,b,c);

	return c;
}

/* One-at-a-time hash */
uint32_t oneatatime_hash(unsigned char *key, unsigned long len)
{
	uint32_t hash=0;
	size_t i;

	for (i=0; i <len; ++i)
    {
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

int isame(const void *p1, const void *p2)
{
	return strcmp((const char * ) p1, (const char * ) p2);
}

