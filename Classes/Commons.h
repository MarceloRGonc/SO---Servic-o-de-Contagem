# ifndef __COMMONS__
# define __COMMONS__

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <ctype.h>


typedef int (*equalFunction)(void *,void *);
typedef int (*hashFunction)(void *, int);

int errorAndExit (char *message);

int safeFork ();
char * nextLine (int fd);
int writeLine (int fd, char *line);
char ** explode (char *line, char *delimiter, int *size);
int startsWith(char *line, char *prefix);
char * implode (char **pieces, char delimiter, int n);

char * buildIncrement (char **pieces, int start, int end, int value);
char * buildChild (char **pieces, int end);

# endif
