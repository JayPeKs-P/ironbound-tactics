//
// Created by Julian Kalb on 13/08/2025.
//

#pragma once
#include <memory>
#include "spdlog/spdlog.h"

namespace gl3::engine {
    class Log {
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger> &getCoreLogger() { return s_coreLogger; }
        inline static std::shared_ptr<spdlog::logger> &getClientLogger () { return s_clientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}


/// @brief Core log macros
#define CORE_TRACE(...) gl3::engine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  gl3::engine::Log::getCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  gl3::engine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) gl3::engine::Log::getCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) gl3::engine::Log::getCoreLogger()->fatal(__VA_ARGS__)

/// @brief Client log macros
#define TRACE(...)      gl3::engine::Log::getClientLogger()->trace(__VA_ARGS__)
#define INFO(...)       gl3::engine::Log::getClientLogger()->info(__VA_ARGS__)
#define WARN(...)       gl3::engine::Log::getClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)      gl3::engine::Log::getClientLogger()->error(__VA_ARGS__)
#define FATAL(...)      gl3::engine::Log::getClientLogger()->fatal(__VA_ARGS__)

#ifdef RELEASE
/// @brief Core log macros
#define CORE_TRACE
#define CORE_INFO
#define CORE_WARN
#define CORE_ERROR
#define CORE_FATAL

/// @brief Client log macros
#define TRACE
#define INFO
#define WARN
#define ERROR
#define FATAL(...)      gl3::engine::Log::getClientLogger()->fatal(__VA_ARGS__)
#endif