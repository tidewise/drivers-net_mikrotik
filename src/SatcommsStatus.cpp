#include "SatcommsStatus.hpp"

using namespace std;
using namespace network_status_monitoring;

SatcommsStatusClass::SatcommsStatusClass()
{
}

size_t SatcommsStatusClass::writeCallback(void* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void SatcommsStatusClass::setURL(string url)
{
    this->url_link = url;
}

void SatcommsStatusClass::getURL()
{
    CURL* curl_handle;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, this->url_link.c_str());
    // disable progress meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, this->writeCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->url_data);
    curl_easy_perform(curl_handle);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

map<string, string> SatcommsStatusClass::processText(vector<string> status_id)
{
    // istringstream s(this->url_data);
    map<string, string> status;
    boost::smatch matches;
    for (const auto& feature : status_id) {
        if (regex_search(this->url_data,
                matches,
                boost::regex("id=\"" + feature + "\".*?>(.*?)<"))) {
            status[feature] = matches[1];
        }
    }
    return status;
}

SatcommsStatusStruct SatcommsStatusClass::getSatcommsStatus()
{
    const std::vector<std::string> status_id = {
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
    map<string, string> status_map;

    status_map = processText(status_id);

    SatcommsStatusStruct status;
    status.online_offline_state = status_map["online_offline_state"];
    status.flrx_snr = status_map["flrx_snr"];
    status.antenna_status_az = status_map["antenna_status_az"];
    status.antenna_status_el = status_map["antenna_status_el"];
    status.antenna_state = status_map["antenna_state"];
    status.satelliteOrb = status_map["satelliteOrb"];
    status.beamInfo = status_map["beamInfo"];
    status.satellite_dlfreq = status_map["satellite_dlfreq"];
    status.modem_state = status_map["modem_state"];
    status.login_last = status_map["login_last"];
    status.flrx_state = status_map["flrx_state"];
    status.flrx_carrier = status_map["flrx_carrier"];
    status.rltx_state = status_map["rltx_state"];
    status.rltx_carrier = status_map["rltx_carrier"];
    status.rltx_power = status_map["rltx_power"];

    return status;
}