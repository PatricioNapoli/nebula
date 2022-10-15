#pragma once

#if defined(__GNUC__) || defined(__clang__)
    #define NEB_FUNC_SIG __PRETTY_FUNCTION__
    #define NEB_FUNC_SIG_PREFIX '='
    #define NEB_FUNC_SIG_SUFFIX ']'
#elif defined(_MSC_VER)
    #define NEB_FUNC_SIG __FUNCSIG__
    #define NEB_FUNC_SIG_PREFIX '='
    #define NEB_FUNC_SIG_SUFFIX ']'
#endif

#define NEBULA_ASSERT(condition, message) \
do { \
    if (! (condition)) { \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                  << " line " << __LINE__ << ":" << NEB_FUNC_SIG << " : " << (message) << std::endl; \
        std::terminate(); \
    } \
} while (false)
