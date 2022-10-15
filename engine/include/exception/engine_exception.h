#pragma once

#include <exception>

#include "memory/containers.h"

namespace nebula {
    class EngineException : public std::exception {
    public:
        enum Severity
        {
            Warning = 1,
            Fatal = 2
        };

        explicit EngineException(string message, string component, Severity severity);

        [[nodiscard]] const char* what() const noexcept override;

        string getComponent();
        Severity getSeverity();
    private:
        string _message;
        string _component;
        Severity _severity;
    };
}
