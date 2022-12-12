#include <boost/regex.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <network_status_monitoring/SatcommsStatus.hpp>

using namespace network_status_monitoring;

struct SatcommsStatusTest : public ::testing::Test {
};

TEST_F(SatcommsStatusTest, it_should_not_crash_when_set_url_is_called)
{
    network_status_monitoring::SatcommsStatusClass satcommsstatus;
    satcommsstatus.setURL("https://www.google.com/");

    ASSERT_STREQ(satcommsstatus.url_link.c_str(), "https://www.google.com/");
}

TEST_F(SatcommsStatusTest, it_should_not_crash_when_get_url_is_called)
{
    network_status_monitoring::SatcommsStatusClass satcommsstatus;

    satcommsstatus.setURL("https://www.google.com/");
    satcommsstatus.getURL();

    boost::smatch matches;
    regex_search(satcommsstatus.url_data, matches, boost::regex("google"));
    ASSERT_GT(matches.size(), 0);
}

TEST_F(SatcommsStatusTest, it_should_return_a_map_with_results)
{
    network_status_monitoring::SatcommsStatusClass satcommsstatus;
    std::string path = std::getenv("SRC_DIR");
    path += "/satcomms.html";
    std::ifstream file(path);

    if (file.is_open()) { // checking whether the file is open
        std::stringstream strStream;
        strStream << file.rdbuf(); // read the file
        satcommsstatus.url_data = strStream.str();
    }
    else {
        std::cout << "File not found" << std::endl;
    }
    file.close();

    const std::vector<std::string> features2 = {
        "online_offline_state",
        "flrx_snr",
        "antenna_status_az",
        "antenna_status_el",
        "antenna_state",
        "satelliteOrb",
        "beamInfo",
        "satellite_dlfreq",
        "modem_state",
        "login_last",
        "flrx_state",
        "flrx_carrier",
        "rltx_state",
        "rltx_carrier",
        "rltx_power",
    };

    std::map<std::string, std::string> result = satcommsstatus.processText(features2);
    ASSERT_EQ(result.size(), 15);
}

TEST_F(SatcommsStatusTest, it_should_return_a_SatcommsStatusStruct)
{
    network_status_monitoring::SatcommsStatusClass satcommsstatus;
    std::string path = std::getenv("SRC_DIR");
    path += "/satcomms.html";
    std::ifstream file(path);

    if (file.is_open()) {
        std::stringstream strStream;
        strStream << file.rdbuf();
        satcommsstatus.url_data = strStream.str();
    }
    else {
        std::cout << "File not found" << std::endl;
    }
    file.close();

    SatcommsStatusStruct result = satcommsstatus.getSatcommsStatus();

    SatcommsStatusStruct expected;
    expected.online_offline_state = "UNKNOWN";
    expected.flrx_snr = "-999.99dB";
    expected.antenna_status_az = "&deg;";
    expected.antenna_status_el = "&deg;";
    expected.antenna_state = "INITIALIZING";
    expected.satelliteOrb = "0";
    expected.beamInfo = "UNKNOWN-BAND: UNKNOWN";
    expected.satellite_dlfreq = "UNKNOWN";
    expected.modem_state = "WAITING_FOR_RX_LOCK";
    expected.login_last = "UNKNOWN";
    expected.flrx_state = "UNKNOWN";
    expected.flrx_carrier = "UNKNOWN";
    expected.rltx_state = "UNKNOWN";
    expected.rltx_carrier = "UNKNOWN";
    expected.rltx_power = "UNKNOWN";

    ASSERT_EQ(result.online_offline_state, expected.online_offline_state);
}
