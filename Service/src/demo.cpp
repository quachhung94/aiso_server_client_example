#include "demo.h"
#include "spdlog/spdlog.h"

Demo::Demo(const std::shared_ptr<IfRuntime>& runtime, const std::string &ipv6_multicast, uint16_t port, const std::string& _ipv6IF)
{
    SPDLOG_INFO("Demo Initial!!");
    this->ipv6IF = _ipv6IF;
    // get CppCommon::Asio::Service
    this->_pRunTime = runtime;
    this->_ptrService = this->_pRunTime->getService();
    // get the working io_service(bound to another thread) for queue operation
    this->_ptrIOService = this->_ptrService->GetAsioService();

    // Create TCP Server IF
    this->_pTCPServerIf = std::make_shared<TCPServerAdapter>(this->_ptrService, ipv6_multicast);
    SPDLOG_INFO("Demo Initial DONE!!");
}

Demo::~Demo()
{
    this->_pTCPServerIf.reset();
}

bool Demo::start()
{
  SPDLOG_DEBUG("Demo Start");
  auto self(this->shared_from_this());
    // create TCP handler object
  this->_onTcpConnect = std::make_shared<onTCPEvent>(en_tcpServerAdapterCallbackType_CONNECT, self);
  this->_onTcpDisconnect = std::make_shared<onTCPEvent>(en_tcpServerAdapterCallbackType_DISCONNECT, self);
  this->_onTcpEmpty = std::make_shared<onTCPEvent>(en_tcpServerAdapterCallbackType_EMPTY, self);
  this->_onTcpReceive = std::make_shared<onTCPEvent>(en_tcpServerAdapterCallbackType_RECEIVE, self);
  this->_onTcpSent = std::make_shared<onTCPEvent>(en_tcpServerAdapterCallbackType_SENT, self);
  this->_onTcpError = std::make_shared<onTCPEvent>(en_tcpServerAdapterCallbackType_ERROR, self);
  // add TCP handler object to TCP interface
  this->_pTCPServerIf->setEventCallBack(this->_onTcpConnect);
  this->_pTCPServerIf->setEventCallBack(this->_onTcpDisconnect);
  this->_pTCPServerIf->setEventCallBack(this->_onTcpEmpty);
  this->_pTCPServerIf->setEventCallBack(this->_onTcpReceive);
  this->_pTCPServerIf->setEventCallBack(this->_onTcpSent);
  this->_pTCPServerIf->setEventCallBack(this->_onTcpError);


  this->_pTCPServerIf->start();
  SPDLOG_DEBUG("Demo Start Success");

  // Endless loop
  this->_ptrIOService->run();

  return true;
}


bool Demo::stop()
{
  (void)this->_pTCPServerIf->disconnectAll();
  this->_pTCPServerIf->stop();
  SPDLOG_DEBUG("[STC]: Logical link leave, stop TCP, UDP");
  return true;
}

// event for connect/disconnect TCP
void onTCPEvent::operator()() {
//   std::shared_ptr<BaseOperation> responseEvent = std::make_shared<BaseOperation>(OpType_TCP);
  switch (this->type){
    case en_tcpServerAdapterCallbackType_CONNECT: {
      // push event to event queue
    //   this->parent->_pTCPServerIf->IsStarted();
      SPDLOG_DEBUG("en_tcpServerAdapterCallbackType_CONNECT");
    //   responseEvent->TcpControlRes = std::make_shared<iso1Part4_V2G_TCP_TLS_Port_Control_Internal_MessageRes>();
    //   responseEvent->TcpControlRes->result.portStatus.closed = !(this->parent->_pTCPIf->IsConnected());
    //   responseEvent->TcpControlRes->result.portNumber.intValue = this->parent->_pTCPIf->port();
    //   responseEvent->TcpControlRes->command = iso1Part4_V2G_TCP_TLS_Port_Control_Command_TYPE::e_connectPort;
    //   responseEvent->TcpControlRes->ack = iso1Part4_V2G_TCP_TLS_Port_Control_Ack_TYPE::e_success;
    //   this->parent->pt_V2G_TCP_TLS_ALM_SECC_Port->setEvent(responseEvent);
    //   Logging::debug(LogComponent_ENABLE, fmt::format("[STC]: SUT TCP {0} - {1} connected",this->parent->_pTCPIf->address(), this->parent->_pTCPIf->port()));
      break;
    }
    case en_tcpServerAdapterCallbackType_DISCONNECT: {
    //   responseEvent->TcpControlRes = std::make_shared<iso1Part4_V2G_TCP_TLS_Port_Control_Internal_MessageRes>();
    //   responseEvent->TcpControlRes->result.portStatus.closed = !(this->parent->_pTCPIf->IsConnected());
    //   responseEvent->TcpControlRes->result.portNumber.intValue = this->parent->_pTCPIf->port();
    //   responseEvent->TcpControlRes->command = iso1Part4_V2G_TCP_TLS_Port_Control_Command_TYPE::e_disconnectPort;
    //   responseEvent->TcpControlRes->ack = iso1Part4_V2G_TCP_TLS_Port_Control_Ack_TYPE::e_success;
    //   this->parent->pt_V2G_TCP_TLS_ALM_SECC_Port->setEvent(responseEvent);
    //   Logging::debug(LogComponent_ENABLE, fmt::format("[STC]: SUT TCP {0} - {1} disconnected",this->parent->_pTCPIf->address(), this->parent->_pTCPIf->port()));
        SPDLOG_DEBUG("en_tcpServerAdapterCallbackType_DISCONNECT");
      break;
    }
    case en_tcpAdapterCallbackType_EMPTY: {
      SPDLOG_DEBUG("Empty event TCP");
      break;
    }
    default:
      break;
  }
}

// TCP receive event receive handler
void onTCPEvent::operator()(const void *buffer, size_t size) {
//   std::shared_ptr<V2gTpMessage> recMsg = std::make_shared<V2gTpMessage>();
  // push data to recMsg
//   recMsg->setMessage((const char*)buffer, size);
  // update to port queue
//   this->parent->pt_V2G_TCP_TLS_ALM_SECC_Port->setMsg(recMsg);
  SPDLOG_DEBUG("[STC]: TCP receive event receive handler");
}

// TCP interface sent event handler
void onTCPEvent::operator()(size_t sent, size_t pending) {
//   std::shared_ptr<BaseOperation> responseEvent = std::make_shared<BaseOperation>(OpType_TCP);
    SPDLOG_DEBUG("[STC]: TCP interface sent event handler");
}

// TCP interface error event handler
void onTCPEvent::operator()(int error, const std::string& category, const std::string& message) {
//   std::shared_ptr<BaseOperation> responseEvent = std::make_shared<BaseOperation>(OpType_TCP);
  // process tcp error
  // connected error/ disconnected error/ send error/ receive error/ connection abort ?
  SPDLOG_DEBUG("[STC]: SUT TCP Error {0} - {1} disconnected", category, message);
}