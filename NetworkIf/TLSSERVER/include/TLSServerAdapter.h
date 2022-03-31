#ifndef _TLSSERVERADAPTER_H_
#define _TLSSERVERADAPTER_H_

#include "asiowrapper/ssl_server.h"
#include "boost/function.hpp"
#include "spdlog/spdlog.h"

typedef enum {
  en_tlsServerAdapterCallbackType_CONNECT,
  en_tlsServerAdapterCallbackType_DISCONNECT,
  en_tlsServerAdapterCallbackType_RECEIVE,
  en_tlsServerAdapterCallbackType_SENT,
  en_tlsServerAdapterCallbackType_EMPTY,
  en_tlsServerAdapterCallbackType_ERROR,
  en_tlsServerAdapterCallbackType_NUM
} en_tlsServerAdapterCallbackType;

class onTLSServerAdapterEventType {
public:
  onTLSServerAdapterEventType(en_tlsServerAdapterCallbackType _type){
    type = _type;
  }
  onTLSServerAdapterEventType (onTLSServerAdapterEventType && ) = default;
  onTLSServerAdapterEventType (const onTLSServerAdapterEventType & ) = default;
  ~onTLSServerAdapterEventType(){}
  void setType(en_tlsServerAdapterCallbackType _type) {type = _type;}
  virtual void operator() () {}
  virtual void operator() (const void *buffer, size_t size) {}
  virtual void operator() (size_t sent, size_t pending) {}
  virtual void operator() (int error, const std::string& category, const std::string& message) {}
  onTLSServerAdapterEventType & operator= ( const onTLSServerAdapterEventType & ) = default;
  onTLSServerAdapterEventType & operator= ( onTLSServerAdapterEventType && ) = default;
  en_tlsServerAdapterCallbackType type;
};

class TLSServerAdapterSession: public CppCommon::Asio::SSLSession
{
public:
  using CppCommon::Asio::SSLSession::SSLSession;

  void onConnected() override;
  void onDisconnected() override;
  void onReceived(const void* buffer, size_t size) override;
  void onSent(size_t sent, size_t pending) override;
  void onError(int error, const std::string& category, const std::string& message) override;
};

class TLSServerAdapter : public CppCommon::Asio::SSLServer
{
private:
  std::shared_ptr<onTLSServerAdapterEventType> _onEventHandler[en_tlsServerAdapterCallbackType_NUM];
public:
  TLSServerAdapter(const std::shared_ptr<CppCommon::Asio::Service>& service, const std::shared_ptr<CppCommon::Asio::SSLContext>& context, const std::string ip);
  
  virtual ~TLSServerAdapter();

  bool start();
  bool stop();
  bool restart();
  bool disconnectAll();
  bool multicast(std::string_view text);
  void setEventCallBack(const std::shared_ptr<onTLSServerAdapterEventType>& eventHandler);
  
protected:
  std::shared_ptr<CppCommon::Asio::SSLSession> CreateSession(const std::shared_ptr<CppCommon::Asio::SSLServer>& server) override
  {
    return  std::make_shared<TLSServerAdapterSession>(server);
  }
  void onConnected(std::shared_ptr<CppCommon::Asio::SSLSession>& session) override;
  void onDisconnected(std::shared_ptr<CppCommon::Asio::SSLSession>& session) override;
  void onError(int error, const std::string& category, const std::string& message) override;
  void onReceived(std::shared_ptr<CppCommon::Asio::SSLSession>& session, const void* buffer, size_t size) override;
};

#endif