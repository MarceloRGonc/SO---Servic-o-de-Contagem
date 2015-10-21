# include "Commons.h"
# include "Data.h"
# include "Counter.h"

# define MAP_SIZE 1001

int logFile = 0;

/* Quando recebe o sinal para kill para fechar o servidor fecha o ficheiro log */
void term(int signum)
{
    printf("Received SIGTERM, exiting...\n");
    if (logFile)
      close(logFile);
    exit(0);
}

/* Recebe a linha */
int process (char *input) {
  char **pieces = NULL;
  char *line;
  char *tmp, *inputCopy;
  int i, value = 0, size = 0;
  inputCopy = tmp = strdup(input);
  if (startsWith(inputCopy, "i")) {
    tmp = strsep(&inputCopy, " ");/* Tira o i da linha */
    pieces = explode(strsep(&inputCopy, "!"), ":", &size); /* Recebe os pedaços da linha */
    if (size > 1) {
      value = atoi(inputCopy); /* Transforma o valor em inteiro */
      for (i=0;i<size ;i++) {
        line = buildIncrement(pieces, 0, i, value);
        /* 
          * Vai enviar para o agregador sucessivamente
           * 1º A:12
           * 2º A:B:12
           * 3º A:B:C:12
         */
        writeLine(1, line);
        free(line);
      }
      for (i=0;i<size-1;i++) {
          line = buildChild(pieces, size - i);
          /* Recebe 
           1º linha do tipo c A:B!A:B:C
           2º linha do tipo c A!A:B
          */
          writeLine(1, line);
          free(line);
      }
    }
  } else if (startsWith(inputCopy, "a")) {
    writeLine(1, inputCopy);
  }
  free(tmp);
  return 0;
}

int server (char *namedPipe, int toAgregatorFd) {
  char *line;
  int pid = 0;
  int fd = open(namedPipe, O_RDONLY);
  signal(SIGINT, term);
  logFile = open("server.log", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR);
  while (1) {
    line = nextLine(fd); /* Recebe uma linha no formato 'i A:B:C!12' */
      /* Só avança quando tiver uma linha */
    if (line != NULL) {
      writeLine(logFile, line); /* Escreve no ficheiro log */
      pid = safeFork(); /* Cria um processo filho */
      if (!pid) {
          /* O processo filho é que fará o trabalho a partir daqui, o pai volta para ler a linha */
        dup2(toAgregatorFd, 1); /* Coloca o filedes[1] a apontar para o ecrã */
        return process(line);
      }
    }
  }
  return 0;
}

int start () {
  int filedes[2];
  pipe(filedes); /* Coloca na posição filedes[0] o descritor para reading e no filedes[1] o descritor para writing */
  int pid = safeFork();
  if (!pid) {
      /* Agregador */
    dup2(filedes[0], 0); /* Coloca o filedes[0] a apontar para a ler do teclado */
    return runCounter(); /* Corre o agregador */
  } else {
    mkfifo("named-pipe", 0777);/* Cria o named pipe para comunicação com a biblioteca */
    return server("named-pipe", filedes[1]);
  }
  return 0;
}

int main () {
  start();
}
