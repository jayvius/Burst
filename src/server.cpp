#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

NetworkStreamer::NetworkStreamer()
{
    printf("starting listen thread\n");
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        fprintf(stderr, "ERROR: could not open socket\n");
        exit(1);
    }

    int optval = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        close(s);
        fprintf(stderr, "ERROR: setsockopt error\n");
        exit(1);
    }

    struct addrinfo hints, *addr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "3333", &hints, &addr);
    if (bind(s, addr->ai_addr, addr->ai_addrlen) == -1) {
        close(s);
        fprintf(stderr, "ERROR: bind error\n");
        exit(1);
    }

    freeaddrinfo(addr);

    if (listen(s, 1) == -1) {
        close(s);
        fprintf(stderr, "ERROR: listen error\n");
        exit(1);
    }

    this->listenSocket = s;
}

size_t NetworkStreamer::get(uint8_t *buffer, size_t len)
{
    while (!this->acceptSocket) {
        struct sockaddr_storage remote;
        socklen_t sin_size;
        this->acceptSocket = accept(this->listenSocket, (struct sockaddr *)&remote, &sin_size);
        if (*(this->acceptSocket) == -1) {
            fprintf(stderr, "ERROR: accept error\n");
            this->acceptSocket.reset();
        }
    }

    size_t count = recv(*(this->acceptSocket), buffer, len, 0);
    if (count == 0) {
        close(*(this->acceptSocket));
        this->acceptSocket.reset();
    }
    return count;
}
