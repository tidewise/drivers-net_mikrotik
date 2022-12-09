#ifndef NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_MONITOR_CONFIG_HPP
#define NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_MONITOR_CONFIG_HPP

#include <string>

namespace network_status_monitoring {
    struct MikrotikRouterStatusMonitorConfig {
        std::string router_ip;

        std::string user;

        std::string password;
    };
}

#endif // NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_MONITOR_CONFIG_HPP