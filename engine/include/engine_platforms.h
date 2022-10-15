#pragma once

#ifdef __linux__

    #define NEBULA_LINUX

#elif defined(__APPLE__)

    #include "TargetConditionals.h"

    #if TARGET_PHONE_SIMULATOR == 1

        #define NEBULA_IOS_SIMULATOR

    #elif TARGET_OS_IPHONE == 1

        #define NEBULA_IOS

    #elif TARGET_OS_MAC == 1

        #define NEBULA_MACOS

    #endif

#elif defined(_WIN64)

    #define NEBULA_WINDOWS

#elif defined(__ANDROID__)

    #define NEBULA_ANDROID

#endif

