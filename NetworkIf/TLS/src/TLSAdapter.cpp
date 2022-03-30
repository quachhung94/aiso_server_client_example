#include "TLSAdapter.h"
#include "spdlog/spdlog.h"
TLSAdapter::TLSAdapter(const std::shared_ptr<CppCommon::Asio::Service>& service, const std::shared_ptr<CppCommon::Asio::SSLContext>& context)
  : SSLClient(service, context, "", 0) {
  for (int i = 0; i < en_tlsAdapterCallbackType_NUM; i++) {
    this->_onEventHandler[i] = nullptr;
  }
}

TLSAdapter::~TLSAdapter() {
}

bool TLSAdapter::checkIpv6Address(const std::string& ipv6_address) {
  asio::error_code ec;
  asio::ip::make_address_v6(ipv6_address, ec);
  if(ec){
    this->_validSUTAddress = false;
    return false;
  }
  else {
    this->_validSUTAddress = true;
    return true;
  }
}

bool TLSAdapter::setSUTAddress(const std::string& ipv6_address, int port, const std::string &channel){
  // update SUTAddress ipv6 port|endpoint
  std::string ipv6 = fmt::format("{0}%{1}",ipv6_address, channel);
  if (this->checkIpv6Address(ipv6) && (0 <= port) && (65536 > port)) {
    // this->_port = port;
    // this->_address = ipv6;
    this->setAddressAndPort(ipv6, port);
    return true;
  }
  else {
    SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: Input ipv6 {0} port {1} is invalid",ipv6, port);
    return false;
  }
}

bool TLSAdapter::connect() {
  // if SUTAddress is configured and valid then call TCPClient.Connect
  if (this->_validSUTAddress) {
    this->SetupKeepAlive(true);
    if (true == this->ConnectAsync())
    {
      SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: try connected {0}-{1}",this->address(), this->port());
      return true;
    }
    else
    {
      SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: already connected or is processing {0}-{1}",this->address(), this->port());
      return false;
    }
  }
  else {
    return false;
  }
}

bool TLSAdapter::connect(const std::shared_ptr<CppCommon::Asio::TCPResolver>& resolver)
{
  if (this->_validSUTAddress) {
    this->SetupKeepAlive(true);
    if (true == this->ConnectAsync(resolver))
    {
      SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: try connected {0}-{1}",this->address(), this->port());
      return true;
    }
    else
    {
      SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: already connected or is processing {0}-{1}",this->address(), this->port());
      return false;
    }
  }
  else {
    return false;
  }
}

bool TLSAdapter::disconnect() {
  return this->DisconnectAsync();
}

size_t TLSAdapter::send(const void* buffer, size_t size){
  // call Send of TCPClient
  // no additional logic
  std::string data_array;
  for (size_t i = 0; i < size; i++)
  {
    data_array.append(fmt::format("{:02X} ", ((char*)buffer)[i]));
  }
  // Logging::debug(LogSutTCP_ENABLE, fmt::format("[SUT_IF][ISO15118][TCP]: send: {0} {1} | {2} | size {3}",this->endpoint().address().to_string(), this->endpoint().port(), data_array, size));
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: send: {}",data_array);
  
  return this->Send(buffer, size, CppCommon::Timespan(std::chrono::seconds(60)));
}

// Component call sendAsync
bool TLSAdapter::sendAsync(const void* buffer, size_t size){
  std::string data_array;
  for (size_t i = 0; i < size; i++)
  {
    data_array.append(fmt::format("{:02X} ", ((char*)buffer)[i]));
  }
  // Logging::debug(LogSutTCP_ENABLE, fmt::format("[SUT_IF][ISO15118][TCP]: send: {0} {1} | {2} | size {3}",this->endpoint().address().to_string(), this->endpoint().port(), data_array, size));
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: send: {}",data_array);
  
  return this->SendAsync(buffer, size);
}

size_t TLSAdapter::receive(void* buffer, size_t size){
  return this->Receive(buffer, size, CppCommon::Timespan(std::chrono::seconds(60)));
}

// call receive async
void TLSAdapter::receiveAsync(){
  this->ReceiveAsync();
}

void TLSAdapter::setEventCallBack(const std::shared_ptr<onTLSAdapterEventType>& eventHandler) {
  this->_onEventHandler[eventHandler->type] = eventHandler;
}
void TLSAdapter::onConnected() {
  // call function object configured by Component setup
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: connected {0}-{1}",this->endpoint().address().to_string(), this->endpoint().port());
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: start listening on {0} {1}", this->socket().local_endpoint().address().to_string(), this->socket().local_endpoint().port());
  if (this->_onEventHandler[en_tlsAdapterCallbackType_CONNECT] != nullptr)
    (*this->_onEventHandler[en_tlsAdapterCallbackType_CONNECT])();
}

void TLSAdapter::onDisconnected() {
  // call function object configured by Component setup
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: disconnect");
  if (this->_onEventHandler[en_tlsAdapterCallbackType_DISCONNECT] != nullptr)
    (*this->_onEventHandler[en_tlsAdapterCallbackType_DISCONNECT])();
}

void TLSAdapter::onReceived(const void *buffer, size_t size) {
  // call function object configured by Component setup
  // TBD: should process size expected to receive before pass to callback
  std::string data_array;
  for (size_t i = 0; i < size; i++)
  {
    data_array.append(fmt::format("{:02X} ", ((char*)buffer)[i]));
  }
  // Logging::debug(LogSutTCP_ENABLE, fmt::format("[SUT_IF][ISO15118][TCP]: receive: {0} {1} | {2} | size {3}",this->endpoint().address().to_string(), this->endpoint().port(), data_array, size));
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: receive: {}",data_array);
  
  if (this->_onEventHandler[en_tlsAdapterCallbackType_RECEIVE] != nullptr)
    (*this->_onEventHandler[en_tlsAdapterCallbackType_RECEIVE])(buffer, size);
}

void TLSAdapter::onSent(size_t sent, size_t pending) {
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: send: {0} | pending: {1}", sent, pending);
  if (this->_onEventHandler[en_tlsAdapterCallbackType_SENT] != nullptr)
    (*this->_onEventHandler[en_tlsAdapterCallbackType_SENT])(sent, pending);
}

void TLSAdapter::onEmpty() {
  if (this->_onEventHandler[en_tlsAdapterCallbackType_EMPTY] != nullptr)
    (*this->_onEventHandler[en_tlsAdapterCallbackType_EMPTY])();
}

void TLSAdapter::onError(int error, const std::string& category, const std::string& message) {
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: on error {0}, {1}, {2}", error, category , message);
  if (this->_onEventHandler[en_tlsAdapterCallbackType_ERROR] != nullptr)
    (*this->_onEventHandler[en_tlsAdapterCallbackType_ERROR])(error, category, message);
}

void TLSAdapter::onLogging(const std::string& message) {
  SPDLOG_DEBUG("[SUT_IF][ISO15118][TLS]: on logging -> {0}", message);
}