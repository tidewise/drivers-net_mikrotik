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
    delete m_interface_connection;

    string interface_url = "https://" + config.router_ip + "/rest/interface";
    m_interface_connection = setupConnection(interface_url, config);
}

RestClient::Connection* RESTAPI::setupConnection(std::string const& url,
    RESTAPIConfig const& config)
{
    auto* connection = new RestClient::Connection(url);
    connection->SetBasicAuth(config.user, config.password);
    connection->SetTimeout(static_cast<int>(config.timeout.toSeconds()));

    RestClient::HeaderFields headers{};
    headers["content-type"] = "application/json";
    connection->SetHeaders(headers);
    connection->SetVerifyPeer(false);

    return connection;
}

string RESTAPI::makePropListParams(vector<string> const& prop_list)
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
    return m_interface_connection->get(makePropListParams(m_prop_list));
}

std::vector<InterfaceStats> RESTAPI::parseInterfaceResponse(
    RestClient::Response const& response)
{
    throwOnCurlError(response);

    std::vector<InterfaceStats> interface_stats;
    Json::Value json_response;
    stringstream(response.body) >> json_response;

    throwOnRequestFailure(response, json_response);

    Time timestamp = Time::now();
    for (auto link : json_response) {
        InterfaceStats stats;
        stats.timestamp = timestamp;

        throwOnInvalidFieldName(link, "name");
        stats.name = link["name"].asString();

        stats.actual_mtu = parseUint32Field(link, "actual-mtu");
        stats.link_downs = parseUint32Field(link, "link-downs");

        stats.rx_error = parseUint64Field(link, "rx-error");
        stats.rx_drop = parseUint64Field(link, "rx-drop");
        stats.tx_queue_drop = parseUint64Field(link, "tx-queue-drop");
        stats.tx_error = parseUint64Field(link, "rx-error");
        stats.tx_drop = parseUint64Field(link, "tx-drop");

        stats.rx_byte = parseUint64Field(link, "rx-byte");
        stats.rx_packet = parseUint64Field(link, "rx-packet");
        stats.tx_byte = parseUint64Field(link, "tx-byte");
        stats.tx_packet = parseUint64Field(link, "tx-packet");
        stats.fp_rx_byte = parseUint64Field(link, "fp-rx-byte");
        stats.fp_tx_byte = parseUint64Field(link, "fp-tx-byte");
        stats.fp_rx_packet = parseUint64Field(link, "fp-rx-packet");
        stats.fp_tx_packet = parseUint64Field(link, "fp-tx-packet");

        stats.running = parseBooleanField(link, "running");
        stats.last_link_up_time = parseTimeField(link, "last-link-up-time");

        interface_stats.push_back(stats);
    }
    return interface_stats;
}

string RESTAPI::detailedParsingErrorMessage(std::string const& field_name,
    std::string const& value,
    std::string const& error_message)
{
    stringstream message;
    message << "Error parsing the " << field_name << " field. The value read is " << value
            << ". Got the following message: " << error_message;
    return message.str();
}

void RESTAPI::throwOnCurlError(RestClient::Response const& response)
{
    if (response.code > 100) {
        return;
    }

    stringstream error_message;
    error_message << "Curl error " << response.code << ": " << response.body;
    throw runtime_error(error_message.str());
}

void RESTAPI::throwOnRequestFailure(RestClient::Response const& response,
    Json::Value const& json_response)
{
    if (response.code == 200) {
        return;
    }

    stringstream error_message;
    Json::FastWriter writer;
    error_message << "Bad response: " << writer.write(json_response);
    throw runtime_error(error_message.str());
}

void RESTAPI::throwOnInvalidFieldName(Json::Value const& json, string const& field_name)
{
    if (!json.isMember(field_name)) {
        stringstream error_message;
        error_message << field_name << " is not a valid field in the json structure";
        throw runtime_error(error_message.str());
    }
}

uint32_t RESTAPI::parseUint32Field(Json::Value const& json, string const& field_name)
{
    throwOnInvalidFieldName(json, field_name);

    string value = json[field_name].asString();
    try {
        return stoul(value);
    }
    catch (invalid_argument const& err) {
        throw invalid_argument(
            detailedParsingErrorMessage(field_name, value, err.what()));
    }
    catch (out_of_range const& err) {
        throw out_of_range(detailedParsingErrorMessage(field_name, value, err.what()));
    }
}

uint64_t RESTAPI::parseUint64Field(Json::Value const& json, string const& field_name)
{
    throwOnInvalidFieldName(json, field_name);

    string value = json[field_name].asString();
    try {
        return stoull(value);
    }
    catch (invalid_argument const& err) {
        throw invalid_argument(
            detailedParsingErrorMessage(field_name, value, err.what()));
    }
    catch (out_of_range const& err) {
        throw out_of_range(detailedParsingErrorMessage(field_name, value, err.what()));
    }
}

bool RESTAPI::parseBooleanField(Json::Value const& json, string const& field_name)
{
    throwOnInvalidFieldName(json, field_name);

    string value = json[field_name].asString();
    return value == "true";
}

Time RESTAPI::parseTimeField(Json::Value const& json, string const& field_name)
{
    if (!json.isMember(field_name)) {
        return Time();
    }

    string value = json[field_name].asString();
    try {
        // Add Z to the time string to treat it as UTC
        return Time::fromString(value + "Z", Resolution::Seconds, "%b/%d/%Y %T");
    }
    catch (runtime_error const& err) {
        throw runtime_error(detailedParsingErrorMessage(field_name, value, err.what()));
    }
}