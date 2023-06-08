#include <type_traits>
#include <utility>

#include "memory/containers.h"

namespace nebula {
    struct Error {
        enum Severity
        {
            Warning = 1,
            Fatal = 2
        };

        Error() = default;
        Error(Severity what, string where, string why) : what(what), where(std::move(where)), why(std::move(why)) {}

        Severity what{Warning};
        string where{};
        string why{};
    };

    template <typename T>
    struct Result {

        explicit Result(T& val) : val(val) {}
        explicit Result(Error& err) : err(err), ok(false) {}

        explicit Result(T&& val) : val(std::move(val)) {}
        explicit Result(Error&& err) : err(std::move(err)), ok(false) {}

        template <typename ...Args>
        explicit Result(Args ...args) : val(std::forward<Args>(args)...) {}

        Result() = default;

        auto with(T& v) {
            val = v;
            return *this;
        }

        auto with(Error& e) {
            err = e;
            ok = false;
            return *this;
        }

        auto with(T&& v) {
            val = std::move(v);
            return *this;
        }

        template <typename ...Args>
        auto with(Args ...args) {
            val = T(std::forward<Args>(args)...);
            return *this;
        }

        template <typename ...Args>
        auto withErr(Args ...args) {
            err = Error(std::forward<Args>(args)...);
            ok = false;
            return *this;
        }

        auto with(Error&& e) {
            err = std::move(e);
            ok = false;
            return *this;
        }

        T val{};
        Error err{};
        bool ok = true;
    };
}
