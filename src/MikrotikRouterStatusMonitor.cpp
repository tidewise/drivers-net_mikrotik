#include "MikrotikRouterStatusMonitor.hpp"

using namespace network_status_monitoring;

MikrotikRouterStatusMonitor::MikrotikRouterStatusMonitor()
{
    RestClient::init();
};

MikrotikRouterStatusMonitor::~MikrotikRouterStatusMonitor()
{
    RestClient::disable();
}

void MikrotikRouterStatusMonitor::setup(MikrotikRouterStatusMonitorConfig const& config)
{
    std::string stats_url = "https://" + config.router_ip + "/rest/interface/print/stats";
    m_stats_connection = setupConnection(stats_url, config.user, config.password);

    std::string details_url =
        "https://" + config.router_ip + "/rest/interface/print/details";
    m_details_connection = setupConnection(details_url, config.user, config.password);
}

RestClient::Connection* MikrotikRouterStatusMonitor::setupConnection(std::string url,
    std::string user,
    std::string password)
{
    auto* connection = new RestClient::Connection(url);
    connection->SetBasicAuth(user, password);

    RestClient::HeaderFields headers{};
    headers["content-type"] = "application/json";
    connection->SetHeaders(headers);

    return std::move(connection);
}

MikrotikRouterStatusResponse MikrotikRouterStatusMonitor::update()
{
    auto details_response = m_details_connection->post("",
        "{\".proplist\":[\"actual_mtu\",\"last_link_up_time\",\"last_link_down_time\","
        "\"link_downs\"]");
    auto details = processDetails(details_response);

    auto stats_response = m_stats_connection->post("", "");
    auto stats = processStats(stats_response);

    MikrotikRouterStatusResponse router_status;
    router_status.stats = stats;
    router_status.details = details;
    return router_status;
}

std::vector<MikrotikRouterStats> MikrotikRouterStatusMonitor::processStats(
    RestClient::Response const& response)
{
    std::vector<MikrotikRouterStats> routers_stats;
    Json::Value parsed_response;
    m_json_reader.parse(response.body, parsed_response, false);

    for (auto router : parsed_response) {
        MikrotikRouterStats stats;
        stats.id = parsed_response["#"].asInt();
        stats.name = parsed_response["name"].asString();
        stats.rx_byte = parsed_response["rx-byte"].asDouble();
        stats.tx_byte = parsed_response["tx-byte"].asDouble();
        stats.rx_packet = parsed_response["rx-packet"].asDouble();
        stats.tx_packet = parsed_response["tx-packet"].asDouble();
        stats.tx_queue_drop = parsed_response["tx-queue-drop"].asDouble();
        routers_stats.push_back(stats);
    }
    return routers_stats;
}

std::vector<MikrotikRouterDetails> MikrotikRouterStatusMonitor::processDetails(
    RestClient::Response const& response)
{
    std::vector<MikrotikRouterDetails> routers_details;
    Json::Value parsed_response;
    m_json_reader.parse(response.body, parsed_response, false);

    for (auto router : parsed_response) {
        MikrotikRouterDetails details;
        details.id = parsed_response["#"].asInt();
        details.name = parsed_response["name"].asString();
        details.actual_mtu = parsed_response["actual_mtu"].asDouble();
        details.last_link_up_time =
            base::Time::fromString(parsed_response["last_link_up_time"].asString());
        details.last_link_down_time =
            base::Time::fromString(parsed_response["last_link_down_time"].asString());
        details.link_downs = parsed_response["link_downs"].asInt();
        routers_details.push_back(details);
    }
    return routers_details;
}