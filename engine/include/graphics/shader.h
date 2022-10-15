#pragma once

#include "templating/crtp.h"

namespace nebula {
    struct IShader {
        string name{};

        enum Cull {
            BACK,
            FRONT
        } cull{BACK};

        enum Depth {
            LESS,
            LEQUAL,
            GREATER,
            GEQUAL
        } depth{LESS};

        enum Stencil {

        };

        bool blending{false};
        bool cullTest{true};
        bool depthTest{true};
        bool stencilTest{false};
        bool multisample{true};

        const char* vert{};
        const char* frag{};

        IShader() = default;
        explicit IShader(string name, const char* vert, const char* frag) : name(std::move(name)), vert(vert), frag(frag) {}

        NEBULA_CRTP
    };
}
