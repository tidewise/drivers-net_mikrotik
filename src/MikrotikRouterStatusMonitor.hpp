#ifndef NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_MONITOR_HPP
#define NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_MONITOR_HPP

#include "MikrotikRouterStatusMonitorConfig.hpp"
#include "MikrotikRouterStatusResponse.hpp"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include "json/json.h"

namespace network_status_monitoring {
    class MikrotikRouterStatusMonitor {
    public:
        MikrotikRouterStatusMonitor();
        ~MikrotikRouterStatusMonitor();

        void setup(MikrotikRouterStatusMonitorConfig const& config);

        MikrotikRouterStatusResponse update();

        void disableClient();

    private:
        RestClient::Connection* m_stats_connection = nullptr;
        RestClient::Connection* m_details_connection = nullptr;
        Json::Reader m_json_reader{};

        static RestClient::Connection* setupConnection(std::string url,
            std::string user,
            std::string password);

        std::vector<MikrotikRouterStats> processStats(RestClient::Response const& response);

        std::vector<MikrotikRouterDetails> processDetails(RestClient::Response const& response);
    };

} // end namespace network_status_monitoring

#endif // NETWORK_STATUS_MONITORING_MIKROTIK_ROUTER_STATUS_MONITOR_HPP
