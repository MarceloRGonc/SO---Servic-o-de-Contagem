# include "Commons.h"
# include "Biblioteca.h"

char *tolower_line (char *string){
    int i = strlen(string);
    
    if (!string) return NULL;
    
    while (i>=0) {
        string[i] = tolower(string[i]);
        i--;
    }
    return string;
}

void reverseString (char *str) {
    int i, j, len;
    char temp;
    i=j=len=temp=0;
    
    len=strlen(str);
    for (i=0, j=len-1; i<=j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int error_inc (char *copia){
    int contador = 0;
    char * aux;
    char *line = strdup(copia);
    
    while (line){
        aux = strsep (&line, ":");
        contador++;
    }
    
    if (contador == 4 && line == NULL){
        return 1;}
    else
        return 0;
}

int error_agr (char *copia){
    int contador = 0;
    char *aux;
    char *line = strdup(copia);
    
    reverseString(line);
    
    aux = strsep(&line, " ");
    if (!line) return 0;

    aux = strsep(&line, " ");
    if (!line) return 0;

    reverseString(aux);
    contador = atoi(aux);
    
    if (contador < 0 || contador > 2) return 0;
    
    if (!line) return 0;
    
    reverseString(line);
    contador = 0;
    
    while (line){
        aux = strsep (&line, ":");
        contador++;
    }
    
    if (contador > 0 && contador < 4 && line == NULL)
        return 1;
    else
        return 0;
}

int chamarAgregar(char *copia){
    int valor;
    char *nomes[4];
    char *concelho = NULL, *freguesia = NULL, *distrito = NULL, *path = NULL, *inteiro = NULL;

    reverseString(copia);
    
    path = strsep(&copia, " ");
    
    inteiro = strsep(&copia, " ");
    
    reverseString(path);
    reverseString(copia);
    reverseString(inteiro);
    
    valor = atoi(inteiro);
    
    distrito = strsep(&copia, ":");
    
    if (copia) {
        concelho = strsep(&copia, ":");
    }
    if (copia) {
        freguesia = strsep(&copia, ":");
    }
    
    nomes[0] = strdup(tolower_line(distrito));
    
    if(concelho) nomes[1] = strdup(tolower_line(concelho));
    else nomes[1] = NULL;
    
    if(freguesia) nomes[2] = strdup(tolower_line(freguesia));
    else nomes[2] = NULL;
    nomes[3] = NULL;
    
    agregar(nomes, valor, path);
    
    return 0;
}

int chamarIncrementar(char *copia){
    int valor;
    char *nomes[4];
    char *concelho = NULL, *freguesia = NULL, *distrito = NULL;

    distrito = strsep(&copia, ":");
    concelho = strsep(&copia, ":");
    freguesia = strsep(&copia, ":");
    
    valor = atoi(copia);
    
    nomes[0] = strdup(tolower_line(distrito));
    nomes[1] = strdup(tolower_line(concelho));
    nomes[2] = strdup(tolower_line(freguesia));
    nomes[3] = NULL;
    
    incrementar(nomes, valor);
    
    return 0;
}

int main(){

    char *line, *comando, *copia;
    int ficheiroT;

    while((line = nextLine(0))){
        
        copia = strdup(line);
        
        comando = strsep(&copia, " ");

        if (!copia && (strcmp(comando, "RunTest") != 0)) {
            printf("Syntax Error\n");
            free(comando);
        }
        else if  (strcmp(comando, "incrementar") == 0) {
            if (!error_inc(copia)){
                printf("Syntax Error\n");
            }
            else{
                chamarIncrementar(copia);
                free(comando);
            }
        }
        else if(strcmp(comando, "agregar") == 0) {
            if (!error_agr(copia)){
                printf("Syntax Error\n");
            }
            else{
                chamarAgregar(copia);
                free(comando);
            }
        }
        else if(strcmp(comando, "RunTest") == 0) {
            ficheiroT = open("test.txt", O_RDWR | O_APPEND);
            
            while((line = nextLine(ficheiroT)) != NULL){
                copia = strdup(line);
                comando = strsep(&copia, " ");
                if  (strcmp(comando, "incrementar") == 0)                 
                    chamarIncrementar(copia);
                if(strcmp(comando, "agregar") == 0)
                    chamarAgregar(copia);
                free(line);
                line=NULL;
            }
        }
        else printf("Syntax Error\n");
    }
}
