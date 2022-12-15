#include "RESTAPI.hpp"
#include "RESTAPIConfig.hpp"
#include <iostream>

using namespace std;
using namespace net_mikrotik;

int main(int argc, char const* argv[])
{
    RestClient::init();

    if (argc != 4) {
        cerr << "Expecting 3 arguments in the form of <router-ip> <user> <password>\n";
        return 1;
    }

    RESTAPIConfig config;
    config.router_ip = argv[1];
    config.user = argv[2];
    config.password = argv[3];
    config.timeout = base::Time::fromSeconds(10);

    RESTAPI api;
    api.setup(config);

    auto response = api.getInterfaceResponse();
    auto parsed = api.parseInterfaceResponse(response);

    for (const auto& stats : parsed) {
        cout << "\n==== LINK ====\n";
        cout << "Timestamp: " << stats.timestamp.toString() << "\n";
        cout << "Name: " << stats.name << "\n";
        cout << "Actual MTU: " << stats.actual_mtu << "\n";
        cout << "Last Link Up Time: " << stats.last_link_up_time.toString() << "\n";
        cout << "Link downs: " << stats.link_downs << "\n";
        cout << "RX Byte: " << stats.rx_byte << "\n";
        cout << "TX Byte: " << stats.tx_byte << "\n";
        cout << "RX Packet: " << stats.rx_packet << "\n";
        cout << "TX Packet: " << stats.rx_packet << "\n";
        cout << "RX Error: " << stats.rx_error << "\n";
        cout << "TX Error: " << stats.rx_error << "\n";
        cout << "RX Drop: " << stats.rx_drop << "\n";
        cout << "TX Drop: " << stats.rx_drop << "\n";
        cout << "FP RX Byte: " << stats.fp_rx_byte << "\n";
        cout << "FP TX Byte: " << stats.fp_tx_byte << "\n";
        cout << "FP RX Packet: " << stats.fp_rx_packet << "\n";
        cout << "FP TX Packet: " << stats.fp_tx_packet << "\n";
        cout << "Running: " << stats.running << "\n";
    }
    RestClient::disable();
    return 0;
}
