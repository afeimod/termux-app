#include <unistd.h>
#include <asm-generic/fcntl.h>
#include <sys/socket.h>
#include <linux/un.h>
#include "OutputClient.h"

void OutputClient::Init(int socket) {
    dataSocket = socket;
    //设置成非阻塞模式；
    //获取文件的flags值。
    int flags = fcntl(dataSocket, F_GETFL, 0);
    fcntl(dataSocket, F_SETFL, flags | O_NONBLOCK);
    //设置成阻塞模式；
    /*int flags  = fcntl(dataSocket,F_GETFL,0);
    fcntl(dataSocket,F_SETFL,flags&~O_NONBLOCK);*/
}

void OutputClient::Reset() {
    close(dataSocket);
    dataSocket = -1;
}

void OutputClient::Destroy() {
    Reset();
}

void OutputClient::SendOutputEvent(OutputEvent ev) {
    send(dataSocket, &ev, sizeof(ev), MSG_DONTWAIT);
}

OutputClient::~OutputClient() {
    Reset();
}
