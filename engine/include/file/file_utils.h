#pragma once

#include <vector>
#include <sstream>

#include "memory/containers.h"

namespace nebula::file {
    string getLastFilePart(string file);
    bool exists(const string& name);
}
