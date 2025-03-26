#include <sys/timerfd.h>
#include <sys/epoll.h>
#include "SocketIPCServer.h"
#include "OutputClient.h"
#include "server.h"
#include "LogUtil.h"

#define MAX_EVENTS 10
static bool isRunning = false;
static AHardwareBuffer *hwBuffer = nullptr;
static SocketIPCServer *serverRenderer = nullptr;
static int socketFd = -1;
static int dataSocket = -1;
static int outputSocket = -1;
static int epollFd = -1;
static AAssetManager *nativeasset = nullptr;

static JNIEnv *env;
static jobject surface;
static JavaVM *vm;

static OutputClient *outputClient;

#define SOCKET_NAME     "shard_texture_socket"

static jobject displayObject;


extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_initJNIEnv(JNIEnv *env, jobject thiz) {
    displayObject = env->NewGlobalRef(thiz);;
}

void notifyWindowChanged(int state) {
    jclass clazz = env->GetObjectClass(displayObject);
    jmethodID methodId = env->GetMethodID(clazz, "notifyWindowChanged", "(I)V");
    env->CallVoidMethod(displayObject, methodId, state);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_termux_display_Display_setServerNativeAssetManager(JNIEnv *env, jobject thiz,
                                                            jobject asset_manager) {
    if (!nativeasset) { nativeasset = AAssetManager_fromJava(env, asset_manager); }
}

void setNativeWindow(JNIEnv *ev) {
    env = ev;
    if (!vm){
        env->GetJavaVM(&vm);
    }
}
void setSurface(jobject sf){
    surface = sf;
}
void *ServerSetup(void *object);

void DisplayServerInit() {
    if (socketFd < 0) {
        LOG_D("    SERVER_APP_CMD_INIT");
        pthread_t serverThread;
        pthread_create(&serverThread, nullptr, ServerSetup, vm);
    }
}

void *ServerSetup(void *object) {
    int ret;
    struct sockaddr_un serverAddr;
    char socketName[108];

    LOG_I("Start server setup");

    socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketFd < 0) {
        LOG_E("socket: %s", strerror(errno));
        return nullptr;
    }
    LOG_D("socket made.");

    memcpy(&socketName[0], "\0", 1);
    strcpy(&socketName[1], SOCKET_NAME);

    memset(&serverAddr, 0, sizeof(struct sockaddr_un));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, socketName, sizeof(serverAddr.sun_path) - 1); // max is 108

    ret = bind(socketFd, reinterpret_cast<const sockaddr *>(&serverAddr),
               sizeof(struct sockaddr_un));
    if (ret < 0) {
        LOG_E("bind: %s", strerror(errno));
        return nullptr;
    }
    LOG_D("bind made");

    // open 8 back buffers for this demo
    ret = listen(socketFd, 8);
    if (ret < 0) {
        LOG_E("listen: %s", strerror(errno));
        return nullptr;
    }

    LOG_I("ServerSetup complete.");

    epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (epollFd == -1) {
        LOG_E("epoll_create failed:%s", strerror(errno));
        return nullptr;
    }

//    while (1) {
//        // accept
//        int currentSocket = accept(socketFd, nullptr, nullptr);
//        if (currentSocket < 0) {
//            LOG_E("accept: %s", strerror(errno));
//            break;
//        }
//        if (dataSocket < 1) {
//            dataSocket = currentSocket;
//            pthread_t t;
//            pthread_create(&t, nullptr, reinterpret_cast<void *(*)(void *)>(ServerStart), object);
//            LOG_I("accept dataSocket: %d", currentSocket);
//        } else {
//            outputSocket = currentSocket;
//            outputClient = new OutputClient;
//            outputClient->Init(outputSocket);
//
//            struct epoll_event outputEvent;
//            outputEvent.events = EPOLLIN;
//            outputEvent.data.fd = outputSocket;
//            if (epoll_ctl(epollFd, EPOLL_CTL_ADD, outputSocket, &outputEvent) == -1) {
//                LOG_E("output epoll_ctl failed:%s", strerror(errno));
//            }
//            LOG_I("accept outputSocket: %d", currentSocket);
//        }
//    }
    {
        // accept
        dataSocket = accept(socketFd, nullptr, nullptr);
        if (dataSocket < 0) {
            LOG_E("accept: %s", strerror(errno));
        }
        pthread_t t;
        pthread_create(&t, nullptr, reinterpret_cast<void *(*)(void *)>(ServerStart), object);
        LOG_I("accept dataSocket: %d", dataSocket);

        outputSocket = accept(socketFd, nullptr, nullptr);
        if (outputSocket < 0) {
            LOG_E("accept: %s", strerror(errno));
        }
        outputClient = new OutputClient;
        outputClient->Init(outputSocket);

        struct epoll_event outputEvent;
        outputEvent.events = EPOLLIN;
        outputEvent.data.fd = outputSocket;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, outputSocket, &outputEvent) == -1) {
            LOG_E("output epoll_ctl failed:%s", strerror(errno));
        }
        LOG_I("accept outputSocket: %d", outputSocket);
    }

