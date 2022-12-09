#ifndef NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP
#define NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP

#include <string>
#include "base/Time.hpp"

namespace network_status_monitoring {
    struct MikrotikRouterDetails {
        int id;

        std::string name;

        double actual_mtu;

        base::Time last_link_up_time;

        base::Time last_link_down_time;

        double link_downs;
    };

    struct MikrotikRouterStats {
        int id;

        std::string name;

        double rx_byte;

        double tx_byte;

        double rx_packet;

        double tx_packet;

        double tx_queue_drop;
    };

    struct MikrotikRouterStatusResponse {
        std::vector<MikrotikRouterStats> stats;
        std::vector<MikrotikRouterDetails> details;
    };
}

#endif // NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP
