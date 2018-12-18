#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pipe_networking.h"

char *WKP = "pk";

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client)
{
    char buf[256];
    int fd = mkfifo(WKP, 0644);
    if (fd < 0 && errno != EEXIST)
    {
        perror("Failed to create named pipe");
    }
    printf("Waiting for connection...\n");
    fd = open(WKP, O_RDONLY);
    if (fd < 0)
    {
        perror("Failed to open the pipe you just created???");
    }
    read(fd, buf, 256);
    if (strncmp(buf, "WhiteAlbum", 10) != 0)
    {
        perror("Received bad message, handshake aborted");
    }
    printf("Received message from client, sending message\n");
    write(fd, "weeps", 5);
    return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server)
{
    int fd = mkfifo("fire", 0644);
    if (fd < 0 && errno != EEXIST)
    {
        perror("Failed to create named pipe");
    }

    fd = open(WKP, O_WRONLY);
    if (fd < 0)
    {
        perror("Failed open the server's pipe");
    }
    write(fd, "WhiteAlbum", 10);
    return 0;
}
