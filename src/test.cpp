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

#define SERVER_PORT 12345

int main (int argc, char *argv[])
{
    int    i, num, rc, flag = 1;
    int    fd, listen_sd, accept_sd = -1;

    size_t local_addr_length;
    size_t remote_addr_length;
    size_t total_sent;

    struct sockaddr_in6   addr;
    struct sockaddr_in6   local_addr;
    struct sockaddr_in6   remote_addr;
    struct sf_parms      parms;

    char   buffer[255];

    /*************************************************/
    /* If an argument is specified, use it to        */
    /* control the number of incoming connections    */
    /*************************************************/
    if (argc >= 2)
        num = atoi(argv[1]);
    else
        num = 1;

    /*************************************************/
    /* Create an AF_INET6 stream socket to receive   */
    /* incoming connections on                       */
    /*************************************************/
    listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sd < 0)
    {
        perror("socket() failed");
        exit(-1);
    }

    /*************************************************/
    /* Set the SO_REUSEADDR bit so that you do not   */
    /* need to wait 2 minutes before restarting      */
    /* the server                                    */
    /*************************************************/
    rc = setsockopt(listen_sd,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    (char *)&flag,
                    sizeof(flag));
    if (rc < 0)
    {
        perror("setsockop() failed");
        close(listen_sd);
        exit(-1);
    }

    /*************************************************/
    /* Bind the socket                               */
    /*************************************************/
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family      = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port        = htons(SERVER_PORT);
    rc = bind(listen_sd,
              (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0)
    {
        perror("bind() failed");
        close(listen_sd);
        exit(-1);
    }

    /*************************************************/
    /* Set the listen backlog                        */
    /*************************************************/
    rc = listen(listen_sd, 5);
    if (rc < 0)
    {
        perror("listen() failed");
        close(listen_sd);
        exit(-1);
    }

    /*************************************************/
    /* Initialize the local and remote addr lengths  */
    /*************************************************/
    local_addr_length  = sizeof(local_addr);
    remote_addr_length = sizeof(remote_addr);

    /*************************************************/
    /* Inform the user that the server is ready      */
    /*************************************************/
    printf("The server is ready\n");

    /*************************************************/
    /* Go through the loop once for each connection  */
    /*************************************************/
    for (i=0; i < num; i++)
    {
        /**********************************************/
        /* Wait for an incoming connection            */
        /**********************************************/
        printf("Iteration: %d\n", i+1);
        printf("  waiting on accept_and_recv()\n");

        rc = accept_and_recv(listen_sd,&accept_sd,(struct sockaddr *)&remote_addr,&remote_addr_length,(struct sockaddr *)&local_addr,&local_addr_length,&buffer,sizeof(buffer));
        if (rc < 0)
        {
            perror("accept_and_recv() failed");
            close(listen_sd);
            close(accept_sd);
            exit(-1);
        }
        printf("  Request for file: %s\n", buffer);

        /**********************************************/
        /* Open the file to retrieve                  */
        /**********************************************/
        fd = open(buffer, O_RDONLY);
        if (fd < 0)
        {
            perror("open() failed");
            close(listen_sd);
            close(accept_sd);
            exit(-1);
        }

        /**********************************************/
        /* Initialize the sf_parms structure          */
        /**********************************************/
        memset(&parms, 0, sizeof(parms));
        parms.file_descriptor = fd;
        parms.file_bytes      = -1;

        /**********************************************/
        /* Initialize the counter of the total number */
        /* of bytes sent                              */
        /**********************************************/
        total_sent = 0;

        /**********************************************/
        /* Loop until the entire file has been sent   */
        /**********************************************/
        do
        {
            rc = send_file(&accept_sd, &parms, SF_CLOSE);
            if (rc < 0)
            {
                perror("send_file() failed");
                close(fd);
                close(listen_sd);
                close(accept_sd);
                exit(-1);
            }
            total_sent += parms.bytes_sent;

        } while (rc == 1);

        printf("  Total number of bytes sent: %lu\n", total_sent);

        /**********************************************/
        /* Close the file that is sent out            */
        /**********************************************/
        close(fd);
    }

    /*************************************************/
    /* Close the listen socket                       */
    /*************************************************/
    close(listen_sd);

    /*************************************************/
    /* Close the accept socket                       */
    /*************************************************/
    if (accept_sd != -1)
        close(accept_sd);
}
