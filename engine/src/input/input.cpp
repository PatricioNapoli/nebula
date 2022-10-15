#include <input/input_events.h>
#include "input/input.h"
#include "window/events.h"

#include "concurrency/messages.h"

namespace nebula {
    Input::Input() {
        for (auto& _key : _keys)
            _key = 0;

        for (auto& _mouseButton : _mouseButtons)
            _mouseButton = 0;
    }

    void Input::mapDependencies(EnvironmentVars& globalEnv) {
        map<Configuration>(&_config);
        map<Messaging>(&_messaging);
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
    }

    void Input::onConstruct() {
        _messaging->subscribe<window::KeyMsg>([&](const window::KeyMsg& m) {
            if (m.action != _keys[m.key] && m.action != GLFW_REPEAT) {
                _keys[m.key] = m.action;
                _messaging->publish(input::KeyMsg(m.key, m.action));
            }
        });

        _messaging->subscribe<window::MouseBtnMsg>([&](const window::MouseBtnMsg& m) {
            if (m.action != _mouseButtons[m.btn] && m.action != GLFW_REPEAT) {
                _mouseButtons[m.btn] = m.action;
                _messaging->publish(input::MouseBtnMsg(m.btn, m.action));
            }
        });

        _messaging->subscribe<window::CursorMsg>([&](const window::CursorMsg& m) {
            auto mx = m.xPos;
            auto my = m.yPos;

            if (_isMouseFocus) {
                _mx = mx;
                _my = my;
                _isMouseFocus = false;
            }

            _mxOffset = _mx - mx;
            _myOffset = _my - my;

            _mx = mx;
            _my = my;

            if (_mxOffset != 0 || _myOffset != 0) {
                _messaging->publish(input::CursorMsg(_mx, _my, _mxOffset, _myOffset));

                _mxOffset = 0;
                _myOffset = 0;
            }
        });

        _messaging->subscribe<window::ScrollMsg>([&](const window::ScrollMsg& m) {
            _scrollXOffset = m.xOffset;
            _scrollYOffset = m.yOffset;
        });

        _messaging->subscribe<window::FocusMsg>([&](const window::FocusMsg& m) {
            _isMouseFocus = m.focus;
        });
    }

    bool Input::isKeyPressed(unsigned int keycode) const {
        if (keycode >= static_cast<unsigned int>(MAX_KEYS))
            return false;
        return _keys[keycode] != GLFW_RELEASE;
    }

    bool Input::isMouseButtonPressed(unsigned int button) const {
        if (button >= static_cast<unsigned int>(MAX_BUTTONS))
            return false;
        return _mouseButtons[button] != GLFW_RELEASE;
    }
}
