# include "LinkedList.h"

LinkedList * newElem () {
  LinkedList *new = (LinkedList *) malloc(sizeof(LinkedList));
  if (!new) errorAndExit("Memria Insuficiente");
  new->data = NULL;
  new->next = NULL;
  return new;
}
void * getData (LinkedList *elem) {
  if (!elem) return NULL;
  return elem->data;
}
int hasNext (LinkedList *list) {
  if (!list) return 0;
  return (list->next != NULL);
}
LinkedList * insertElemTail(LinkedList *list, void *data) {
  LinkedList *elem = newElem();
  elem->data = data;
  LinkedList *aux;
  if (!list) return elem;
  else {
    aux = list;
    while (aux->next) aux = aux->next;
    aux->next = elem;
  }
  return list;
}
LinkedList * insertElemHead(LinkedList *list, void *data) {
  LinkedList *elem = newElem();
  elem->data = data;
  elem->next = list;
  return elem;
}

LinkedList * nextElem (LinkedList *elem) {
  if (!elem) return NULL;
  return elem->next;
}
LinkedList * removeElem (LinkedList *list, void *data, equalFunction equalityFunc) {
  LinkedList *aux = list;
  LinkedList *behind = NULL;
  if (!list) return NULL;
  while (aux&&!((equalityFunc)(data,aux->data))) {
    behind = aux;
    aux = aux->next;
  }
  if (aux) {
    if (behind) {
      behind->next = aux->next;
    }
    else return aux->next;
  }
  return list;
}
int existsElem (LinkedList *list, void *data, equalFunction eq) {
  LinkedList *aux = list;
  int result = 0;
  while (aux && (eq)(getData(aux),data))
    aux = nextElem(aux);
  if (aux) result = 1;
  return result;
}

int listSize (LinkedList *list) {
        int n = 0;
        LinkedList *aux = list;
        while (aux) {
                aux = aux->next;
                n++;
        }
        return n;
}

LinkedList * mergeLists (LinkedList *head, LinkedList *tail) {
  LinkedList *aux = head;
  if (!head) return tail;
  while (aux->next) aux = aux->next;
  aux->next = tail;
  return head;
}

void freeLinkedList (LinkedList *list) {
  LinkedList *aux;
  while (list) {
    aux = list;
    list = list->next;
    free(aux);
  }
}

static LinkedList * insertAfter (LinkedList *lst, void *data) {
  if (!lst) return insertElemHead(lst,data);
  LinkedList *aux = lst->next;
  lst->next = newElem();
  lst->next->next = aux;
  lst->next->data = data;
  return lst;
}

LinkedList * sortList (LinkedList *lst, equalFunction f, int desc) {
  LinkedList *res = NULL;
  LinkedList *tmp,*grd = lst;
  LinkedList *aux1,*aux2,*aux = grd;
  if (!aux) return NULL;
  res = insertElemHead(res,getData(aux));
  grd = aux;
  aux = aux->next;
  int insert;
  int cmp = (desc) ? 1 : -1;
  free(grd);
  while (aux) {
    tmp = res; aux1 = NULL; insert = 0;
    while (tmp&&!insert) {
      if ((f)(getData(aux),getData(tmp)) == cmp) {
        if (!aux1) {
          res = insertElemHead(res,getData(aux));
        }
        else {
          aux1 = insertAfter(aux1,getData(aux));
        }
        insert = 1;
      }
      aux1 = tmp;
      tmp = tmp->next;
    }
    if (!insert) {
      aux1 = insertElemTail(aux1,getData(aux));
    }
    grd = aux;
    aux = aux->next;
    free(grd);
  }
  return res;
}
