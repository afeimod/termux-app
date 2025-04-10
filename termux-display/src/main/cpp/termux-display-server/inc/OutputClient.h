#pragma once

#include "SocketIPCServer.h"
#include "OutputEvent.h"
class   OutputClient{
public:
    void Init(int socket);
    void Reset();
    void Destroy();
    void SendOutputEvent(termux_event ev);
private:
    int dataSocket;
    ~OutputClient();
};
