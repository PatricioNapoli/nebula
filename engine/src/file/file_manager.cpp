#include "file/file_manager.h"

#include <filesystem>

namespace nebula {

    void FileManager::mapDependencies(EnvironmentVars &globalEnv) {}

    void FileManager::onConstruct() {}

    FileManager::File FileManager::read(const string& filepath, File::Type t) {
        NEBULA_PROFILE_D(filepath);

        std::ifstream in(filepath, std::ios::binary);

        if (in) {
            auto p = std::filesystem::path(filepath);
            auto fileSize = std::filesystem::file_size(p);

            switch(t) {
                case File::String:
                {
                    string s(fileSize, ' ');
                    in.read(&s[0], s.size());
                    in.close();
                    return File(filepath, s);
                }
                case File::Vector:
                {
                    vector<uint8_t> vec(fileSize);
                    in.read(reinterpret_cast<char*>(&vec.front()), vec.size());
                    in.close();
                    return File(filepath, vec);
                }
            }
        }

        in.close();
        throw EngineException("Failed to read file: " + filepath, __FILE__, EngineException::Warning);
    }

    FileManager::File& FileManager::cache(const string& filepath, File::Type t) {
        if (!_files[filepath].path.empty())
            return _files[filepath];

        std::lock_guard<std::mutex> lock(_mutex);

        this->_files[filepath] = read(filepath, t);
        return _files[filepath];
    }

    FileManager::File FileManager::get(const string& id) {
        return _files[id];
    }

    void FileManager::del(const string& filepath) {
        _files.erase(filepath);
    }

    void FileManager::write(const string& path, const string& data) {
        write(path, (char*)(&data[0]), data.size());
    }

    void FileManager::write(const string& path, vector<uint8_t>& data) {
        write(path, reinterpret_cast<char *>(&data[0]), data.size());
    }
}
