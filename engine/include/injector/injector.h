#pragma once

#include <unordered_map>
#include "memory/containers.h"

#include "injectable.h"

namespace nebula {
    class Injector {
        hmap<string, hmap<string, Injectable*>> _services;
        hmap<string, string> _di;

        EnvironmentVars _environment;

        vector<Injectable*> _factoryCreated;

    public:
        explicit Injector(EnvironmentVars environment);
        ~Injector();

        template <typename T>
        auto getService() {
            auto service = "nebula::" + string(T::di_name);
            auto svc = _services[service][_di[service]];
            return (T*)construct(svc, service);
        }

        template <typename T, typename TVar>
        auto makeService(TVar vars) {
            auto service = "nebula::" + string(T::di_name);
            auto svc = new T(vars);
            _factoryCreated.emplace_back(svc);
            return (T*)construct(svc, service);
        }

    private:
        Injectable* construct(Injectable* svc, const string& service);
        Injectable* findService(const string& service);
    };
}