//    LOG_D("Close dataSocket");
//    close(socketFd);
//    close(epollFd);
    return nullptr;
}

void ServerStart(void *object) {
    LOG_D("----------------------------------------------------------------");
    LOG_D("    Server.Start");
    int ret = AHardwareBuffer_recvHandleFromUnixSocket(dataSocket, &hwBuffer);
    if (ret != 0) {
        LOG_E("recvHandleFromUnixSocket: %d", ret);
        return;
    }
    AHardwareBuffer_Desc desc;
    AHardwareBuffer_describe(hwBuffer, &desc);
    LOG_D("recvHandleFromUnixSocket: %d x %d, layer: %d, format: %d", desc.width,
          desc.height, desc.layers, desc.format);
    {

        LOG_D("    datasocket prepared");
        vm->AttachCurrentThread(&env, nullptr);
        notifyWindowChanged(2);
        serverRenderer = SocketIPCServer::GetInstance();
        serverRenderer->m_NativeAssetManager = nativeasset;
        serverRenderer->Init(hwBuffer, env, surface);
        isRunning = true;
        LOG_D("    datasocket connected");

        notifyWindowChanged(0);
        vm->DetachCurrentThread();

        int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
        if (timer_fd == -1) {
            LOG_E("timerfd_create failed:%s", strerror(errno));
            return;
        }

        struct itimerspec timer_spec;
        timer_spec.it_interval.tv_sec = 1; // 1-second interval
//        timer_spec.it_interval.tv_nsec = 30000000;
        timer_spec.it_interval.tv_nsec = 0;
        timer_spec.it_value.tv_sec = 0;
        timer_spec.it_value.tv_nsec = 30000000;//初始延迟秒触发

        if (timerfd_settime(timer_fd, 0, &timer_spec, NULL) == -1) {
            LOG_E("timerfd_settime failed:%s", strerror(errno));
            close(timer_fd);
            return;
        }

        struct epoll_event timerEvent;
        timerEvent.events = EPOLLIN;
        timerEvent.data.fd = timer_fd;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, timer_fd, &timerEvent) == -1) {
            LOG_E("timer epoll_ctl failed:%s", strerror(errno));
            close(timer_fd);
            return;
        }


        struct epoll_event events[MAX_EVENTS];
        int cnt =0;
        while (true) {
            int num_events = epoll_wait(epollFd, events, MAX_EVENTS, -1);
            if (num_events == -1) {
                LOG_E("epoll_wait failed:%s", strerror(errno));
                epoll_ctl(epollFd, EPOLL_CTL_DEL, timer_fd, &timerEvent);
                close(timer_fd);
                break;
            }

            for (int i = 0; i < num_events; ++i) {
                if (events[i].data.fd == timer_fd) {
                    uint64_t expirations;
                    ssize_t s = read(timer_fd, &expirations, sizeof(expirations));
                    if (s != sizeof(expirations)) {
                        LOG_E("read failed:%s", strerror(errno));
                        epoll_ctl(epollFd, EPOLL_CTL_DEL, timer_fd, &timerEvent);
                        close(timer_fd);
                        continue;
                    }
//                    LOG_I("Server Timer expired!");
                    // Add your code to handle timer expiration asynchronously
                    if (isRunning && serverRenderer) {
                        serverRenderer->Draw();
                        cnt++;
                        if (outputClient&&cnt<20){
//                            LOG_I("output finished!");
                            OutputEvent e = {.type=13};
                            outputClient->SendOutputEvent(e);
                        }
                    }
                } else if (events[i].data.fd == outputSocket) {
                    OutputEvent ev;
                    read(outputSocket, &ev, sizeof(ev));
                    if (ev.type == EVENT_CLIENT_EXIT) {
                        vm->AttachCurrentThread(&env, nullptr);
//                        serverRenderer->Destroy();
                        notifyWindowChanged(1);
                        vm->DetachCurrentThread();

                        struct epoll_event outputEvent;
                        timerEvent.events = EPOLLIN;
                        timerEvent.data.fd = outputSocket;
                        epoll_ctl(epollFd, EPOLL_CTL_DEL, outputSocket, &outputEvent);
                        close(outputSocket);
                        outputSocket = -1;

                        close(dataSocket);
                        dataSocket = -1;

                        epoll_ctl(epollFd, EPOLL_CTL_DEL, timer_fd, &timerEvent);
                        close(timer_fd);
                        timer_fd = -1;

                        AHardwareBuffer_release(hwBuffer);
                        hwBuffer = nullptr;

                        return;
                    }
                }
            }
        }
    }
}

void SendOutputEvent(OutputEvent ev) {
    if (outputClient) {
        outputClient->SendOutputEvent(ev);
    }
}

