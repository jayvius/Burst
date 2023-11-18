#ifndef SERVER_H
#define SERVER_H

#include <optional>

class ByteCodeStreamer
{
public:
    virtual size_t get(uint8_t *buffer, size_t len) = 0;
};

class NetworkStreamer : public ByteCodeStreamer
{
public:
    NetworkStreamer();
    virtual size_t get(uint8_t *buffer, size_t len);

private:
    int listenSocket;
    std::optional<int> acceptSocket;
};

#endif
