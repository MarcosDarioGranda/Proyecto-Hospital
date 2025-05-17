#include <stdio.h>
#include <time.h>
#include "log.h"

void escribirLog(char *mensaje)
{
    FILE * fichLog = fopen("log.txt", "a"); //Fichero de registros log, en modo "append"

    time_t t;
    struct tm * tmInfo;
    char buffer[64];

    time(&t);
    tmInfo = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmInfo); //Hace un formatea la fecha y la hora en una cadena de caracteres.

    fprintf(fichLog, "[%s] %s\n", buffer, mensaje);
    fclose(fichLog);

    
    

}