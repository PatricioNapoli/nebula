#include "string/string_utils.h"

#include <algorithm>

namespace nebula::string_utils {
    string remove(const string& str, char character) {
        string ret;
        std::remove_copy(str.begin(), str.end(), std::back_inserter(ret), character);
        return ret;
    }

    void replace(string& data, const string& search, const string& replace){
        size_t pos = data.find(search);
        while (pos != string::npos) {
            data.replace(pos, search.size(), replace);
            pos = data.find(search, pos + replace.size());
        }
    }
}
