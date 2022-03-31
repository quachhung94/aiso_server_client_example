#ifndef _TLSCLIENT_H_
#define _TLSCLIENT_H_

#include "IfRuntime.h"
#include "TLSAdapter.h"

class TLSClient;

class on15118TLSEvent: public onTLSAdapterEventType {
private:
  std::shared_ptr<TLSClient> parent;
public:
  on15118TLSEvent(en_tlsAdapterCallbackType _type, std::shared_ptr<TLSClient>& _parent)
  : onTLSAdapterEventType(_type) {parent = _parent;}
  // connect/disconnect/empty
  void operator() () override;
  // receive
  void operator() (const void *buffer, size_t size) override;
  // sent
  void operator() (size_t sent, size_t pending) override;
  // error
  void operator() (int error, const std::string& category, const std::string& message) override;
};

class TLSClient: public std::enable_shared_from_this<TLSClient>
{
private:
  std::shared_ptr<CppCommon::Asio::Service> _ptrService;
  // runtime
  std::shared_ptr<IfRuntime> _pRuntime;
  // ioService for port queue
  std::shared_ptr<asio::io_service> _ptrIOService;

  
  std::shared_ptr<CppCommon::Asio::SSLContext> _pSSLContextIf;

public:
  TLSClient(const std::shared_ptr<IfRuntime>& runtime, const std::string &ipv6_multicast, uint16_t port, const std::string& _ipv6IF);
  ~TLSClient();

  bool start();
  bool stop();
  void kill();

  // TCP Adapter
  std::shared_ptr<TLSAdapter> _pTLSIf;
  std::string ipv6IF;
  // event handler for TCP
  std::shared_ptr<on15118TLSEvent> _on15118TLSConnect;
  std::shared_ptr<on15118TLSEvent> _on15118TLSDisconnect;
  std::shared_ptr<on15118TLSEvent> _on15118TLSEmpty;
  std::shared_ptr<on15118TLSEvent> _on15118TLSReceive;
  std::shared_ptr<on15118TLSEvent> _on15118TLSSent;
  std::shared_ptr<on15118TLSEvent> _on15118TLSError;
};

#endif