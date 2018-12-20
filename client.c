#include <string.h>
#include <signal.h>

#include "pipe_networking.h"

int to_server;
int from_server = -1;

void sighandler(int s)
{
    switch (s)
    {
        case SIGINT:
            remove("pkfire");
            exit(0);
        case SIGPIPE:
            close(to_server);
            printf("Server disconnected the connection.\n");
            exit(0);
    }
}

int main()
{
    char buf[256];

    signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);

    for (;;)
    {
        while (from_server == -1)
        {
            from_server = client_handshake(&to_server);
            if (from_server == -1)
            {
                printf("Retrying handshake in 5 seconds...\n");
                sleep(5);
            }
        }

        memset(buf, 0, 256);
        printf("Enter message to send:\n");
        read(STDIN_FILENO, buf, 256);
        buf[255] = 0;
        buf[strlen(buf) - 1] = 0;
        write(to_server, buf, 256);
        read(from_server, buf, 256);
        printf("Response from server:\n%s\n", buf);
    }
}
