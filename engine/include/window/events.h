#pragma once

#include "concurrency/messages.h"

namespace nebula::window {
    struct FocusMsg : messaging::Message {
        NEBULA_EVENT_NAME(window.focus)

        int focus{0};

        explicit FocusMsg(int focus) : focus(focus) {}
    };

    struct ResizeMsg : messaging::Message {
        NEBULA_EVENT_NAME(window.resize)

        int width{0};
        int height{0};

        ResizeMsg(int width, int height) : width(width), height(height) {}
    };

    struct ScrollMsg : messaging::Message {
        NEBULA_EVENT_NAME(window.scroll)

        double xOffset{0.0};
        double yOffset{0.0};

        ScrollMsg(double xOffset, double yOffset) : xOffset(xOffset), yOffset(yOffset) {}
    };

    struct CursorMsg : messaging::Message {
        NEBULA_EVENT_NAME(window.cursor)

        double xPos{0.0};
        double yPos{0.0};

        double xOffset{0.0};
        double yOffset{0.0};

        CursorMsg(double xPos, double yPos) : xPos(xPos), yPos(yPos) {}

        CursorMsg(double xPos, double yPos, double xOffset, double yOffset) : xPos(xPos), yPos(yPos),
                                                                              xOffset(xOffset),
                                                                              yOffset(yOffset) {}
    };

    struct KeyMsg : messaging::Message {
        NEBULA_EVENT_NAME(window.key)

        int key{0};
        int action{0};

        KeyMsg(int key, int action) : key(key), action(action) {}
    };

    struct MouseBtnMsg : messaging::Message {
        NEBULA_EVENT_NAME(window.mousebtn)

        int btn{0};
        int action{0};

        MouseBtnMsg(int btn, int action) : btn(btn), action(action) {}
    };
}
