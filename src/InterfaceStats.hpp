#ifndef NET_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP
#define NET_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP

#include <string>
#include "base/Time.hpp"

namespace net_mikrotik {
    struct InterfaceStats {
        base::Time timestamp;

        std::string name;

        uint32_t actual_mtu;

        base::Time last_link_up_time;

        uint32_t link_downs;

        int64_t rx_byte;

        int64_t tx_byte;

        int64_t rx_packet;

        int64_t tx_packet;

        int32_t tx_queue_drop;

        int32_t rx_error;

        int32_t tx_error;

        int32_t rx_drop;

        int32_t tx_drop;

        uint64_t fp_rx_byte;

        uint64_t fp_tx_byte;

        uint64_t fp_rx_packet;

        uint64_t fp_tx_packet;

        bool running;
    };
}

#endif // NET_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP
