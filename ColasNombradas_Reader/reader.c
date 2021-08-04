#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define Name "MiFifoReceive"
#define SizeBuffer 20
#define buffer1 "Se recibio la señal SIGUSR1\n"
#define buffer2 "Se recibio la señal SIGUSR2\n"
#define buffer3 "Se recibio el mensaje - %s -\n"

void sigusr1_handler(int sig){

write( 1, buffer1, strlen(buffer1) );

}

void sigusr2_handler(int sig){

write( 1, buffer2, strlen(buffer2) );

}

int main(){

struct  sigaction  sa1,sa2;
sa2.__sigaction_handler = sigusr1_handler;
sa1.sa_flags = 0;
sigemptyset(&sa1.sa_mask);

sa2.__sigaction_handler = sigusr2_handler;
sa2.sa_flags = 0;
sigemptyset(&sa2.sa_mask);

char buffer[SizeBuffer];
char log[SizeBuffer];
int32_t fd, code;
uint32_t bytes;


if ( (code = mknod(Name, S_IFIFO | 0666, 0)) < -1){

printf("Error creando la cola nombrada %d\n", code);
exit(1);

}

printf("Esperando por un proceso escritor");

if ( (fd = open(Name, O_RDONLY)) < 0){

printf("Error abriendo la cola nombrada %d\n", fd);
exit(1);

}

printf("Tengo un proceso escritor");

if ( sigaction(SIGUSR1, &sa1, NULL) == -1 || sigaction(SIGUSR2, &sa2, NULL) == -1 ){
    perror("sigaction:");
    exit(1);
}

do{

if ( (bytes = read(fd, buffer, SizeBuffer)) == -1) {

    perror("read:");

}else{

buffer[SizeBuffer] = '\0';
write( 1, buffer, strlen(buffer));

}

}while (bytes > 0);

    return 0;
}