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
    m_interface_connection = setupConnection(interface_url, config);
}

RestClient::Connection* RESTAPI::setupConnection(std::string const& url,
    RESTAPIConfig const& config)
{
    auto* connection = new RestClient::Connection(url);
    connection->SetBasicAuth(config.user, config.password);
    connection->SetTimeout(config.timeout);

    RestClient::HeaderFields headers{};
    headers["content-type"] = "application/json";
    connection->SetHeaders(headers);

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

    throwOnRequestFailure(response, json_response);

    Time timestamp = Time::now();
    for (auto router : json_response) {
        InterfaceStats stats;
        stats.timestamp = timestamp;

        throwOnInvalidFieldName(router, "name");
        stats.name = router["name"].asString();

        stats.actual_mtu = parseUnsignedField(router, "actual-mtu");
        stats.link_downs = parseUnsignedField(router, "link-downs");

        stats.rx_error = parseInt32Field(router, "rx-error");
        stats.rx_drop = parseInt32Field(router, "rx-drop");
        stats.tx_queue_drop = parseInt32Field(router, "tx-queue-drop");
        stats.tx_error = parseInt32Field(router, "rx-error");
        stats.tx_drop = parseInt32Field(router, "tx-drop");

        stats.rx_byte = parseInt64Field(router, "rx-byte");
        stats.rx_packet = parseInt64Field(router, "rx-packet");
        stats.tx_byte = parseInt64Field(router, "tx-byte");
        stats.tx_packet = parseInt64Field(router, "tx-packet");
        stats.fp_rx_byte = parseInt64Field(router, "fp-rx-byte");
        stats.fp_tx_byte = parseInt64Field(router, "fp-tx-byte");
        stats.fp_rx_packet = parseInt64Field(router, "fp-rx-packet");
        stats.fp_tx_packet = parseInt64Field(router, "fp-tx-packet");

        stats.running = parseBooleanField(router, "running");
        stats.last_link_up_time = parseTimeField(router, "last-link-up-time");

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

uint32_t RESTAPI::parseUnsignedField(Json::Value const& json, string const& field_name)
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

int64_t RESTAPI::parseInt64Field(Json::Value const& json, string const& field_name)
{
    throwOnInvalidFieldName(json, field_name);

    string value = json[field_name].asString();
    try {
        return stoll(value);
    }
    catch (invalid_argument const& err) {
        throw invalid_argument(
            detailedParsingErrorMessage(field_name, value, err.what()));
    }
    catch (out_of_range const& err) {
        throw out_of_range(detailedParsingErrorMessage(field_name, value, err.what()));
    }
}

int32_t RESTAPI::parseInt32Field(Json::Value const& json, string const& field_name)
{
    throwOnInvalidFieldName(json, field_name);

    string value = json[field_name].asString();
    try {
        return stol(value);
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
    throwOnInvalidFieldName(json, field_name);

    string value = json[field_name].asString();
    try {
        // Add Z to the time string to treat it as UTC
        return Time::fromString(value + "Z", Resolution::Seconds, "%b/%d/%Y %T");
    }
    catch (runtime_error const& err) {
        throw runtime_error(detailedParsingErrorMessage(field_name, value, err.what()));
    }
}