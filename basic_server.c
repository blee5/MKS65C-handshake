#include <string.h>
#include <signal.h>

#include "pipe_networking.h"

int to_client;
int from_client = -1;

void sighandler(int s)
{
    switch (s)
    {
        case SIGINT:
            remove("pkfreeze");
            exit(0);
        case SIGPIPE:
            close(to_client);
            printf("Client disconnected.\n");
            exit(0);
    }
}

void process_input()
{
    char buf[256];
    for (;;)
    {
        memset(buf, 0, 256);
        read(from_client, buf, 256);
        sprintf(buf, "Length of string: %ld\n", strlen(buf));
        write(to_client, buf, 256);
    }
}

int main()
{
    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    int pid;

    for (;;)
    {
        while (from_client == -1)
        {
            from_client = server_handshake(&to_client);
        }
        pid = fork();
        if (pid == 0)
        {
            /* Child process */
            process_input();
        }
        else
        {
            /* Parent process */
            from_client = -1;
        }
    }
}
