#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "starter_example.h"
#include "platform/core/SystemStatistics.h"
#include "platform/core/Task.h"
#include "platform/core/ipc/IEventListener.h"
#include "platform/core/ipc/Publisher.h"
#include "platform/core/ipc/SubscribingTaskEventQueue.h"
#include "platform/core/task_priorities.h"
#include "platform/config_constants.h"

#include <fstream>
#include <iostream>
#include <random>

// #include <json/json.h>
// #include <json/reader.h>
#define LOG_TAG "App"

static void spdlog_init()
{   
    constexpr auto spdLogPath = "tls_demo.txt";                        // Store absolute path of log files
    constexpr auto spdNumberLogFile = 3;                                            // Max number of log files
    constexpr auto spdLogFileSize = 4194304;                                        // Max size of each log file
    std::vector<spdlog::sink_ptr> sinks;
    /* File sink */
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_st>(spdLogPath, spdLogFileSize, spdNumberLogFile);
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern("[%D %H:%M:%S.%e][PID:%P][%t][%^%L%$][%s:%#]->%!(): %v");
    /* Stdout sink */
    // auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_st>();
    // console_sink->set_level(spdlog::level::debug);
    // console_sink->set_pattern("[%D %H:%M:%S.%e][PID:%P][%t][%^%L%$][%s:%#]->%!(): %v");
    /* Sys sink */
    // auto sys_sink = std::make_shared<spdlog::sinks::syslog_sink_st>(LOG_TAG, 0, LOG_USER, true);
    // sys_sink->set_level(spdlog::level::info);
    // sys_sink->set_pattern("[%P][%s:%#]: %v");
    
    sinks.push_back(file_sink);
    // sinks.push_back(console_sink);
    // sinks.push_back(sys_sink);

    auto logger = std::make_shared<spdlog::logger>(LOG_TAG, begin(sinks), end(sinks));
    logger->set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::debug);
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
}

int main()
{
    spdlog_init();
    SPDLOG_INFO("Hello, {}", kLogName);

    
    platform::core::SystemStatistics::instance().dump();

    auto demo = std::make_shared<starter_example::App>();

    demo->start();
    
    return 0;
}