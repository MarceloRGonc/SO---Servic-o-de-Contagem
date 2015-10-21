# ifndef LIST_DEFINITION
# define LIST_DEFINITION

# include "Commons.h"

typedef struct Element {
	void *data;
	struct Element *next;
} LinkedList;

LinkedList * newElem();
void * getData(LinkedList *elem);
LinkedList * insertElemTail(LinkedList *list, void *data);
LinkedList * insertElemHead(LinkedList *list, void *data);
LinkedList * nextElem(LinkedList *elem);
LinkedList * removeElem(LinkedList *list, void *data, equalFunction f);
int hasNext (LinkedList *list);
int existsElem (LinkedList *list, void *data, equalFunction f);
LinkedList * sortList (LinkedList *lst, equalFunction f, int desc);
int listSize (LinkedList *list);
LinkedList *mergeLists (LinkedList *head, LinkedList *tail);
void freeLinkedList (LinkedList * list);

# endif                                                                                                                                               
