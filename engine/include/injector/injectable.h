#pragma once

#include <map>
#include <utility>
#include <vector>
#include <any>
#include <unordered_map>

#include "memory/containers.h"

#define NEBULA_INJECTABLE(n) \
        public: \
        static constexpr const char* di_name = #n; \
        protected: \
            void mapDependencies(EnvironmentVars& globalEnv) override; \
            void onConstruct() override

#define NEBULA_INJECTABLE_E(n) \
        public: \
        static constexpr const char* di_name = #n; \
        protected: \
            void mapDependencies(EnvironmentVars& globalEnv) override {} \
            void onConstruct() override {}

typedef nebula::hmap<nebula::string, std::any> EnvironmentVars;

namespace nebula {
    class Injectable {
        friend class Injector;

        hmap<string, Injectable**> _dependencies;
        bool _constructed{false};
    public:
        virtual ~Injectable() = default;

    protected:
        virtual void mapDependencies(EnvironmentVars& globalEnv) {}
        virtual void onConstruct() {}

        template <typename T>
        void map(T** svc) {
            auto service = "nebula::" + string(T::di_name);
            _dependencies[service] = (Injectable**)svc;
        }

        template <typename T, typename TVar>
        void mapFactory(T** svc, TVar vars) {
            auto service = "nebula::" + string(T::di_name);
            _dependencies[service] = (Injectable**)svc;
            *(svc) = new T(vars); // Later deleted by Injector
        }
    };
}
