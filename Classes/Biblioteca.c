# include "Biblioteca.h"

static int fd = 0;

/* Envia a linha no formato 'i A:B:C!12' */
int incrementar(char *nome[], unsigned valor) {
	int i;
	if (!fd) fd = open("named-pipe", O_WRONLY);

	for (i=0;nome[i];i++);

	char *command = buildIncrement(nome, 0, i-1, valor);
	writeLine(fd, command);
	free(command);
	return 0;
}

/* Envia a linha no formato 'a A:B:C!nivel!path' */
char * buildAgrega (char **nomes, int end, int nivel, char *path) {
	int size = strlen("a ");
	char level[100];
	sprintf(level, "%d", nivel);
	char *nome = implode(nomes, ':', end);
	size += strlen(path)+1;
	size += strlen(level)+1;
	char *result = (char *) malloc(size);
	result[0] = 0;
	strcat(result, "a ");
	strcat(result, nome);
	strcat(result, "!");
	strcat(result, level);
	strcat(result, "!");
	strcat(result, path);

	free(nome);
	return result;
}

/* Envia a linha no formato 'a A:B:C!12' */
int agregar(char *prefixo[], unsigned nivel, char *path) {
	int i;
	if (!fd) fd = open("named-pipe", O_WRONLY);
	for (i=0;prefixo[i];i++);

	char *send = buildAgrega(prefixo, i, nivel, path);
	writeLine(fd, send);
	free(send);
	return 0;
}