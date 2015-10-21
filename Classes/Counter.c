# include "Counter.h"
# include "Map.h"
# include "Commons.h"
# include "LinkedList.h"
# include "Data.h"

# define MAP_SIZE 1001

static int agrega (ChainingHashMap *counters, ChainingHashMap *childs, char *name, int level, int fd, LinkedList *added) {

  Counter *counter;
  Childs *childInfo;
  char buffer[1000];
  char *childName;
  LinkedList *directChilds;

  if (level == 0 ) {
      counter = (Counter *) getChaining(counters, name);
      if (counter != NULL) {
        if (!existsElem(added, name, compareKeys)) {
          added = insertElemHead(added, name);
          sprintf(buffer, "%s:%d", name, counter->counter);
          writeLine(fd, buffer);
        }
      }
  } else {
      Childs *childsInfo = (Childs *)getChaining(childs, name);
      if (childsInfo) {
        directChilds = childsInfo->childs;
        while (directChilds) {
          agrega(counters, childs, getData(directChilds), level - 1, fd, added);
          directChilds = nextElem(directChilds);
        }
      }  
  }
  return 0;
}

/* Agregador */
int runCounter () {
  char *tmp, *line = NULL;
  char *name, *parent, *child;
  int increment, nivel, file;
  Counter *counter;
  Childs *childInfo;

  ChainingHashMap *counters = newChainingHashMap(MAP_SIZE); /* Cria a tabela de hash e counters */
  counters->hash = portugueseHash;
  counters->eq = compareKeys;

  ChainingHashMap *childs = newChainingHashMap(MAP_SIZE); /* Cria a tabela de hash childs */
  childs->hash = portugueseHash;
  childs->eq = compareKeys;

  while(1) {
    tmp = line = nextLine(0); /* Lê a linha vinda do servidor */
    if (line != NULL) {
      if (startsWith(line, "i")) {
        /* Recebe i A:12 despois i A:B:12 ... ;
          * Insere nas hash na counter A:12 depois A:B:12 ...
          * Na childs A depois A:B ...
         */
        strsep(&line, " ");
        name = strdup(strsep(&line, "!"));
        increment = atoi(line);        
        counter = (Counter *) getChaining(counters, (void *) name); /* Vai Buscar o elemento á hash */
        if (counter == NULL) {
          putChaining(counters, strdup(name), newCounter(name, increment)); /* Coloca um novo elemento na hash */
          putChaining(childs, strdup(name), newChilds(name));
        }
        else {
          counter->counter += increment;
        }
      } else if (startsWith(line, "c")) {
          /* Recebe c A:B!A:B:C despois c A!A:B;
           * Coloca a informação dos descendentes na hash
           */
        strsep(&line, " ");
        parent = strsep(&line, "!");
        childInfo = (Childs *) getChaining(childs, parent);
        if (childInfo == NULL) {
          perror("Unable to find parent");
        } else {
          childInfo->childs = insertElemHead(childInfo->childs, strdup(line));
            /* Coloca a informação de um descendente, ou seja, para um distrito coloca a informação de um concelho e freguesia */
        }
      }
      else if (startsWith(line, "a")) {
          /* Recebe a A:B:C!nivel!path;
           * Abre ou cria o ficheiro, e coloca lá a informação
           */
        strsep(&line, " ");
        name = strsep(&line, "!");
        nivel = atoi(strsep(&line, "!"));
        file = open(line, O_CREAT | O_WRONLY, S_IRUSR);
        agrega(counters, childs, name, nivel, file, NULL);
        close(file);
      }
      free(tmp);
    }
  }
}
