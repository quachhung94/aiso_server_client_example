/*!
    \file ssl_context.h
    \brief SSL context definition
    \author Ivan Shynkarenka
    \date 12.02.2019
    \copyright MIT License
*/

#ifndef CPPSERVER_ASIO_SSL_CONTEXT_H
#define CPPSERVER_ASIO_SSL_CONTEXT_H

#include "service.h"

namespace CppCommon {
namespace Asio {

//! SSL context
/*!
    SSL context is used to handle and validate certificates in SSL clients and servers.

    Thread-safe.
*/
class SSLContext : public asio::ssl::context
{
public:
    using asio::ssl::context::context;

    SSLContext(const SSLContext&) = delete;
    SSLContext(SSLContext&&) = delete;
    ~SSLContext() = default;

    SSLContext& operator=(const SSLContext&) = delete;
    SSLContext& operator=(SSLContext&&) = delete;

    //! Configures the context to use system root certificates
    void set_root_certs();
    void SSLContext_load_verify_file(const std::string& filename);
    void SSLContext_set_verify_mode(verify_mode v);
    void SSLContext_set_options(context::options o);
    void SSLContext_set_default_verify_paths();
};

} // namespace Asio
} // namespace CppCommon

#endif // CPPSERVER_ASIO_SSL_CONTEXT_H
