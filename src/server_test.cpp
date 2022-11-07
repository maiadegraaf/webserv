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
#include <sys/ioctl.h>

#define TRUE             1
#define FALSE            0

using namespace std;
int main(int argc, char *argv[])
{
    int    rc, len = 0, on = 1, nfds = 16, newSd = -1;
    struct pollfd fds[200];
    char   buffer[80];
    int serverSd = -1;
    string sBuf;
    int    close_conn, compress_array = FALSE, end_server = FALSE;
    char    header[] = "HTTP/1.1 200 OK\r\n\r\n";
    sockaddr_in servAddr;


    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    int port = atoi(argv[1]);
    serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0)
    {
        perror("socket() failed");
        exit(-1);
    }
    rc = setsockopt(serverSd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0)
    {
        perror("setsockopt() failed");
        close(serverSd);
        exit(-1);
    }
    rc = ioctl(serverSd, FIONBIO, (char *)&on);
    if (rc < 0)
    {
        perror("ioctl() failed");
        close(serverSd);
        exit(-1);
    }
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    rc = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if(rc < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        close(serverSd);
        exit(-1);
    }
    rc = listen(serverSd, 32);
    if (rc < 0)
    {
        perror("listen() failed");
        close(serverSd);
        exit(0);
    }
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = serverSd;
    fds[0].events = POLLIN;
    int timeout = (3 * 60 * 1000);
    do  {
        cout << "waiting poll..." << endl;
        rc = poll(fds, nfds, timeout);
        if (rc == 0)
        {
            printf("  poll() timed out.  End program.\n");
            break;
        }
        int current_size = nfds;
        for (int i = 0; i < current_size; i++) {
            if (fds[i].revents == 0) {
                continue;
            }
//            if (fds[i].revents != POLLIN) {
//                printf("  Error! revents = %d\n", fds[i].revents);
//                end_server = TRUE;
//                break;
//            }
            if (fds[i].fd == serverSd) {
                printf("  Listening socket is readable\n");
                do {
                    newSd = accept(serverSd , NULL, NULL);
                    if (newSd < 0) {
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
            } else {
                printf("  Descriptor %d is readable\n", fds[i].fd);
                close_conn = FALSE;
                while (1) {
                    rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (rc < 0) {
                        if (errno != EWOULDBLOCK) {
                            perror("  recv() failed");
                            close_conn = TRUE;
                        }
                        break;
                    }
                    if (rc == 0) {
                        printf("  Connection closed\n");
                        close_conn = TRUE;
                        break;
                    }
                    len = rc;
                    printf("  %d bytes received\n", len);
//                    rc = send(fds[i].fd, buffer, len, 0);
                }
                int read = open("www/index.html", O_RDONLY);
                off_t len = 0;
                send(fds[i].fd, header, len, 0);
                rc = sendfile(read, fds[i].fd, 0, &len, NULL, 0);
                if (rc < 0) {
                    perror("  send() failed");
                    close_conn = TRUE;
                    break;
                }
                if (close_conn) {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    compress_array = TRUE;
                }
            }
            if (compress_array) {
                compress_array = FALSE;
                for (int i = 0; i < nfds; i++) {
                    if (fds[i].fd == -1) {
                        for (int j = i; j < nfds; j++) {
                            fds[j].fd = fds[j + 1].fd;
                        }
                        i--;
                        nfds--;
                    }
                }
            }
        }
        } while (end_server == FALSE); /* End of serving running.    */
        for (int i = 0; i < nfds; i++) {
            if (fds[i].fd >= 0)
                close(fds[i].fd);
        }
    return 0;
}