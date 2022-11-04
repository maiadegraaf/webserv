#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#define TRUE             1
#define FALSE            0

using namespace std;
int main(int argc, char *argv[])
{
    int    len, on = 1;
    struct pollfd fds[200];
    bool   end_server = FALSE;
    char   buffer[80];
    int    close_conn;


    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    int port = atoi(argv[1]);

    char msg[1500];
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    int rc = listen(serverSd, 32);
    if (rc < 0)
    {
        perror("listen() failed");
        close(serverSd);
        exit(0);
    }
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    int read = open("www/index.html", O_RDONLY);
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = newSd;
    fds[0].events = POLLIN;
    int timeout = (3 * 60 * 1000);
    int nfds = 1;
    do  {
        cout << "Awaiting client response..." << endl;
        rc = poll(fds, nfds, timeout);
        if (rc == 0)
        {
            printf("  poll() timed out.  End program.\n");
            break;
        }
        int current_size = nfds;
        for (int i = 0; i < current_size; i++)
        {
            if(fds[i].revents == 0)
                continue;
            if(fds[i].revents != POLLIN)
            {
                printf("  Error! revents = %d\n", fds[i].revents);
                end_server = TRUE;
                break;

            }
            if (fds[i].fd == serverSd)
            {
                printf("  Listening socket is readable\n");
                do
                {
                    newSd = accept(serverSd, NULL, NULL);
                    if (newSd < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("  accept() failed");
                            end_server = TRUE;
                        }
                        break;
                    }
                    printf("  New incoming connection - %d\n", newSd);
                    fds[nfds].fd = newSd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } while (newSd != -1);
            }
            else
            {
                do
                {
                    rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (rc < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("  recv() failed");
                            close_conn = TRUE;
                        }
                        break;
                    }
                    if (rc == 0)
                    {
                        printf("  Connection closed\n");
                        close_conn = TRUE;
                        break;
                    }
                    len = rc;
                    printf("  %d bytes received\n", len);
                    rc = send(fds[i].fd, buffer, len, 0);
                    if (rc < 0)
                    {
                        perror("  send() failed");
                        close_conn = TRUE;
                        break;
                    }
                } while(TRUE);
                if (close_conn)
                {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    int compress_array = TRUE;
                }
            }
        }
        off_t len = 10000;
//        memset(&msg, 0, sizeof(msg));
//        recv(newSd, (char*)&msg, sizeof(msg), 0);
//        std::cout << msg << std::endl;
//        sendfile(read, newSd, 0, &len, NULL, 0);
    }  while (end_server == FALSE);
    close(newSd);
    close(serverSd);
    return 0;
}