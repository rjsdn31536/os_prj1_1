#include <stdio.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>

typedef struct bm_array
{
	char name[5];
	struct bitmap *array;
}bm_array;
bm_array bm[10];

struct bitmap *bitmap_expand( struct bitmap *bm,int size);

struct list_item{
	struct list_elem elem;
	int bar;
};

typedef struct list_array
{
	char name[8];
	struct list *link;
}list_array;
list_array la[10];

struct list *create_list();
struct list_item *create_list_item();
bool less(const struct list_elem* a, const struct list_elem *b, void *aux);
void list_swap(struct list_elem *a,struct list_elem *b);
void list_shuffle(struct list *list);

struct hash_item{
	struct hash_elem elem;
	int data;
};

typedef struct hash_array
{
	char name[10];
	struct hash_iterator *link;
}hash_array;
hash_array ha[10];

struct hash_iterator *create_hash_iterator();
struct hash *create_hash();
struct hash_item *create_hash_item();
void square(struct hash_elem *a,void *aux);
void triple(struct hash_elem *a, void *aux);
unsigned hf(const struct hash_elem *e, void *aux);
bool hl(const struct hash_elem *a,const struct hash_elem *b, void *aux);
void hash_destructor(struct hash_elem *a,void *aux);
unsigned hash_int_2(int i);
