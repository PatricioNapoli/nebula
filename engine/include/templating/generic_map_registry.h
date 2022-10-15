#pragma once

#include <unordered_map>
#include <algorithm>

namespace nebula {
#define GEN_MAP_REG \
        template<class CMap> \
        CMap& _genericMapReg() { \
            static CMap _registry; \
            return _registry; \
        }
}
