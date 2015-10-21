# include "Commons.h"


int errorAndExit(char *message) {
  perror(message);
  exit(1);
}

/* Lê a uma linha do fd */
char *nextLine (int fd) {
	char buffer[1];
	int s = 0;
	char *line = NULL;
	char *tmp;
	while (read(fd, buffer, 1) > 0) {
		if (buffer[0] == '\n' || buffer[0] == 0) {
            return line;
		} else {
			s++;
			tmp = line;
			line = (char *) malloc(sizeof(char) * (s+1));
			if (tmp != NULL) {
				strcpy(line, tmp);
				free(tmp);
			}
			line[s-1] = buffer[0];
			line[s] = 0;
		}
	}
	return line;
}

/* Escreve o *line no fd */
int writeLine (int fd, char *line) {
	char newLine = '\n';
	write(fd, line, strlen(line));
	write(fd, &newLine, 1);
	return 0;
}

/* Recebe na *line uma string do tipo A:B:C, delimiter é o limitador
 Devolve ABC */
char **explode (char *line, char *delimiter, int *size) {
	int j, i = 0;
	char **result = (char **) malloc(sizeof(char *));
	char **tmp;
	*size = 0;
	while ((result[i] = strsep(&line, delimiter))) 
	{
		(*size)++;
		tmp = (char **) malloc(sizeof(char *) * (i+2));
		for (j=0;j<=i;j++)
			tmp[j] = result[j];
		free(result);
		result = tmp;
		i++;
	}
	return result;
}

/* Recebe na *line uma string do tipo ABC, delimiter é o limitador 
 Devolve A:B:C se o delimiter for :*/
char *implode (char **pieces, char delimiter, int n) {
	int tmp, i, size = 0;
	for (i=0;i<n;i++) {
		size += strlen(pieces[i]) + 1;
	}
	char *result = (char *) malloc(size);
	result[0] = 0;
	for (i = 0;i<n;i++) {
		strcat(result, pieces[i]);
		if (i<n-1) {
			tmp = strlen(result);
			result[tmp] = delimiter;
			result[tmp+1] = 0; 
		}
	}
	return result;
}

/* Cria um novo processo filho */
int safeFork () {
	int pid = fork();
	if (pid == -1) {
		perror("Error forking");
		exit(1);
	}
	return pid;
}

/* Verifica se a *line começa pelo *prefixo */
int startsWith(char *line, char *prefix) { return strstr(line, prefix) == line; }

/* Cria a linha no formato 'i A:B:C!12' */
char *buildIncrement (char **pieces, int start, int end, int value) {
  char *command = "i";
  int size = strlen(command) + 1;
  int i;
  char number[100];
  for (i=start;i<=end;i++) {
    size += strlen(pieces[i]) + 1;
  }
  sprintf(number, "%d", value);
  size += strlen(number) + 1;

  char *line = (char *) malloc(size);
  line[0] = '\0';
  strcat(line, command);
  strcat(line, " ");
  for (i=start;i<=end;i++) {
    strcat(line, pieces[i]);
    if (i < end)
      strcat(line, ":");
    else
      strcat(line, "!");
  }
  strcat(line, number);

  return line;
}

/* Recebe na *line uma string do tipo ABC, e o total de caracteres
    Devolve linhas do tipo c A!A:B ou c A:B!A:B:C se o delimiter for :
 */
char *buildChild (char **pieces, int end) {
  char *command = "c ";
  char *sep = "!";
  int size = strlen(command) + 1;
  int i;

  char *fst = implode(pieces, ':', end-1);
  char *snd = implode(pieces, ':', end);
  size += strlen(fst) + 1;
  size += strlen(snd) + 1;

  char *result = (char *) malloc(size);
  result[0] = 0;
  strcat(result, command);
  strcat(result, fst);
  strcat(result, sep);
  strcat(result, snd);
  free(fst);
  free(snd);
  return result;
}