#include "net_mikrotik/RESTAPI.hpp"
#include "net_mikrotik/RESTAPIConfig.hpp"
#include <fstream>
#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace net_mikrotik;
using namespace std;

struct RESTAPITest : public ::testing::Test {
    RESTAPITest()
    {
    }

    void assertEqualInterfaceStats(InterfaceStats expected, InterfaceStats actual)
    {
        ASSERT_EQ(expected.name, actual.name);
        ASSERT_EQ(expected.actual_mtu, actual.actual_mtu);
        ASSERT_EQ(expected.last_link_up_time, actual.last_link_up_time);
        ASSERT_EQ(expected.link_downs, actual.link_downs);
        ASSERT_EQ(expected.rx_byte, actual.rx_byte);
        ASSERT_EQ(expected.tx_byte, actual.tx_byte);
        ASSERT_EQ(expected.rx_packet, actual.rx_packet);
        ASSERT_EQ(expected.tx_packet, actual.tx_packet);
        ASSERT_EQ(expected.tx_queue_drop, actual.tx_queue_drop);
        ASSERT_EQ(expected.rx_error, actual.rx_error);
        ASSERT_EQ(expected.tx_error, actual.tx_error);
        ASSERT_EQ(expected.rx_drop, actual.rx_drop);
        ASSERT_EQ(expected.tx_drop, actual.tx_drop);
        ASSERT_EQ(expected.fp_rx_byte, actual.fp_rx_byte);
        ASSERT_EQ(expected.fp_tx_byte, actual.fp_tx_byte);
        ASSERT_EQ(expected.fp_rx_packet, actual.fp_rx_packet);
        ASSERT_EQ(expected.fp_tx_packet, actual.fp_tx_packet);
        ASSERT_EQ(expected.running, actual.running);
    }

    vector<InterfaceStats> getExpectedStats()
    {
        InterfaceStats first;
        first.name = "ether1";
        first.actual_mtu = 1500;
        first.link_downs = 0;
        first.rx_byte = 84395252;
        first.rx_packet = 273364;
        first.rx_error = 0;
        first.rx_drop = 0;
        first.tx_byte = 30408261;
        first.tx_packet = 71327;
        first.tx_error = 0;
        first.tx_drop = 0;
        first.tx_queue_drop = 0;
        first.fp_rx_byte = 83302224;
        first.fp_tx_byte = 24497719;
        first.fp_rx_packet = 273367;
        first.fp_tx_packet = 46493;
        first.running = true;
        first.last_link_up_time =
            base::Time::fromString("20221213-09:47:29Z", base::Time::Resolution::Seconds);

        InterfaceStats second;
        second.name = "ether2";
        second.actual_mtu = 1500;
        second.link_downs = 0;
        second.rx_byte = 28597163;
        second.rx_packet = 62845;
        second.rx_error = 0;
        second.rx_drop = 0;
        second.tx_byte = 44244509;
        second.tx_packet = 70997;
        second.tx_error = 0;
        second.tx_drop = 0;
        second.tx_queue_drop = 0;
        second.fp_rx_byte = 28343886;
        second.fp_tx_byte = 13541656;
        second.fp_rx_packet = 62833;
        second.fp_tx_packet = 27226;
        second.running = true;
        second.last_link_up_time =
            base::Time::fromString("20221213-09:47:30Z", base::Time::Resolution::Seconds);

        return vector<InterfaceStats>{first, second};
    }
};

TEST_F(RESTAPITest, it_is_able_to_establish_a_connection_to_google)
{
    RestClient::init();
    auto api = RESTAPI();

    RESTAPIConfig config;
    config.timeout = base::Time::fromSeconds(5);

    auto* connection = api.setupConnection("www.google.com", config);
    auto response = connection->get("");
    ASSERT_EQ(200, response.code);
    RestClient::disable();
}

TEST_F(RESTAPITest, it_succesfully_parses_a_complete_message)
{
    auto api = RESTAPI();

    auto resource_dir = getenv("NET_MIKROTIK_RESOURCE_DIR");
    stringstream rest_responses;
    rest_responses << resource_dir << "rest_responses.json";
    ifstream file(rest_responses.str(), ifstream::binary);

    Json::FastWriter writer;
    Json::Value root;
    file >> root;

    RestClient::Response response;
    response.code = 200;
    response.body = writer.write(root);
    auto stats = api.parseInterfaceResponse(response);

    auto expected = getExpectedStats();
    for (size_t i = 0; i < expected.size(); i++) {
        assertEqualInterfaceStats(expected[i], stats[i]);
    }
}

