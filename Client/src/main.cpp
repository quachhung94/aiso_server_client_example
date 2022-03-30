#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "client.h"

#define LOG_TAG "ClientCpp"
#define	LOG_USER	(1<<3)	/* random user-level messages */
#define IPV6_ADDR "::1"
#define IPV6_PORT 9090

static void Spdlog_Init()
{
    constexpr auto spdLogPath = "client_demo.txt";                        // Store absolute path of log files
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

    SPDLOG_INFO("**************** START Client DEMO APP **************");
    SPDLOG_INFO("Release date: {}, release time {}", __DATE__, __TIME__);

    auto runenv = std::make_shared<IfRuntime>();
    auto client = std::make_shared<Client>(runenv, IPV6_ADDR, IPV6_PORT, "lo");
    client->start();

   // Connect the client
    std::cout << "Client connecting..." << std::endl;
    client->_pTCPIf->connect();
    std::cout << "Done!" << std::endl;

    std::cout << "Press Enter to stop the client or '!' to reconnect the client..." << std::endl;


    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line.empty())
            break;

        // Reconnect the client
        if (line == "!")
        {
            std::cout << "Client reconnecting..." << std::endl;
            // clientIsConnected() ? client->ReconnectAsync() : client->ConnectAsync();
            if (client->_pTCPIf->IsConnected())
                client->_pTCPIf->ReceiveAsync();
            else
                client->_pTCPIf->ConnectAsync();      
            
            std::cout << "Done!" << std::endl;
            continue;
        }

        // Send the entered text to the chat server
        client->_pTCPIf->SendAsync(line);
    }

    // Disconnect the client
    std::cout << "Client disconnecting..." << std::endl;
    client->_pTCPIf->DisconnectAndStop();
    std::cout << "Done!" << std::endl;

    return 0;
}