#include "TLSClient.h"
#include "spdlog/spdlog.h"

TLSClient::TLSClient(const std::shared_ptr<IfRuntime>& runtime, const std::string &ipv6_multicast, uint16_t port, const std::string& _ipv6IF)
{
    // get CppCommon::Asio::Service
    this->_pRuntime = runtime;
    this->_ptrService = this->_pRuntime->getService();
    // get the working io_service(bound to another thread) for queue operation
    this->_ptrIOService = this->_ptrService->GetAsioService();
    this->_pSSLContextIf = std::make_shared<CppCommon::Asio::SSLContext>(asio::ssl::context::tlsv12);
    this->_pSSLContextIf->set_default_verify_paths();
    this->_pSSLContextIf->set_root_certs();
    this->_pSSLContextIf->set_verify_mode(asio::ssl::verify_peer | asio::ssl::verify_fail_if_no_peer_cert);
    this->_pSSLContextIf->load_verify_file("../../tools/certificates/ca.pem");

    this->_pTLSIf = std::make_shared<TLSAdapter>(this->_ptrService, this->_pSSLContextIf);
    this->ipv6IF = _ipv6IF;
    this->_pTLSIf->setSUTAddress(ipv6_multicast, port, this->ipv6IF);
}

TLSClient::~TLSClient()
{
  _pSSLContextIf.reset();
  this->_pTLSIf.reset();
}

bool TLSClient::start()
{
    auto self(this->shared_from_this());
    // create TCP handler object
    this->_on15118TLSConnect = std::make_shared<on15118TLSEvent>(en_tlsAdapterCallbackType_CONNECT, self);
    this->_on15118TLSDisconnect = std::make_shared<on15118TLSEvent>(en_tlsAdapterCallbackType_DISCONNECT, self);
    this->_on15118TLSEmpty = std::make_shared<on15118TLSEvent>(en_tlsAdapterCallbackType_EMPTY, self);
    this->_on15118TLSReceive = std::make_shared<on15118TLSEvent>(en_tlsAdapterCallbackType_RECEIVE, self);
    this->_on15118TLSSent = std::make_shared<on15118TLSEvent>(en_tlsAdapterCallbackType_SENT, self);
    this->_on15118TLSError = std::make_shared<on15118TLSEvent>(en_tlsAdapterCallbackType_ERROR, self);
    // add TCP handler object to TCP interface
    this->_pTLSIf->setEventCallBack(this->_on15118TLSConnect);
    this->_pTLSIf->setEventCallBack(this->_on15118TLSDisconnect);
    this->_pTLSIf->setEventCallBack(this->_on15118TLSEmpty);
    this->_pTLSIf->setEventCallBack(this->_on15118TLSReceive);
    this->_pTLSIf->setEventCallBack(this->_on15118TLSSent);
    this->_pTLSIf->setEventCallBack(this->_on15118TLSError);

    return true;
}

// event for connect/disconnect TCP
void on15118TLSEvent::operator()() {
  switch (this->type){
    case en_tlsAdapterCallbackType_CONNECT: {
      // push event to event queue
      this->parent->_pTLSIf->receiveAsync();
      SPDLOG_INFO("[STC]: SUT TCP {0} - {1} connected",this->parent->_pTLSIf->address(), this->parent->_pTLSIf->port());
      break;
    }
    case en_tlsAdapterCallbackType_DISCONNECT: {
      SPDLOG_INFO("[STC]: SUT TCP {0} - {1} disconnected",this->parent->_pTLSIf->address(), this->parent->_pTLSIf->port());
      break;
    }
    case en_tlsAdapterCallbackType_EMPTY: {
      SPDLOG_INFO("Empty event TCP");
      break;
    }
    default:
      break;
  }
}

// TCP receive event receive handler
void on15118TLSEvent::operator()(const void *buffer, size_t size) {
  SPDLOG_INFO("[STC]: TCP receive {} event receive handler: {}", size, ((char*)buffer));
}

// TCP interface sent event handler
void on15118TLSEvent::operator()(size_t sent, size_t pending) {
  SPDLOG_INFO("[STC]: TCP interface sent event handler");
}

// TCP interface error event handler
void on15118TLSEvent::operator()(int error, const std::string& category, const std::string& message) {
  // process tcp error
  // connected error/ disconnected error/ send error/ receive error/ connection abort ?
  SPDLOG_INFO("[STC]: SUT TCP Error {0} - {1} disconnected", category, message);
}
