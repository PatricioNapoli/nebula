#include "window/window.h"

#include "exception/engine_exception.h"

#include "window/events.h"

namespace nebula {
    Window::~Window() {
        glfwTerminate();
    }

    void Window::mapDependencies(EnvironmentVars& environment) {
        map<Configuration>(&_config);
        map<Messaging>(&_messaging);
        mapFactory<FixedTimer, double>(&_fixedTimer, 1.0 / 60.0);
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
    }

    void Window::onConstruct() {
        if (_config->get().mode == config::Editor)
            return;

        _title = _config->get().appName.data();

        _messaging->subscribe<window::ResizeMsg>([&](const window::ResizeMsg& m) {
            _width = m.width;
            _height = m.height;
        });

        _messaging->subscribe<window::KeyMsg>([&](const window::KeyMsg& m) {
            int key = m.key;
            int ev = m.action;

            if (key == GLFW_KEY_ESCAPE && ev == GLFW_RELEASE)
                _close = true;
        });

        if (!glfwInit())
            throw EngineException("Failed to initialize window.", __FILE__, EngineException::Fatal);

        _borderless = _config->get().borderless;
        _width = _config->get().windowWidth;
        _height = _config->get().windowHeight;

        glfwWindowHint(GLFW_SAMPLES, 4);

        if (_config->get().debug == config::debug::Mode::ON)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (_borderless) {
            _monitor = glfwGetPrimaryMonitor();
            auto* mode = glfwGetVideoMode(_monitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            _width = mode->width;
            _height = mode->height;

            _config->get().windowWidth = mode->width;
            _config->get().windowWidth = mode->height;

            _window = glfwCreateWindow(mode->width, mode->height, _title, _monitor, nullptr);
        }
        else
            _window = glfwCreateWindow(_width, _height, _title, _monitor, nullptr);

        if (!_window) {
            glfwTerminate();
            throw EngineException("Failed to create window.", __FILE__, EngineException::Fatal);
        }

        glfwMakeContextCurrent(_window);
        glfwSetFramebufferSizeCallback(_window, Window::framebufferResize);
        glfwSetWindowFocusCallback(_window, Window::windowFocus);

        glfwSetKeyCallback(_window, keyCallback);
        glfwSetMouseButtonCallback(_window, mouseButtonCallback);
        glfwSetCursorPosCallback(_window, cursorPositionCallback);
        glfwSetScrollCallback(_window, scrollCallback);

        glfwSetWindowUserPointer(_window, this);
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSwapInterval(_config->get().vsync);

        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        _messaging->publish(window::ResizeMsg(width, height));
    }

    void Window::framebufferResize(GLFWwindow *window, GLint width, GLint height) {
        auto w = (Window*)glfwGetWindowUserPointer(window);
        w->getMessaging()->publish(window::ResizeMsg(width, height));
    }

    void Window::windowFocus(GLFWwindow* window, GLint focus) {
        auto w = (Window*)glfwGetWindowUserPointer(window);
        w->getMessaging()->publish(window::FocusMsg(focus));
    }

    void Window::keyCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods) {
        auto w = (Window*)glfwGetWindowUserPointer(window);
        w->getMessaging()->publish(window::KeyMsg(key, action));
    }

    void Window::mouseButtonCallback(GLFWwindow* window, GLint button, GLint action, GLint mods) {
        auto w = (Window*)glfwGetWindowUserPointer(window);
        w->getMessaging()->publish(window::MouseBtnMsg(button, action));
    }

    void Window::cursorPositionCallback(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {
        auto w = (Window*)glfwGetWindowUserPointer(window);
        w->getMessaging()->publish(window::CursorMsg(xpos, ypos));
    }

    void Window::scrollCallback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset) {
        auto w = (Window*)glfwGetWindowUserPointer(window);
        w->getMessaging()->publish(window::ScrollMsg(xoffset, yoffset));
    }
}
