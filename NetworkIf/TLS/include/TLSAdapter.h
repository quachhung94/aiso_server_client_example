#ifndef _TLSADAPTER_H_
#define _TLSADAPTER_H_

#include "asiowrapper/ssl_client.h"
#include "boost/function.hpp"
typedef enum {
  en_tlsAdapterCallbackType_CONNECT,
  en_tlsAdapterCallbackType_DISCONNECT,
  en_tlsAdapterCallbackType_RECEIVE,
  en_tlsAdapterCallbackType_SENT,
  en_tlsAdapterCallbackType_EMPTY,
  en_tlsAdapterCallbackType_ERROR,
  en_tlsAdapterCallbackType_NUM
} en_tlsAdapterCallbackType;
class onTLSAdapterEventType {
public:
  onTLSAdapterEventType(en_tlsAdapterCallbackType _type){
    type = _type;
  }
  onTLSAdapterEventType (onTLSAdapterEventType && ) = default;
  onTLSAdapterEventType (const onTLSAdapterEventType & ) = default;
  ~onTLSAdapterEventType(){}
  void setType(en_tlsAdapterCallbackType _type) {type = _type;}
  virtual void operator() () {}
  virtual void operator() (const void *buffer, size_t size) {}
  virtual void operator() (size_t sent, size_t pending) {}
  virtual void operator() (int error, const std::string& category, const std::string& message) {}
  onTLSAdapterEventType & operator= ( const onTLSAdapterEventType & ) = default;
  onTLSAdapterEventType & operator= ( onTLSAdapterEventType && ) = default;
  en_tlsAdapterCallbackType type;
};
class TLSAdapter : public CppCommon::Asio::SSLClient
{
public:
  TLSAdapter(const std::shared_ptr<CppCommon::Asio::Service>& service, const std::shared_ptr<CppCommon::Asio::SSLContext>& context);
  virtual ~TLSAdapter();
  bool setSUTAddress(const std::string& ipv6_address, int port, const std::string &channel);
  bool connect();
  bool connect(const std::shared_ptr<CppCommon::Asio::TCPResolver>& resolver);
  bool disconnect();
  size_t send(const void* buffer, size_t size);
  bool sendAsync(const void* buffer, size_t size);
  size_t receive(void* buffer, size_t size);
  void receiveAsync();
  // Component define callback function to handle TLS event
  // then push to TLSAdapter by setEventCallBack function
  void setEventCallBack(const std::shared_ptr<onTLSAdapterEventType>& eventHandler);
  void DisconnectAndStop()
  {
    _stop = true;
    DisconnectAsync();
    while (IsConnected())
      CppCommon::Thread::Yield();
  }

protected:
  void onConnected() override;
  void onDisconnected() override;
  void onReceived(const void *buffer, size_t size) override;
  void onSent(size_t sent, size_t pending) override;
  void onEmpty() override;
  void onError(int error, const std::string& category, const std::string& message) override;
  void onLogging(const std::string& message) override;

private:
  bool checkIpv6Address(const std::string& ipv6_address);
  bool _validSUTAddress;
  std::shared_ptr<onTLSAdapterEventType> _onEventHandler[en_tlsAdapterCallbackType_NUM];
  std::atomic<bool> _stop{false};
};

#endif