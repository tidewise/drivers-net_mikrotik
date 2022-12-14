#ifndef NET_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP
#define NET_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP

#include "base/Time.hpp"
#include <string>

namespace net_mikrotik {
    struct InterfaceStats {
        base::Time timestamp;
        std::string name;
        uint32_t actual_mtu = 0;
        base::Time last_link_up_time;
        uint32_t link_downs = 0;
        int64_t rx_byte = 0;
        int64_t tx_byte = 0;
        int64_t rx_packet = 0;
        int64_t tx_packet = 0;
        int32_t tx_queue_drop = 0;
        int32_t rx_error = 0;
        int32_t tx_error = 0;
        int32_t rx_drop = 0;
        int32_t tx_drop = 0;
        uint64_t fp_rx_byte = 0;
        uint64_t fp_tx_byte = 0;
        uint64_t fp_rx_packet = 0;
        uint64_t fp_tx_packet = 0;
        bool running = false;
    };
}

#endif // NET_MIKROTIK_ROUTER_STATUS_RESPONSE_HPP
