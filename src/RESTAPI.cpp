#include "RESTAPI.hpp"
#include <sstream>

using namespace net_mikrotik;
using namespace std;

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
        "fp_rx_byte",
        "fp_rx_packet",
        "fp_tx_byte",
        "fp_tx_packet",
        "running"};
    return m_interface_connection->get(makePropListParams(prop_list));
}

std::vector<InterfaceStats> RESTAPI::parseInterfaceResponse(
    RestClient::Response const& response)
{
    std::vector<InterfaceStats> interface_stats;
    Json::Value json_response;
    m_json_reader.parse(response.body, json_response, false);

    for (auto router : json_response) {
        InterfaceStats stats;
        stats.name = json_response["name"].asString();
        stats.rx_byte = json_response["rx-byte"].asDouble();
        stats.rx_packet = json_response["rx-packet"].asDouble();
        stats.rx_error = json_response["rx-error"].asDouble();
        stats.rx_drop = json_response["rx-drop"].asDouble();
        stats.tx_byte = json_response["tx-byte"].asDouble();
        stats.tx_packet = json_response["tx-packet"].asDouble();
        stats.tx_queue_drop = json_response["tx-queue-drop"].asDouble();
        interface_stats.push_back(stats);
    }
    return interface_stats;
}