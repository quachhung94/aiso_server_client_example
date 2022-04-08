#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "TLSServer.h"

#define LOG_TAG "DemoCpp"
#define	LOG_USER	(1<<3)	/* random user-level messages */


static void Spdlog_Init()
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
    auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_st>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern("[%D %H:%M:%S.%e][PID:%P][%t][%^%L%$][%s:%#]->%!(): %v");
    /* Sys sink */
    // auto sys_sink = std::make_shared<spdlog::sinks::syslog_sink_st>(LOG_TAG, 0, LOG_USER, true);
    // sys_sink->set_level(spdlog::level::info);
    // sys_sink->set_pattern("[%P][%s:%#]: %v");
    
    sinks.push_back(file_sink);
    sinks.push_back(console_sink);
    // sinks.push_back(sys_sink);

    auto logger = std::make_shared<spdlog::logger>(LOG_TAG, begin(sinks), end(sinks));
    logger->set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::debug);
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
}

int main(int argc, char **argv) {
    Spdlog_Init();

    SPDLOG_INFO("**************** START DEMO APP **************");
    SPDLOG_INFO("Release date: {}, release time {}", __DATE__, __TIME__);

    auto runenv = std::make_shared<IfRuntime>();
    auto demo = std::make_shared<Demo>(runenv, IPV6_ADDR, IPV6_PORT, "enp59s0");
    demo->start();
    // platform::core::SystemStatistics::instance().dump();

    // auto MainApp = std::make_shared<App>();
    // MainApp->start();

    // Perform text input
    // std::string line;
    // while (getline(std::cin, line))
    // {
    //     if (line.empty())
    //         break;

    //     // Restart the server
    //     if (line == "!")
    //     {
    //         std::cout << "Server restarting...";
    //         demo->_pTCPServerIf->restart();
    //         std::cout << "Done!" << std::endl;
    //         continue;
    //     }

    //     // Multicast admin message to all sessions
    //     line = "(admin) " + line;
    //     demo->_pTCPServerIf->multicast(line);
    // }

    // Stop the server
    std::cout << "Server stopping...";
    std::cout << "Done!" << std::endl;

    return 0;
}