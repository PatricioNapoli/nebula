#pragma once

#include "time/delta_timer.h"
#include "logging/logger.h"
#include "random/random.h"
#include "system/system_info.h"
#include "file/file_manager.h"
#include "concurrency/messaging.h"
#include "concurrency/thread_pool.h"
#include "concurrency/scheduler.h"
#include "window/window.h"
#include "input/input.h"
#include "config/configuration.h"
#include "camera/camera.h"
#include "time/fixed_timer.h"
#include "graphics/object_loader.h"
#include "graphics/opengl/opengl.h"
#include "graphics/shader_loader.h"
#include "graphics/texture_loader.h"

#define NEBULA_INJECT(baseType, type) _services[#baseType][#type] = new type();
#define NEBULA_INJECT_T(baseType, type, t) _services[#baseType][#type] = new type<t>();

#define NEBULA_INJECTION \
        NEBULA_INJECT(nebula::DeltaTimer, nebula::DeltaTimer) \
        NEBULA_INJECT(nebula::Logger, nebula::Logger) \
        NEBULA_INJECT(nebula::Random, nebula::Random) \
        NEBULA_INJECT(nebula::SystemInfo, nebula::SystemInfo) \
        NEBULA_INJECT(nebula::FileManager, nebula::FileManager) \
        NEBULA_INJECT(nebula::Messaging, nebula::Messaging) \
        NEBULA_INJECT(nebula::ThreadPool, nebula::ThreadPool) \
        NEBULA_INJECT(nebula::Window, nebula::Window) \
        NEBULA_INJECT(nebula::Input, nebula::Input) \
        NEBULA_INJECT(nebula::Configuration, nebula::Configuration) \
        NEBULA_INJECT(nebula::OpenGL, nebula::OpenGL) \
        NEBULA_INJECT(nebula::Camera, nebula::Camera) \
        NEBULA_INJECT(nebula::FixedTimer, nebula::FixedTimer) \
        NEBULA_INJECT(nebula::ObjectLoader, nebula::ObjectLoader) \
        NEBULA_INJECT(nebula::ShaderLoader, nebula::ShaderLoader) \
        NEBULA_INJECT(nebula::TextureLoader, nebula::TextureLoader) \
        NEBULA_INJECT(nebula::Scheduler, nebula::Scheduler)   \
        NEBULA_INJECT(nebula::ECS, nebula::ECS)
