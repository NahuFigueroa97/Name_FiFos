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

#define Name "MiFifo"
#define buffer1 "SIGN: 1"
#define buffer2 "SIGN: 2"
#define SizeBuffer 20
#define Message "XXXX: "

volatile sig_atomic_t signalNumber;

void handler1(int sig){

    signalNumber = 1;

}
void handler2(int sig){

    signalNumber = 2;

}

int main(){

char bufferOut[SizeBuffer];
int32_t code = 0, fd = 0;
uint32_t bytesWrite = 0;
struct sigaction sa;
sa.sa_handler = handler1;
sa.sa_flags = 0;

if (sigemptyset(&sa.sa_mask) == -1){

    perror("sigemptyset");
    exit(1);

}
if (sigaction(SIGUSR1, &sa, NULL) == -1){

    perror("sigaction");
    exit(1);

}
sa.sa_handler = handler2;
sa.sa_flags = 0;

if (sigemptyset(&sa.sa_mask) == -1){

    perror("sigemptyset");
    exit(1);

}

if (sigaction(SIGUSR2, &sa, NULL) == -1){

    perror("sigaction");
    exit(1);

}

if ( (code = mknod(Name, S_IFIFO | 0644, 0)) < -1) {

printf(" Error creando la cola nombrada: %d\n", code);
exit(1);

}

printf("Esperando por un proceso lector \n");

if ( (fd = open(Name, O_WRONLY)) < 0 ){

printf(" Error abriendo la cola nombrada: %d\n", fd);
exit(1);

}

printf("Tengo un proceso lector!! \n");

while (1){

    fgets(bufferOut, SizeBuffer, stdin);

    char bufferIn[sizeof(Message) + SizeBuffer];

    if (signalNumber != 0){

        sprintf(bufferIn, "SIGN:%d\n", signalNumber);
        signalNumber = 0;

    }
    else
    {
        sprintf(bufferIn, "DATA:%s", bufferOut);
    }

    if( (bytesWrite = write(fd, bufferIn, strlen(bufferIn)-1) == -1) ){

        perror("Write: ");

    }else {
        
        printf("Escritor: se escribieron %d bytes\n", bytesWrite);

    }
}

    return 0;
}