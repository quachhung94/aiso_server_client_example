#include "TLSServer.h"
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

  this->_pSSLContextIf = std::make_shared<CppCommon::Asio::SSLContext>(asio::ssl::context::tlsv12);
  this->_pSSLContextIf->set_password_callback([](size_t max_length, asio::ssl::context::password_purpose purpose) -> std::string { return "qwerty"; });
  this->_pSSLContextIf->use_certificate_chain_file("../../tools/certificates/server.pem");
  this->_pSSLContextIf->use_private_key_file("../../tools/certificates/server.pem", asio::ssl::context::pem);
  this->_pSSLContextIf->use_tmp_dh_file("../../tools/certificates/dh4096.pem");

  // Create TCP Server IF
  this->_pTLSServerIf = std::make_shared<TLSServerAdapter>(this->_ptrService, this->_pSSLContextIf, ipv6_multicast);
  SPDLOG_INFO("Demo Initial DONE!!");
}

Demo::~Demo()
{
    this->_pTLSServerIf.reset();
}

bool Demo::start()
{
  SPDLOG_DEBUG("Demo Start");
  auto self(this->shared_from_this());
    // create TCP handler object
  this->_onTLSConnect = std::make_shared<onTLSEvent>(en_tlsServerAdapterCallbackType_CONNECT, self);
  this->_onTLSDisconnect = std::make_shared<onTLSEvent>(en_tlsServerAdapterCallbackType_DISCONNECT, self);
  this->_onTLSEmpty = std::make_shared<onTLSEvent>(en_tlsServerAdapterCallbackType_EMPTY, self);
  this->_onTLSReceive = std::make_shared<onTLSEvent>(en_tlsServerAdapterCallbackType_RECEIVE, self);
  this->_onTLSSent = std::make_shared<onTLSEvent>(en_tlsServerAdapterCallbackType_SENT, self);
  this->_onTLSError = std::make_shared<onTLSEvent>(en_tlsServerAdapterCallbackType_ERROR, self);
  // add TCP handler object to TCP interface
  this->_pTLSServerIf->setEventCallBack(this->_onTLSConnect);
  this->_pTLSServerIf->setEventCallBack(this->_onTLSDisconnect);
  this->_pTLSServerIf->setEventCallBack(this->_onTLSEmpty);
  this->_pTLSServerIf->setEventCallBack(this->_onTLSReceive);
  this->_pTLSServerIf->setEventCallBack(this->_onTLSSent);
  this->_pTLSServerIf->setEventCallBack(this->_onTLSError);


  this->_pTLSServerIf->start();
  SPDLOG_DEBUG("Demo Start Success");

  // Endless loop
  // this->_ptrIOService->run();

  return true;
}


bool Demo::stop()
{
  (void)this->_pTLSServerIf->disconnectAll();
  this->_pTLSServerIf->stop();
  SPDLOG_DEBUG("[STC]: Logical link leave, stop TCP, UDP");
  return true;
}

// event for connect/disconnect TCP
void onTLSEvent::operator()() {
//   std::shared_ptr<BaseOperation> responseEvent = std::make_shared<BaseOperation>(OpType_TCP);
  switch (this->type){
    case en_tlsServerAdapterCallbackType_CONNECT: {
      // push event to event queue
    //   this->parent->_pTLSServerIf->IsStarted();
      SPDLOG_DEBUG("en_tlsServerAdapterCallbackType_CONNECT");
    //   responseEvent->TcpControlRes = std::make_shared<iso1Part4_V2G_TCP_TLS_Port_Control_Internal_MessageRes>();
    //   responseEvent->TcpControlRes->result.portStatus.closed = !(this->parent->_pTCPIf->IsConnected());
    //   responseEvent->TcpControlRes->result.portNumber.intValue = this->parent->_pTCPIf->port();
    //   responseEvent->TcpControlRes->command = iso1Part4_V2G_TCP_TLS_Port_Control_Command_TYPE::e_connectPort;
    //   responseEvent->TcpControlRes->ack = iso1Part4_V2G_TCP_TLS_Port_Control_Ack_TYPE::e_success;
    //   this->parent->pt_V2G_TCP_TLS_ALM_SECC_Port->setEvent(responseEvent);
    //   Logging::debug(LogComponent_ENABLE, fmt::format("[STC]: SUT TCP {0} - {1} connected",this->parent->_pTCPIf->address(), this->parent->_pTCPIf->port()));
      break;
    }
    case en_tlsServerAdapterCallbackType_DISCONNECT: {
    //   responseEvent->TcpControlRes = std::make_shared<iso1Part4_V2G_TCP_TLS_Port_Control_Internal_MessageRes>();
    //   responseEvent->TcpControlRes->result.portStatus.closed = !(this->parent->_pTCPIf->IsConnected());
    //   responseEvent->TcpControlRes->result.portNumber.intValue = this->parent->_pTCPIf->port();
    //   responseEvent->TcpControlRes->command = iso1Part4_V2G_TCP_TLS_Port_Control_Command_TYPE::e_disconnectPort;
    //   responseEvent->TcpControlRes->ack = iso1Part4_V2G_TCP_TLS_Port_Control_Ack_TYPE::e_success;
    //   this->parent->pt_V2G_TCP_TLS_ALM_SECC_Port->setEvent(responseEvent);
    //   Logging::debug(LogComponent_ENABLE, fmt::format("[STC]: SUT TCP {0} - {1} disconnected",this->parent->_pTCPIf->address(), this->parent->_pTCPIf->port()));
        SPDLOG_DEBUG("en_tlsServerAdapterCallbackType_DISCONNECT");
      break;
    }
    case en_tlsServerAdapterCallbackType_EMPTY: {
      SPDLOG_DEBUG("Empty event TCP");
      break;
    }
    default:
      break;
  }
}

