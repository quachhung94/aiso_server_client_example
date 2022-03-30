#ifndef _TCPSERVERADAPTER_H_
#define _TCPSERVERADAPTER_H_

#include "asiowrapper/tcp_server.h"
#include "boost/function.hpp"
#include "spdlog/spdlog.h"

typedef enum {
  en_tcpServerAdapterCallbackType_CONNECT,
  en_tcpServerAdapterCallbackType_DISCONNECT,
  en_tcpServerAdapterCallbackType_RECEIVE,
  en_tcpServerAdapterCallbackType_SENT,
  en_tcpServerAdapterCallbackType_EMPTY,
  en_tcpServerAdapterCallbackType_ERROR,
  en_tcpServerAdapterCallbackType_NUM
} en_tcpServerAdapterCallbackType;

class onTCPServerAdapterEventType {
public:
  onTCPServerAdapterEventType(en_tcpServerAdapterCallbackType _type){
    type = _type;
  }
  onTCPServerAdapterEventType (onTCPServerAdapterEventType && ) = default;
  onTCPServerAdapterEventType (const onTCPServerAdapterEventType & ) = default;
  ~onTCPServerAdapterEventType(){}
  void setType(en_tcpServerAdapterCallbackType _type) {type = _type;}
  virtual void operator() () {}
  virtual void operator() (const void *buffer, size_t size) {}
  virtual void operator() (size_t sent, size_t pending) {}
  virtual void operator() (int error, const std::string& category, const std::string& message) {}
  onTCPServerAdapterEventType & operator= ( const onTCPServerAdapterEventType & ) = default;
  onTCPServerAdapterEventType & operator= ( onTCPServerAdapterEventType && ) = default;
  en_tcpServerAdapterCallbackType type;
};

class TCPServerAdapterSession: public CppCommon::Asio::TCPSession
{
public:
  using CppCommon::Asio::TCPSession::TCPSession;

  void onConnected() override;
  void onDisconnected() override;
  void onReceived(const void* buffer, size_t size) override;
  void onSent(size_t sent, size_t pending) override;
  void onError(int error, const std::string& category, const std::string& message) override;
};

class TCPServerAdapter : public CppCommon::Asio::TCPServer
{
private:
  std::shared_ptr<onTCPServerAdapterEventType> _onEventHandler[en_tcpServerAdapterCallbackType_NUM];
public:
  TCPServerAdapter(const std::shared_ptr<CppCommon::Asio::Service>& service, const std::string ip);
  
  virtual ~TCPServerAdapter();

  bool start();
  bool stop();
  bool restart();
  bool disconnectAll();
  bool multicast(std::string_view text);
  void setEventCallBack(const std::shared_ptr<onTCPServerAdapterEventType>& eventHandler);
  
protected:
  std::shared_ptr<CppCommon::Asio::TCPSession> CreateSession(const std::shared_ptr<CppCommon::Asio::TCPServer>& server) override
  {
    return  std::make_shared<TCPServerAdapterSession>(server);
  }
  void onConnected(std::shared_ptr<CppCommon::Asio::TCPSession>& session) override;
  void onDisconnected(std::shared_ptr<CppCommon::Asio::TCPSession>& session) override;
  void onError(int error, const std::string& category, const std::string& message) override;
  void onReceived(std::shared_ptr<CppCommon::Asio::TCPSession>& session, const void* buffer, size_t size) override;
};

#endif