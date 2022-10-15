#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config/configuration.h"
#include "injector/injectable.h"
#include "concurrency/messaging.h"
#include "logging/logger.h"

namespace nebula {
    class Window : public Injectable {
        const char* _title{};
        bool _borderless{false};

        GLFWwindow* _window{};
        GLFWmonitor* _monitor{};

        bool _close{false};

        int _width{1024}, _height{768};

        Configuration* _config{nullptr};
        Messaging* _messaging{nullptr};

        Logger* _logger{nullptr};
        FixedTimer* _fixedTimer{nullptr};

    public:
        Window() = default;
        ~Window() override;

        static void framebufferResize(GLFWwindow* window, GLint width, GLint height);
        static void windowFocus(GLFWwindow* window, GLint focus);

        void showCursor() {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void hideCursor() {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        GLFWwindow* getWindow() {
            return _window;
        }

        Messaging* getMessaging() {
            return _messaging;
        }

        int getWidth() {
            return _width;
        }

        int getHeight() {
            return _height;
        }

        void update() const {
            {
                NEBULA_PROFILE;
                glfwSwapBuffers(_window);
            }

            if (!_fixedTimer->shouldUpdate())
                return;

            glfwPollEvents();
        }

        bool isClosed() const {
            return glfwWindowShouldClose(_window) == 1 || _close;
        }

        static void keyCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods);
        static void mouseButtonCallback(GLFWwindow* window, GLint button, GLint action, GLint mods);
        static void cursorPositionCallback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
        static void scrollCallback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);

    NEBULA_INJECTABLE(Window);
    };
}
