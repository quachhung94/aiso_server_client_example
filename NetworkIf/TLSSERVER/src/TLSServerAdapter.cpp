#include "TLSServerAdapter.h"
#include "spdlog/spdlog.h"

TLSServerAdapter::TLSServerAdapter(const std::shared_ptr<CppCommon::Asio::Service>& service, const std::shared_ptr<CppCommon::Asio::SSLContext>& context, const std::string ip)
:SSLServer(service, context, ip, 9090)
{
  SPDLOG_DEBUG("Initial TLSServerAdapter");
}


TLSServerAdapter::~TLSServerAdapter()
{

}

bool TLSServerAdapter::start()
{
  SPDLOG_DEBUG("Initial TCPServerAdapter Start");
  return this->Start();
}

bool TLSServerAdapter::stop()
{
  return this->Stop();
}

bool TLSServerAdapter::restart()
{
  return this->Restart();
}

bool TLSServerAdapter::multicast(std::string_view text)
{
  return this->Multicast(text);
}

bool TLSServerAdapter::disconnectAll()
{
  return this->DisconnectAll();
}

void TLSServerAdapter::onConnected(std::shared_ptr<CppCommon::Asio::SSLSession>& session)
{
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TCP]: connected {0}-{1}",this->endpoint().address().to_string(), this->endpoint().port());
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TCP]: start listening on {0} {1}", this->address(), this->port());
  if (this->_onEventHandler[en_tlsServerAdapterCallbackType_CONNECT] != nullptr)
    (*this->_onEventHandler[en_tlsServerAdapterCallbackType_CONNECT])();
}

void TLSServerAdapter::onDisconnected(std::shared_ptr<CppCommon::Asio::SSLSession>& session)
{
    SPDLOG_DEBUG("[SUT_IF][ISO15118][TCP]: Id {} disconnect");
  if (this->_onEventHandler[en_tlsServerAdapterCallbackType_DISCONNECT] != nullptr)
    (*this->_onEventHandler[en_tlsServerAdapterCallbackType_DISCONNECT])();
}

void TLSServerAdapter::onError(int error, const std::string& category, const std::string& message)
{
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TCP]: on error {0}, {1}, {2}", error, category , message);
  if (this->_onEventHandler[en_tlsServerAdapterCallbackType_ERROR] != nullptr)
    (*this->_onEventHandler[en_tlsServerAdapterCallbackType_ERROR])(error, category, message);
}

void TLSServerAdapter::onReceived(std::shared_ptr<CppCommon::Asio::SSLSession>& session, const void* buffer, size_t size)
{
    SPDLOG_DEBUG("[SUT_IF][ISO15118][TCP]: receive: {}", (char*)buffer);
  
  if (this->_onEventHandler[en_tlsServerAdapterCallbackType_RECEIVE] != nullptr)
    (*this->_onEventHandler[en_tlsServerAdapterCallbackType_RECEIVE])(buffer, size);
}

void TLSServerAdapter::setEventCallBack(const std::shared_ptr<onTLSServerAdapterEventType>& eventHandler)
{
  this->_onEventHandler[eventHandler->type] = eventHandler;
}

void TLSServerAdapterSession::onConnected()
{
  SPDLOG_INFO("TCP session with Id {} CONNECTED", id().string());
}

void TLSServerAdapterSession::onDisconnected()
{
  SPDLOG_INFO("Chat TCP session with Id {} disconnected!", id().string());
}

void TLSServerAdapterSession::onSent(size_t sent, size_t pending)
{
  SPDLOG_INFO("Sent: {} bytes", sent);
}

void TLSServerAdapterSession::onReceived(const void* buffer, size_t size)
{
  std::string message((const char*)buffer, size);
  // SPDLOG_INFO("Incoming: {}", message);
  std::string res("This is RESPONSE from SERVER");

  // Multicast message to all connected sessions
  // server()->Multicast(message);

  // If the buffer starts with '!' the disconnect the current session
  // if (message == "!")
      // Disconnect();
  
  SendAsync(res);
}

void TLSServerAdapterSession::onError(int error, const std::string& category, const std::string& message)
{
    SPDLOG_DEBUG("[SUT_IF][ISO15118][TCP]: Chat TCP session on error {0}, {1}, {2}", error, category , message);
}
