#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "IfRuntime.h"
#include "TCPAdapter.h"
#include "TLSAdapter.h"
#include "EVCCUdpClient.h"
#include "TCPServerAdapter.h"

class Client;

class on15118TCPEvent: public onTCPAdapterEventType {
private:
  std::shared_ptr<Client> parent;
public:
  on15118TCPEvent(en_tcpAdapterCallbackType _type, std::shared_ptr<Client>& _parent)
  : onTCPAdapterEventType(_type) {parent = _parent;}
  // connect/disconnect/empty
  void operator() () override;
  // receive
  void operator() (const void *buffer, size_t size) override;
  // sent
  void operator() (size_t sent, size_t pending) override;
  // error
  void operator() (int error, const std::string& category, const std::string& message) override;
};

class Client: public std::enable_shared_from_this<Client>
{
private:
    std::shared_ptr<CppCommon::Asio::Service> _ptrService;
    // runtime
    std::shared_ptr<IfRuntime> _pRuntime;
    // ioService for port queue
    std::shared_ptr<asio::io_service> _ptrIOService;

    
    std::shared_ptr<CppCommon::Asio::SSLContext> _pSSLContextIf;
    std::shared_ptr<CppCommon::Asio::TCPResolver> _pTCPResolverIf;

public:
    Client(const std::shared_ptr<IfRuntime>& runtime, const std::string &ipv6_multicast, uint16_t port, const std::string& _ipv6IF);
    ~Client();

    bool start();
    bool stop();
    void kill();

    // TCP Adapter
    std::shared_ptr<TCPAdapter> _pTCPIf;
    std::string ipv6IF;
    // event handler for TCP
    std::shared_ptr<on15118TCPEvent> _on15118TcpConnect;
    std::shared_ptr<on15118TCPEvent> _on15118TcpDisconnect;
    std::shared_ptr<on15118TCPEvent> _on15118TcpEmpty;
    std::shared_ptr<on15118TCPEvent> _on15118TcpReceive;
    std::shared_ptr<on15118TCPEvent> _on15118TcpSent;
    std::shared_ptr<on15118TCPEvent> _on15118TcpError;
};

#endif