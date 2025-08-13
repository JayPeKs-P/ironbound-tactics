//
// Created by Julian Kalb on 13/08/2025.
//

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks-inl.h"

namespace gl3::engine {
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;

    void Log::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_coreLogger = spdlog::stdout_color_mt("ENGINE");
        s_coreLogger->set_level(spdlog::level::trace);

        s_clientLogger = spdlog::stdout_color_mt("GAME");
        s_clientLogger->set_level(spdlog::level::trace);
    }
}