// TCP receive event receive handler
void onTLSEvent::operator()(const void *buffer, size_t size) {
//   std::shared_ptr<V2gTpMessage> recMsg = std::make_shared<V2gTpMessage>();
  // push data to recMsg
//   recMsg->setMessage((const char*)buffer, size);
  // update to port queue
//   this->parent->pt_V2G_TCP_TLS_ALM_SECC_Port->setMsg(recMsg);
  SPDLOG_DEBUG("[STC]: TCP receive event receive handler");
}

// TCP interface sent event handler
void onTLSEvent::operator()(size_t sent, size_t pending) {
//   std::shared_ptr<BaseOperation> responseEvent = std::make_shared<BaseOperation>(OpType_TCP);
    SPDLOG_DEBUG("[STC]: TCP interface sent event handler");
}

// TCP interface error event handler
void onTLSEvent::operator()(int error, const std::string& category, const std::string& message) {
//   std::shared_ptr<BaseOperation> responseEvent = std::make_shared<BaseOperation>(OpType_TCP);
  // process tcp error
  // connected error/ disconnected error/ send error/ receive error/ connection abort ?
  SPDLOG_DEBUG("[STC]: SUT TCP Error {0} - {1} disconnected", category, message);
}

// void ATask::init()
// {
//   this->runenv = std::make_shared<IfRuntime>();
//   this->demo = std::make_shared<Demo>(runenv, IPV6_ADDR, IPV6_PORT, "lo");
//   demo->start();
// }

App::App() : Application(platform::core::APPLICATION_BASE_PRIO, std::chrono::seconds(1800)),
            m_timerEventQueue(ExpiredQueue::create(30, *this, *this)),
            timers()
{

}

void App::init()
{
  SPDLOG_INFO("App::Init Starting...");
  // platform::core::SystemStatistics::instance().dump();
  // a_instance.start();
  // this->m_wsMonitorTimeout = 10;
  this->a_instance = std::make_shared<ATask>();
  this->a_instance->start();
  this->runenv = std::make_shared<IfRuntime>();
  this->demo = std::make_shared<Demo>(runenv, IPV6_ADDR, IPV6_PORT, "lo");
  demo->start();
  // SPDLOG_INFO("ATask::Init Starting...");
  // std::this_thread::sleep_for(std::chrono::seconds(2));
  // create_timer(1, std::chrono::milliseconds(1000));
  // create_timer(2, std::chrono::milliseconds(10000));
  // create_timer(3, std::chrono::milliseconds(100000));
  // create_timer(4, std::chrono::milliseconds(1000000));
  // this->create_timer(std::chrono::milliseconds(10000));
  // for (auto& t : this->timers)
  // {
  //     t.timer->start();
  // }
  // timers.front().timer->stop();
  // timers.front().timer->start();
  // timers.at(1).timer->stop();
  // timers.at(1).timer->start();

}

void App::tick()
{
  SPDLOG_INFO("App Hello world!");
}

void App::event(const platform::core::timer::TimerExpiredEvent& event)
{
    auto& info = timers[static_cast<decltype(timers)::size_type>(event.getId())];
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - info.last);
    info.last = std::chrono::steady_clock::now();
    info.count++;
    info.total += duration;

    SPDLOG_INFO("Timer ID {} ({}ms): {}ms, avg: {}",
                    event.getId(),
                    info.interval.count(),
                    duration.count(),
                    static_cast<double>(info.total.count()) / info.count);
}

void App::create_timer(int id, std::chrono::milliseconds interval)
{
    TimerInfo t;
    t.timer = platform::core::timer::Timer::create(static_cast<int32_t>(id), m_timerEventQueue, true, interval);
    t.interval = interval;
    timers.push_back(t);
}
