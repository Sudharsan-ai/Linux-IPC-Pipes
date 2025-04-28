#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>

void server(int rfd, int wfd); 
void client(int wfd, int rfd); 

int main() { 
    int p1[2], p2[2]; 
    pid_t pid;

   
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid = fork(); 
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { 
      
        close(p1[1]); 
        close(p2[0]); 
        server(p1[0], p2[1]); 
        exit(0);
    } 

  
    close(p1[0]); 
    close(p2[1]);
    client(p1[1], p2[0]); 
    
    wait(NULL); 
    return 0; 
} 

void server(int rfd, int wfd) { 
    int n; 
    char fname[2000]; 
    char buff[2000];

  
    n = read(rfd, fname, sizeof(fname) - 1);
    if (n <= 0) {
        perror("server: failed to read filename");
        return;
    }
    fname[n] = '\0';

    int fd = open(fname, O_RDONLY);
    if (fd < 0) { 
        char *errmsg = "can't open\n";
        write(wfd, errmsg, strlen(errmsg));
    } else { 
        while ((n = read(fd, buff, sizeof(buff))) > 0) {
            write(wfd, buff, n); 
        }
        close(fd);
    } 
}

void client(int wfd, int rfd) {
    int n; 
    char fname[2000];
    char buff[2000];

    printf("Enter filename: ");
    fflush(stdout);


    if (scanf("%s", fname) != 1) {
        perror("client: failed to read filename");
        exit(1);
 
    write(wfd, fname, strlen(fname) + 1); 

  
    while ((n = read(rfd, buff, sizeof(buff))) > 0) {
        write(1, buff, n); 
    }
}

