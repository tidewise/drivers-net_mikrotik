#ifndef NET_MIKROTIK_RESTAPI_CONFIG_HPP
#define NET_MIKROTIK_RESTAPI_CONFIG_HPP

#include "base/Time.hpp"
#include <string>

namespace net_mikrotik {
    struct RESTAPIConfig {
        std::string router_ip;
        std::string user;
        std::string password;
        base::Time timeout;
    };
}

#endif // NET_MIKROTIK_RESTAPI_CONFIG_HPP