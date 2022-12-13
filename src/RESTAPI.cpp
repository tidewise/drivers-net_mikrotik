#include "RESTAPI.hpp"
#include <sstream>

using namespace net_mikrotik;
using namespace std;
using namespace base;
using Resolution = base::Time::Resolution;

RESTAPI::RESTAPI(){};

RESTAPI::~RESTAPI()
{
    delete m_interface_connection;
}

void RESTAPI::setup(RESTAPIConfig const& config)
{
    string interface_url = "https://" + config.router_ip + "/rest/interface";
    m_interface_connection = setupConnection(interface_url, config.user, config.password);
}

RestClient::Connection* RESTAPI::setupConnection(std::string url,
    std::string user,
    std::string password)
{
    auto* connection = new RestClient::Connection(url);
    connection->SetBasicAuth(user, password);

    RestClient::HeaderFields headers{};
    headers["content-type"] = "application/json";
    connection->SetHeaders(headers);

    return connection;
}

string RESTAPI::makePropListParams(vector<string> prop_list)
{
    stringstream url_params;
    url_params << "?.proplist=";
    for (auto prop : prop_list) {
        url_params << prop << ",";
    }
    return url_params.str();
}

RestClient::Response RESTAPI::getInterfaceResponse()
{
    vector<string> prop_list{"name",
        "actual-mtu",
        "last-link-up-time",
        "link-downs",
        "rx-byte",
        "rx-packet",
        "rx-error",
        "rx-drop",
        "tx-byte",
        "tx-packet",
        "tx-error",
        "tx-drop",
        "fp-rx-byte",
        "fp-rx-packet",
        "fp-tx-byte",
        "fp-tx-packet",
        "running"};
    return m_interface_connection->get(makePropListParams(prop_list));
}

std::vector<InterfaceStats> RESTAPI::parseInterfaceResponse(
    RestClient::Response const& response)
{
    std::vector<InterfaceStats> interface_stats;
    Json::Value json_response;
    stringstream(response.body) >> json_response;

    Time timestamp = Time::now();
    for (auto router : json_response) {
        InterfaceStats stats;
        stats.timestamp = timestamp;
        stats.name = router["name"].asString();
        stats.actual_mtu = stoul(router["actual-mtu"].asString());
        stats.link_downs = stoul(router["link-downs"].asString());
        stats.rx_byte = stoll(router["rx-byte"].asString());
        stats.rx_packet = stoll(router["rx-packet"].asString());
        stats.rx_error = stoll(router["rx-error"].asString());
        stats.rx_drop = stoll(router["rx-drop"].asString());
        stats.tx_byte = stoll(router["tx-byte"].asString());
        stats.tx_packet = stoll(router["tx-packet"].asString());
        stats.tx_error = stoll(router["tx-error"].asString());
        stats.tx_drop = stoll(router["tx-drop"].asString());
        stats.tx_queue_drop = stoll(router["tx-queue-drop"].asString());
        stats.fp_rx_byte = stoll(router["fp-rx-byte"].asString());
        stats.fp_tx_byte = stoll(router["fp-tx-byte"].asString());
        stats.fp_rx_packet = stoll(router["fp-rx-packet"].asString());
        stats.fp_tx_packet = stoll(router["fp-tx-packet"].asString());
        stats.running = (router["running"].asString() == "true");

        string last_link_up_time_str = router["last-link-up-time"].asString();
        stats.last_link_up_time = Time::fromString(last_link_up_time_str + "Z",
            Resolution::Seconds,
            "%b/%d/%Y %T");
        interface_stats.push_back(stats);
    }
    return interface_stats;
}