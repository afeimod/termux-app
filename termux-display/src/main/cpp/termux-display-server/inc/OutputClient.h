#pragma once

#include "SocketIPCServer.h"
#include "OutputEvent.h"
class   OutputClient{
public:
    void init(int socket);
    void reset();
    void destroy();
    void sendOutputEvent(server_termux_event ev);
private:
    int dataSocket;
    ~OutputClient();
};
