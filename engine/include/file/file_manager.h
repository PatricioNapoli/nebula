#pragma once

#include <utility>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <mutex>
#include <iostream>

#include "memory/containers.h"
#include "string/string_utils.h"
#include "profiling/profiler.h"
#include "injector/injectable.h"
#include "exception/engine_exception.h"

namespace nebula {
    class Logger;

    class FileManager : public Injectable {
    public:
        FileManager() = default;

        struct File {
            string path{};

            string str{};
            vector<uint8_t> bytes{};

            enum Type {
                String,
                Vector
            } type{String};

            string& toString() {
                str = string(bytes.begin(), bytes.end());
                bytes.clear();
                bytes.shrink_to_fit();

                return str;
            }

            vector<uint8_t>& toVector() {
                bytes = vector<uint8_t>(str.begin(), str.end());
                str.clear();
                str.shrink_to_fit();

                return bytes;
            }

            File() = default;
            File(string path, string str) : path(std::move(path)), str(std::move(str)), type(String) {}
            File(string path, vector<uint8_t> bytes) : path(std::move(path)), type(Vector) {
                this->bytes.insert(this->bytes.end(), std::make_move_iterator(bytes.begin()), std::make_move_iterator(bytes.end()));
            }
        };

        FileManager::File& cache(const string& filepath, File::Type t);
        FileManager::File read(const string& filepath, File::Type t);
        FileManager::File get(const string& id);
        void del(const string& id);

        void write(const string& path, const string& data);
        void write(const string& path, vector<uint8_t>& data);
    private:
        template <typename B>
        void write(const string& path, B data, size_t size) {
            NEBULA_PROFILE_D(path);

            auto p = std::filesystem::path(path);

            std::ios::openmode om = std::ios::out;
            if (std::filesystem::exists(p))
                om = std::ios::out;

            auto f = std::fstream(path, om | std::ios::binary);

            f.write(data, size);
            f.close();
        }

        hmap<string, File> _files;
        std::mutex _mutex;

    NEBULA_INJECTABLE(FileManager);
    };
}
