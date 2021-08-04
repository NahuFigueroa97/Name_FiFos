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
#define SizeBuffer 20

int main(){
char buffer[SizeBuffer];
int32_t fd = 0, code = 0;
uint32_t bytesWrite = 0;

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

    fgets(buffer, SizeBuffer, stdin);

    if( (bytesWrite = write(fd, buffer, strlen(buffer)-1) == -1) ){

        perror("Write: ");

    }else {

        if(buffer != SIGUSR1 && buffer != SIGUSR2){

            printf("DATA: \n", buffer);

        }else if (buffer == SIGUSR1){
            
            printf("SIGN: 1\n");

        }else {

            printf("SIGN: 2\n");

        }

    }
}

    return 0;
}