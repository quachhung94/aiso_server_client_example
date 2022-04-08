#ifndef _SERVER_DEMO_H_
#define _SERVER_DEMO_H_

#include "IfRuntime.h"
#include "TLSServerAdapter.h"

#define IPV6_ADDR "::01"
#define IPV6_PORT 9090

namespace S {
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
}
#endif