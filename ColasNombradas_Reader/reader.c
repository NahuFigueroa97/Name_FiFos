#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define Name "MiFifoReceive"
#define SizeBuffer 20

int main(){

char buffer[SizeBuffer];
char log[SizeBuffer];
int32_t fd, code;
uint32_t bytes;
FILE *Log, *Signals;

if ( (code = mknod(Name, S_IFIFO | 0666, 0)) < -1){

printf("Error creando la cola nombrada %d\n", code);
exit(1);

}

printf("Esperando por un proceso escritor \n");

if ( (fd = open(Name, O_RDONLY)) < 0){

printf("Error abriendo la cola nombrada %d\n", fd);
exit(1);

}

printf("Tengo un proceso escritor \n");

Log = fopen("log.txt", "a");

Signals = fopen("signals.txt", "a");

if (Log == NULL && Signals == NULL){

    printf("No se pudo abrir el archivo");
    return 0;

}

do{

if ( (bytes = read(fd, buffer, SizeBuffer)) == -1) {

    perror("read:");

}else{

    buffer[SizeBuffer] = '\0';

}

if (!strncmp("DATA:", buffer, SizeBuffer)){

    fprintf(Log, "%s\n", buffer);
    printf("%s\n", buffer);

}

else{

    fprintf(Signals, "%s\n", buffer);
    printf("%s\n", buffer);

}

}while (bytes > 0);

    fclose(Log);
    fclose(Signals);
    return 0;

}