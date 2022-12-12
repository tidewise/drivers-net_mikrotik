#ifndef _SATCOMMSSTATUSPROJECT_SATCOMMSSTATUS_HPP_
#define _SATCOMMSSTATUSPROJECT_SATCOMMSSTATUS_HPP_

#include <boost/regex.hpp>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace network_status_monitoring {
    struct SatcommsStatusStruct {
        std::string online_offline_state = "";
        std::string flrx_snr = "";
        std::string antenna_status_az = "";
        std::string antenna_status_el = "";
        std::string antenna_state = "";
        std::string satelliteOrb = "";
        std::string beamInfo = "";
        std::string satellite_dlfreq = "";
        std::string modem_state = "";
        std::string login_last = "";
        std::string flrx_state = "";
        std::string flrx_carrier = "";
        std::string rltx_state = "";
        std::string rltx_carrier = "";
        std::string rltx_power = "";
    };

    class SatcommsStatusClass {
    public:
        std::string url_link;
        std::string url_data;
        static size_t writeCallback(void* contents,
            size_t size,
            size_t nmemb,
            void* userp);

    public:
        SatcommsStatusClass();
        std::map<std::string, std::string> processText(
            std::vector<std::string> status_id);

        void setURL(std::string);
        void getURL();

        SatcommsStatusStruct getSatcommsStatus();
    };

} // end namespace network_status_monitoring

#endif // _SATCOMMSSTATUSPROJECT_SATCOMMSSTATUS_HPP_
