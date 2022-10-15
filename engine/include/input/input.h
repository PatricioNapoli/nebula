#pragma once

#include "injector/injectable.h"
#include "window/window.h"
#include "concurrency/messaging.h"
#include "logging/logger.h"
#include "config/configuration.h"

#include <glm/glm.hpp>

namespace nebula {
    class Input : public Injectable {
        Messaging* _messaging{nullptr};
        Configuration* _config{nullptr};

        enum {
            MAX_KEYS = 1024,
            MAX_BUTTONS = 32
        };

        int _keys[MAX_KEYS]{};
        int _mouseButtons[MAX_BUTTONS]{};
        double _mx{}, _my{};

        // How much mouse pointer moved from last frame
        double _mxOffset{0.0}, _myOffset{0.0};
        double _scrollXOffset{}, _scrollYOffset{};

        bool _isMouseFocus{false};

        Logger* _logger{nullptr};

    public:
        Input();

        bool isKeyPressed(unsigned int keycode) const;
        bool isMouseButtonPressed(unsigned int button) const;

        glm::vec2 getMousePosition() const {
            return glm::vec2(_mx, _my);
        }

        glm::vec2 getMouseOffsets() const {
            return glm::vec2(_mxOffset, _myOffset);
        }

        glm::vec2 getScrollOffsets() const {
            return glm::vec2(_scrollXOffset, _scrollYOffset);
        }

    NEBULA_INJECTABLE(Input);
    };
}
