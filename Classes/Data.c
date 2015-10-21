# include "Data.h"

Counter * newCounter (char *name, int count) {
  Counter *element = (Counter *) malloc(sizeof(Counter));
  element->name = strdup(name); /* Informação do elemento*/
  element->counter = count; /* Valor resgistado */
  return element;
}

void freeCounter (Counter *data) {
  free(data->name);
  free(data);
}

Childs * newChilds (char *name) {
  Childs *element = (Childs *) malloc(sizeof(Childs));
  element->name = strdup(name); /* Informação do elemento*/
  element->childs = NULL; /* Informação dos descendentes*/
  return element;
}

void freeChilds (Childs *data) {
  if (data->childs)
    freeLinkedList(data->childs);
  free(data);
}


int compareKeys (void *fst, void *snd) {
  char *sn, *fs;
  fs = (char *) fst;
  sn = (char *) snd;
  return strcmp(fs, sn);
}
