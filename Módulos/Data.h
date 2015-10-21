# ifndef __MAP__
# define __MAP__

# include "LinkedList.h"
# include "Map.h"
# include "Commons.h"


typedef struct CounterElement {
  char *name;
  int counter;
} Counter;


typedef struct ChildsElement {
  char *name;
  LinkedList *childs;
} Childs;

Counter * newCounter (char *name, int count);
void freeCounter (Counter *data);


Childs * newChilds (char *name);
void freeChilds (Childs *data);

int compareKeys (void *fst, void *snd);

# endif