TEST_F(RESTAPITest, it_throws_when_an_expected_field_is_not_a_part_of_the_json)
{
    auto api = RESTAPI();

    auto resource_dir = getenv("NET_MIKROTIK_RESOURCE_DIR");
    stringstream rest_responses;
    rest_responses << resource_dir << "rest_responses.json";
    ifstream file(rest_responses.str(), ifstream::binary);

    Json::FastWriter writer;
    Json::Value root;
    file >> root;

    root[0].removeMember("link-downs");

    RestClient::Response response;
    response.code = 200;
    response.body = writer.write(root);
    ASSERT_THROW(
        {
            try {
                api.parseInterfaceResponse(response);
            }
            catch (runtime_error const& e) {
                ASSERT_STREQ("link-downs is not a valid field in the json structure",
                    e.what());
                throw;
            }
        },
        runtime_error);
}

TEST_F(RESTAPITest, it_throws_when_the_response_failed)
{
    auto api = RESTAPI();

    Json::FastWriter writer;
    Json::Value root;
    root["error"] = 406;
    root["message"] = "some error message";
    root["detail"] = "detailing the error message";

    RestClient::Response response;
    response.code = 406;
    response.body = writer.write(root);
    ASSERT_THROW(
        {
            try {
                api.parseInterfaceResponse(response);
            }
            catch (runtime_error const& e) {
                ASSERT_STREQ(
                    "Bad response: {\"detail\":\"detailing the error "
                    "message\",\"error\":406,\"message\":\"some error message\"}\n",
                    e.what());
                throw;
            }
        },
        runtime_error);
}

TEST_F(RESTAPITest, it_throws_when_there_is_an_invalid_parameter)
{
    auto api = RESTAPI();

    auto resource_dir = getenv("NET_MIKROTIK_RESOURCE_DIR");
    stringstream rest_responses;
    rest_responses << resource_dir << "rest_responses.json";
    ifstream file(rest_responses.str(), ifstream::binary);

    Json::FastWriter writer;
    Json::Value root;
    file >> root;

    root[0]["link-downs"] = "not a valid parameter";

    RestClient::Response response;
    response.code = 200;
    response.body = writer.write(root);
    ASSERT_THROW(
        {
            try {
                api.parseInterfaceResponse(response);
            }
            catch (invalid_argument const& e) {
                ASSERT_STREQ("Error parsing the link-downs field. The value read is not "
                             "a valid parameter. Got the following message: stoul",
                    e.what());
                throw;
            }
        },
        invalid_argument);
}

TEST_F(RESTAPITest, it_throws_when_there_is_the_value_is_out_of_range)
{
    auto api = RESTAPI();

    auto resource_dir = getenv("NET_MIKROTIK_RESOURCE_DIR");
    stringstream rest_responses;
    rest_responses << resource_dir << "rest_responses.json";
    ifstream file(rest_responses.str(), ifstream::binary);

    Json::FastWriter writer;
    Json::Value root;
    file >> root;

    root[0]["link-downs"] = "99999999999999999999";

    RestClient::Response response;
    response.code = 200;
    response.body = writer.write(root);
    ASSERT_THROW(
        {
            try {
                api.parseInterfaceResponse(response);
            }
            catch (out_of_range const& e) {
                ASSERT_STREQ("Error parsing the link-downs field. The value read is "
                             "99999999999999999999. Got the following message: stoul",
                    e.what());
                throw;
            }
        },
        out_of_range);
}

TEST_F(RESTAPITest, it_throws_when_the_time_format_is_not_supported)
{
    auto api = RESTAPI();

    auto resource_dir = getenv("NET_MIKROTIK_RESOURCE_DIR");
    stringstream rest_responses;
    rest_responses << resource_dir << "rest_responses.json";
    ifstream file(rest_responses.str(), ifstream::binary);

    Json::FastWriter writer;
    Json::Value root;
    file >> root;

    root[0]["last-link-up-time"] = "10/feb/1070 16:42:42";

    RestClient::Response response;
    response.code = 200;
    response.body = writer.write(root);
    ASSERT_THROW({ api.parseInterfaceResponse(response); }, runtime_error);
}

TEST_F(RESTAPITest, it_doesnt_throw_when_last_link_up_time_field_is_not_present)
{
    auto api = RESTAPI();

    auto resource_dir = getenv("NET_MIKROTIK_RESOURCE_DIR");
    stringstream rest_responses;
    rest_responses << resource_dir << "rest_responses.json";
    ifstream file(rest_responses.str(), ifstream::binary);

    Json::FastWriter writer;
    Json::Value root;
    file >> root;

    root[0].removeMember("last-link-up-time");

    RestClient::Response response;
    response.code = 200;
    response.body = writer.write(root);

    auto expected = getExpectedStats();
    expected[0].last_link_up_time = base::Time();

    auto actual = api.parseInterfaceResponse(response);
    for (size_t i = 0; i < expected.size(); i++) {
        assertEqualInterfaceStats(expected[i], actual[i]);
    }
}