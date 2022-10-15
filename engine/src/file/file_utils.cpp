#include "file/file_utils.h"

#include <algorithm>
#include <sys/stat.h>

namespace nebula::file {
    string getLastFilePart(string file) {
        std::replace(file.begin(), file.end(), '/', ' ');

        vector<string> array;
        stringstream ss(file);
        string temp;
        while (ss >> temp)
            array.emplace_back(temp);

        return array[array.size() - 1];
    }

    bool exists(const string& name) {
        struct stat buffer{};
        return (stat (name.c_str(), &buffer) == 0);
    }
}
