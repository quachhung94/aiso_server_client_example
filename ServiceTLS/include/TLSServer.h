#ifndef _DEMO_H_
#define _DEMO_H_

#include "IfRuntime.h"
#include "TLSServerAdapter.h"

#include "platform/core/Application.h"
#include "platform/core/task_priorities.h"
#include "platform/core/Task.h"
#include "platform/core/timer/Timer.h"
#include "platform/core/ipc/IEventListener.h"
#include "platform/core/ipc/Publisher.h"
#include "platform/core/ipc/SubscribingTaskEventQueue.h"
#include "platform/core/SystemStatistics.h"

#define IPV6_ADDR "::01"
#define IPV6_PORT 9090

class Demo;

class onTLSEvent: public onTLSServerAdapterEventType
{
private:
    std::shared_ptr<Demo> parent;
public:
    onTLSEvent(en_tlsServerAdapterCallbackType _type, std::shared_ptr<Demo>& _parent)
    : onTLSServerAdapterEventType(_type) {parent = _parent;}

    // connect, disconnect, empty
    void operator() () override;
    // reveive
    void operator() (const void *buffer, size_t size) override;
    // sent
    void operator() (size_t sent, size_t pending) override;
    // error
    void operator() (int error, const std::string& category, const std::string& message) override;
};


class Demo: public std::enable_shared_from_this<Demo>
{
private:
    // runtime
    std::shared_ptr<IfRuntime> _pRunTime;
    // ioService for port queue
    std::shared_ptr<asio::io_service> _ptrIOService;
    std::shared_ptr<CppCommon::Asio::Service> _ptrService;

public:
    std::string ipv6IF;

    // TLS Adapter
    std::shared_ptr<CppCommon::Asio::SSLContext> _pSSLContextIf;

    // TLSSERVER Adapter
    std::shared_ptr<TLSServerAdapter> _pTLSServerIf;

      // event handler for TCP
    std::shared_ptr<onTLSEvent> _onTLSConnect;
    std::shared_ptr<onTLSEvent> _onTLSDisconnect;
    std::shared_ptr<onTLSEvent> _onTLSEmpty;
    std::shared_ptr<onTLSEvent> _onTLSReceive;
    std::shared_ptr<onTLSEvent> _onTLSSent;
    std::shared_ptr<onTLSEvent> _onTLSError;


    Demo(const std::shared_ptr<IfRuntime>& runtime, const std::string &ipv6_multicast, uint16_t port, const std::string& _ipv6IF);
    ~Demo();

    bool start();
    bool stop();
    void kill();
    

    // void sendTCPMsgHandler(std::shared_ptr<V2gTpMessage> &item, std::shared_ptr<BasePort>& port);
    // void sendTLSMsgHandler(std::shared_ptr<V2gTpMessage> &item, std::shared_ptr<BasePort>& port);
    // void sendUDPMsgHandler(std::shared_ptr<V2gTpMessage> &item, std::shared_ptr<BasePort>& port);
    // void cmdHandler(std::shared_ptr<BaseOperation> &item, std::shared_ptr<BasePort>& port);
};

class ATask : public platform::core::Task {
public:
    ATask() : platform::core::Task("Other task", 9000, platform::core::APPLICATION_BASE_PRIO, std::chrono::milliseconds{30}) {}
    ~ATask() {}

    void tick() override; 
    void init() override;
};

class App : public platform::core::Application,
                public platform::core::ipc::IEventListener<platform::core::timer::TimerExpiredEvent>
{
public:
    using ExpiredQueue = platform::core::ipc::TaskEventQueue<platform::core::timer::TimerExpiredEvent>;
    App();

    void init() override;

    void tick() override;

    std::shared_ptr<ATask> a_instance;

    void event(const platform::core::timer::TimerExpiredEvent& event) override;

private:
    void create_timer(int id, std::chrono::milliseconds interval);
    struct TimerInfo
    {
        platform::core::timer::TimerOwner timer;
        std::chrono::milliseconds interval;
        std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
        int count = 0;
        std::chrono::milliseconds total = std::chrono::milliseconds(0);
    };
    std::shared_ptr<ExpiredQueue> m_timerEventQueue;
    std::vector<TimerInfo> timers;
    
    std::shared_ptr<IfRuntime> runenv;
    std::shared_ptr<Demo> demo;
};

#endif