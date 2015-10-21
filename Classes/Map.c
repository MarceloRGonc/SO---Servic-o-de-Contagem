# include "Map.h"

HashMapElem * newHashMapElem (void *key, void *data) {
        HashMapElem *new = (HashMapElem *) malloc(sizeof(HashMapElem));
        if (!new) errorAndExit("Memria Insuficiente");
        new->key = key;
        new->data = data;
        return new;
}

void freeHashMapElem (HashMapElem *elem) {
        free(elem);
}

ChainingHashMap * newChainingHashMap (int size) {
  ChainingHashMap *new = (ChainingHashMap *) malloc(sizeof(ChainingHashMap));
  if (!new) errorAndExit("Memria Insuficiente - HashMap");
  new->list = (LinkedList **) malloc(sizeof(LinkedList *)*(size+1));
  if (!new->list) errorAndExit("Memria Insuficiente - List");
  int i;
  for (i=0;i<size+1;i++) new->list[i] = NULL;
  new->size = size;
  new->hash = NULL;
  new->eq = NULL;
  return new;
}

int putChaining (ChainingHashMap *map, void *key, void *data) {
        int k = (map->hash)(key,map->size);
        int found = 0;
        LinkedList *aux;
        LinkedList *temp;
        HashMapElem *tmp,*new = newHashMapElem(key,data);
        if (!map||k >= map->size || k < 0) return 0;
        if (map->list[k] == NULL) {
                temp = insertElemHead(map->list[k],new);
                if (temp == NULL) return 0;
                else map->list[k] = temp;
                return 1;
        }
        else {
                aux = map->list[k];
                while (!found&&aux) {
                        tmp = (HashMapElem *) getData(aux);
                        if (!(map->eq)(tmp->key,key)) found = 1;
                        else aux = nextElem(aux);
                }
                if (!found) {
                        map->list[k] = insertElemHead(map->list[k],new);
                }
                else aux->data = new;
                return 1;
        }
        return 0;
}

void *getChaining(ChainingHashMap *map, void *key) {
        int k = (map->hash)(key,map->size);
        if (!map||k >= map->size || k < 0) return NULL;
        LinkedList *aux = map->list[k];
        HashMapElem *tmp;
        int found = 0;
        while (aux && !found) {
                tmp = getData(aux);
                if (!(map->eq)(tmp->key,key)) found = 1;
                else aux = nextElem(aux);
        }
        if (aux) return tmp->data;
        else return NULL;
}

int removeChaining(ChainingHashMap *map, void *key) {
        int k = (map->hash)(key,map->size);
        if (!map||k >= map->size || k < 0) return 0;
        int found = 0;
        LinkedList *aux,*parent=NULL;
        HashMapElem *tmp;
        if (k >= map->size) return 0;
        aux = map->list[k];
        while (!found&&aux) {
                tmp = (HashMapElem *) getData(aux);
                if (!(map->eq)(tmp->key,key)) found = 1;
                else {
                        parent = aux;
                        aux = nextElem(aux);
                }
        }
        if (found) {
                if (!parent) map->list[k] = NULL;
                else parent->next = aux->next;
                free(aux);
                return 1;
        }
        return 0;
}

LinkedList * keySetChaining (ChainingHashMap *map) {
  int i;
  LinkedList *result = NULL;
  LinkedList *aux = NULL;
  HashMapElem *tmp;
  for (i=0;i<map->size;i++) {
    aux = map->list[i];
    while (aux) {
      if (aux->data) {
        tmp = aux->data;
        result = insertElemHead(result,tmp->key);
      }
      aux = aux->next;
    }
  }
  return result;
}

void freeChainingHashMap (ChainingHashMap *map) {
  free(map->list);
  free(map);
}

static float probChar(char caracter);
static char cleanChar(char caracter);

static float probChar (char caracter) {
        switch (caracter) {
                case 'a': return 0.143;
                case 'b': return 0.009;
                case 'c': return 0.041;
                case 'd': return 0.049;
                case 'e': return 0.128;
                case 'f': return 0.012;
                case 'g': return 0.013;
                case 'h': return 0.011;
                case 'i': return 0.062;
                case 'j': return 0.008;
                case 'l': return 0.031;
                case 'm': return 0.043;
                case 'n': return 0.044;
                case 'o': return 0.111;
                case 'p': return 0.023;
                case 'q': return 0.011;
                case 'r': return 0.062;
                case 's': return 0.071;
                case 't': return 0.051;
                case 'u': return 0.052;
                case 'v': return 0.019;
                case 'x': return 0.002;
                case 'z': return 0.004;
        }
        return 0;
}

static char cleanChar (char caracter) {
        char c;
        if (!isalpha(caracter)) return NADA;
        else {
                if (isupper(caracter)) c = tolower(caracter);
                else c = caracter;
                if (c != 'k' && c != 'w' && c != 'y') return c;
                else return NADA;
        }
}

int portugueseHash (void *key, int size) {
        char *chave = (char *) key;
        int i,n = strlen(chave);
        float res = 0;
        char tmp;
        for (i=0;i<n;i++) {
                tmp = cleanChar(chave[i]);
                if (tmp != NADA) {
                        res += 1/probChar(tmp);
                }
        }
        return (int) res % size;
}
