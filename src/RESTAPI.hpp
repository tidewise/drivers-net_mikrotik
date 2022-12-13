#ifndef NET_MIKROTIK_RESTAPI_HPP
#define NET_MIKROTIK_RESTAPI_HPP

#include "InterfaceStats.hpp"
#include "RESTAPIConfig.hpp"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include "json/json.h"

namespace net_mikrotik {
    class RESTAPI {
    public:
        /**
         * @brief Construct a new Router Status Monitor object. When using this
         * class, one is responsible for calling the RestClient::init() and
         * RestClient::disable() methods! They ARE NOT thread safe and they need
         * to be called, respectively right at the beginning of the program  and
         * before shutdown.
         *
         */
        RESTAPI();
        ~RESTAPI();

        /**
         * @brief Sets up the rest client connection
         *
         * @param config the configuration parameters
         */
        void setup(RESTAPIConfig const& config);

        /**
         * @brief Gets a new interface stats sample from the configured URL.
         *
         * @return std::vector<InterfaceStats>
         */
        RestClient::Response getInterfaceResponse();

        /**
         * @brief Parses the response into a vector of interface stats objects.
         *
         * @param response the response from the interface URL.
         * @return std::vector<InterfaceStats>
         */
        std::vector<InterfaceStats> parseInterfaceResponse(
            RestClient::Response const& response);

        /**
         * @brief Sets up the connection object from a given URL, using basic
         * authentication.
         *
         * @param url the url to connect to.
         * @param user the username
         * @param password the user's password
         * @return RestClient::Connection*
         */
        static RestClient::Connection* setupConnection(std::string url,
            std::string user,
            std::string password);

    private:
        RestClient::Connection* m_interface_connection = nullptr;
        Json::Reader m_json_reader{};

        /**
         * @brief Makes the prop list URL parameters from a vector of the
         * desired properties.
         *
         * @param prop_list the desired properties.
         * @return std::string the parameters to be added to the URL.
         */
        static std::string makePropListParams(std::vector<std::string> prop_list);
    };

} // end namespace net_mikrotik

#endif // NET_MIKROTIK_RESTAPI_HPP
