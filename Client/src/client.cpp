#include "client.h"
#include "spdlog/spdlog.h"

Client::Client(const std::shared_ptr<IfRuntime>& runtime, const std::string &ipv6_multicast, uint16_t port, const std::string& _ipv6IF)
{
    // get CppCommon::Asio::Service
    this->_pRuntime = runtime;
    this->_ptrService = this->_pRuntime->getService();
    // get the working io_service(bound to another thread) for queue operation
    this->_ptrIOService = this->_ptrService->GetAsioService();
    this->_pTCPIf = std::make_shared<TCPAdapter>(this->_ptrService);
    this->ipv6IF = _ipv6IF;
    this->_pTCPIf->setSUTAddress(ipv6_multicast, port, this->ipv6IF);
}

Client::~Client()
{
    this->_pTCPIf.reset();
}

bool Client::start()
{
    auto self(this->shared_from_this());
    // create TCP handler object
    this->_on15118TcpConnect = std::make_shared<on15118TCPEvent>(en_tcpAdapterCallbackType_CONNECT, self);
    this->_on15118TcpDisconnect = std::make_shared<on15118TCPEvent>(en_tcpAdapterCallbackType_DISCONNECT, self);
    this->_on15118TcpEmpty = std::make_shared<on15118TCPEvent>(en_tcpAdapterCallbackType_EMPTY, self);
    this->_on15118TcpReceive = std::make_shared<on15118TCPEvent>(en_tcpAdapterCallbackType_RECEIVE, self);
    this->_on15118TcpSent = std::make_shared<on15118TCPEvent>(en_tcpAdapterCallbackType_SENT, self);
    this->_on15118TcpError = std::make_shared<on15118TCPEvent>(en_tcpAdapterCallbackType_ERROR, self);
    // add TCP handler object to TCP interface
    this->_pTCPIf->setEventCallBack(this->_on15118TcpConnect);
    this->_pTCPIf->setEventCallBack(this->_on15118TcpDisconnect);
    this->_pTCPIf->setEventCallBack(this->_on15118TcpEmpty);
    this->_pTCPIf->setEventCallBack(this->_on15118TcpReceive);
    this->_pTCPIf->setEventCallBack(this->_on15118TcpSent);
    this->_pTCPIf->setEventCallBack(this->_on15118TcpError);

    return true;
}

// event for connect/disconnect TCP
void on15118TCPEvent::operator()() {
  switch (this->type){
    case en_tcpAdapterCallbackType_CONNECT: {
      // push event to event queue
      this->parent->_pTCPIf->receiveAsync();
      SPDLOG_INFO("[STC]: SUT TCP {0} - {1} connected",this->parent->_pTCPIf->address(), this->parent->_pTCPIf->port());
      break;
    }
    case en_tcpAdapterCallbackType_DISCONNECT: {
      SPDLOG_INFO("[STC]: SUT TCP {0} - {1} disconnected",this->parent->_pTCPIf->address(), this->parent->_pTCPIf->port());
      break;
    }
    case en_tcpAdapterCallbackType_EMPTY: {
      SPDLOG_INFO("Empty event TCP");
      break;
    }
    default:
      break;
  }
}

// TCP receive event receive handler
void on15118TCPEvent::operator()(const void *buffer, size_t size) {
  SPDLOG_INFO("[STC]: TCP receive {} event receive handler: {}", size, ((char*)buffer));
}

// TCP interface sent event handler
void on15118TCPEvent::operator()(size_t sent, size_t pending) {
  SPDLOG_INFO("[STC]: TCP interface sent event handler");
}

// TCP interface error event handler
void on15118TCPEvent::operator()(int error, const std::string& category, const std::string& message) {
  // process tcp error
  // connected error/ disconnected error/ send error/ receive error/ connection abort ?
  SPDLOG_INFO("[STC]: SUT TCP Error {0} - {1} disconnected", category, message);
}
