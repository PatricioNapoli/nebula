#include "exception/engine_exception.h"

#include <utility>

namespace nebula {
    EngineException::EngineException(string message, string component, EngineException::Severity severity) :
            _message(std::move(message)), _component(std::move(component)), _severity(severity) {}

    const char* EngineException::what() const noexcept {
        return _message.c_str();
    }

    string EngineException::getComponent() {
        return _component;
    }

    EngineException::Severity EngineException::getSeverity() {
        return _severity;
    }
}
