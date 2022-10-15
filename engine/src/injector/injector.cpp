#include "injector/injector.h"

#include <algorithm>
#include <utility>
#include <profiling/profiler.h>

#include "injector/injection.h"
#include "injector/di.h"
#include "exception/engine_exception.h"
#include "time/stopwatch.h"

namespace nebula {
    Injector::Injector(EnvironmentVars environment) : _environment(std::move(environment)) {
        // TODO: Json/YML
        _di = NEBULA_DI;
        NEBULA_INJECTION
    }

    Injector::~Injector() {
        for (auto const& service : _services) {
            for (auto const& implementation : service.second) {

                if (service.first != "nebula::" + string(ThreadPool::di_name))
                    delete implementation.second;
            }
        }

        for (auto const& service : _factoryCreated)
            delete service;
    }

    Injectable* Injector::construct(Injectable* svc, const string& service) {
        if (svc == nullptr)
            throw EngineException("Injectable service not found in DI configuration.", __FILE__, EngineException::Fatal);

        if (svc->_constructed)
            return svc;

        NEBULA_PROFILE_D(service);

        svc->mapDependencies(_environment);

        for (auto& entry : svc->_dependencies) {
            auto dependencyDeps = _services[entry.first][_di[entry.first]]->_dependencies;

            if (dependencyDeps.find(service) != dependencyDeps.end())
                throw EngineException("Encountered circular dependency in injector while constructing: " + entry.first + " from root: " + service, __FILE__, EngineException::Fatal);

            // Check if it's a factory
            if (*(entry.second) != nullptr)
                construct(*(entry.second), entry.first);
            else
                *(entry.second) = construct(findService(entry.first), entry.first);
        }

        svc->_constructed = true;
        svc->onConstruct();

        return svc;
    }

    Injectable* Injector::findService(const string& service) {
        return _services[service][_di[service]];
    }
}
