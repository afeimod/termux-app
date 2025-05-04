#ifndef OUTPUT_EVENT_H
#define OUTPUT_EVENT_H

#include <stdint.h>

#ifndef EVENT_TYPE_ENUM
#define EVENT_TYPE_ENUM
typedef enum {
    NONE,
    EVENT_SCREEN_SIZE,
    EVENT_TOUCH,
    EVENT_MOUSE,
    EVENT_KEY,
    EVENT_STYLUS,
    EVENT_STYLUS_ENABLE,
    EVENT_UNICODE,
    EVENT_CLIPBOARD_ENABLE,
    EVENT_CLIPBOARD_ANNOUNCE,
    EVENT_CLIPBOARD_REQUEST,
    EVENT_CLIPBOARD_SEND,
    EVENT_CLIENT_EXIT,
    EVENT_FRAME_COMPLETE,
    EVENT_TOUCH_DOWN,
    EVENT_TOUCH_UP,
    EVENT_TOUCH_MOVE,
    EVENT_TOUCH_POINTER_UP,
    EVENT_DRAW_FRAME,
} event_type;
#endif
#ifndef SERVER_EVENT_TYPE_ENUM
#define SERVER_EVENT_TYPE_ENUM
typedef enum {
    SERVER_TOUCH_DOWN = 0,
    SERVER_TOUCH_UP = 1,
    SERVER_TOUCH_POINTER_DOWN = 2,
    SERVER_TOUCH_POINTER_UP = 3,
    SERVER_TOUCH_CANCEL = 4,
    SERVER_TOUCH_MOVE = 5,
} server_touch_action;
typedef enum {
    SERVER_MOUSE_UNDEFINE = 0,
    SERVER_MOUSE_LEFT_BUTTON = 1,
    SERVER_MOUSE_MIDDLE_BUTTON = 2,
    SERVER_MOUSE_RIGHT_BUTTON = 3,
    SERVER_MOUSE_SCROLL = 4,
} server_mouse_action;
typedef struct {
    int x;
    int y;
    int id;
} server_touch_pointer;
#endif
typedef struct {
    uint8_t num_pointers;
    uint8_t t;
    uint16_t type, id, x, y;
} server_touch_event;
typedef union {
    uint8_t type;
    struct {
        uint8_t t;
        uint16_t width, height, framerate;
    } screenSize;
    server_touch_event touch;
    server_touch_event touch_events[4];
    struct {
        server_touch_action action;
        server_touch_pointer **pointers;
        uint32_t events;
        uint32_t num_pointers;
        uint32_t index;
        uint64_t time;
    } raw_touch;
    struct {
        uint8_t t;
        float x, y;
        uint8_t detail, down, relative;
    } mouse;
    struct {
        uint8_t t;
        uint16_t key;
        uint8_t state;
        uint8_t mod;
    } key;
    struct {
        uint8_t t;
        float x, y;
        uint16_t pressure;
        int8_t tilt_x, tilt_y;
        int16_t orientation;
        uint8_t buttons, eraser, mouse;
    } stylus;
    struct {
        uint8_t t, enable;
    } stylusEnable;
    struct {
        uint8_t t;
        uint32_t code;
    } unicode;
    struct {
        uint8_t t;
        uint8_t enable;
    } clipboardEnable;
    struct {
        uint8_t t;
        uint32_t count;
    } clipboardSend;
    struct {
        uint64_t timestamp;
    } frame;
} server_termux_event;
#endif
