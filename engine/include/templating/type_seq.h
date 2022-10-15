#pragma once

namespace nebula {
    class TypeId {
        typedef std::uint32_t id_t;

        [[nodiscard]] static id_t value() {
            static std::atomic<id_t> value{};
            return value++;
        }

        [[nodiscard]] explicit operator id_t() const { return value(); }
    };
}
