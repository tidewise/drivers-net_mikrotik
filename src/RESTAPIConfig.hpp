#ifndef NET_MIKROTIK_RESTAPI_CONFIG_HPP
#define NET_MIKROTIK_RESTAPI_CONFIG_HPP

#include <string>

namespace net_mikrotik {
    struct RESTAPIConfig {
        std::string router_ip;
        std::string user;
        std::string password;
        int timeout = 0;
    };
}

#endif // NET_MIKROTIK_RESTAPI_CONFIG_HPP