# ifndef HASHMAP_CHAINING_DEF
# define HASHMAP_CHAINING_DEF

# define NADA 0

# include "LinkedList.h"
# include "Commons.h"

typedef struct {
        LinkedList **list;
        int size;
        hashFunction hash;
        equalFunction eq;
} ChainingHashMap;

typedef struct {
	void *key;
	void *data;
} HashMapElem;

HashMapElem * newHashMapElem(void *key, void *data);
void freeHashMapElem (HashMapElem *elem);
ChainingHashMap * newChainingHashMap (int size);
int putChaining(ChainingHashMap *map, void *key, void *data);
void * getChaining(ChainingHashMap *map, void *key);
int removeChaining(ChainingHashMap *map, void *key); 
LinkedList * keySetChaining (ChainingHashMap *map);
void freeChainingHashMap (ChainingHashMap *map);
int portugueseHash (void *key, int size);

# endif
