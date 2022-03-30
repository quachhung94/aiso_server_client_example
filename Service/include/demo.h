#ifndef _DEMO_H_
#define _DEMO_H_

#include "IfRuntime.h"
#include "TCPAdapter.h"
#include "TLSAdapter.h"
#include "EVCCUdpClient.h"
#include "TCPServerAdapter.h"

class Demo;

class onTCPEvent: public onTCPServerAdapterEventType
{
private:
    std::shared_ptr<Demo> parent;
public:
    onTCPEvent(en_tcpServerAdapterCallbackType _type, std::shared_ptr<Demo>& _parent)
    : onTCPServerAdapterEventType(_type) {parent = _parent;}

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

    // TCP Adapter
    std::shared_ptr<TCPAdapter> _pTCPIf;
    // TLS Adapter
    std::shared_ptr<TLSAdapter> _pTLSIf;
    std::shared_ptr<CppCommon::Asio::SSLContext> _pSSLContextIf;
    std::shared_ptr<CppCommon::Asio::TCPResolver> _pTCPResolverIf;
    // UDP Adapter
    std::shared_ptr<EVCCUdpClient> _pUDPIf;

    // TCPSERVER Adapter
    std::shared_ptr<TCPServerAdapter> _pTCPServerIf;
    std::shared_ptr<TCPServerAdapterSession> _pTCPCurSession;

      // event handler for TCP
    std::shared_ptr<onTCPEvent> _onTcpConnect;
    std::shared_ptr<onTCPEvent> _onTcpDisconnect;
    std::shared_ptr<onTCPEvent> _onTcpEmpty;
    std::shared_ptr<onTCPEvent> _onTcpReceive;
    std::shared_ptr<onTCPEvent> _onTcpSent;
    std::shared_ptr<onTCPEvent> _onTcpError;


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


#endif