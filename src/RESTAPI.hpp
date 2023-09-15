#ifndef NET_MIKROTIK_RESTAPI_HPP
#define NET_MIKROTIK_RESTAPI_HPP

#include <optional>

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
        static std::vector<InterfaceStats> parseInterfaceResponse(
            RestClient::Response const& response);

        /**
         * @brief Sets up the connection object from a given URL, using basic
         * authentication.
         *
         * @param url the url to connect to.
         * @param config the rest api config
         * @return RestClient::Connection*
         */
        static RestClient::Connection* setupConnection(std::string const& url,
            RESTAPIConfig const& config);

    private:
        std::vector<std::string> const m_prop_list{"name",
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
            "tx-queue-drop",
            "fp-rx-byte",
            "fp-rx-packet",
            "fp-tx-byte",
            "fp-tx-packet",
            "running"};

        RestClient::Connection* m_interface_connection = nullptr;

        /**
         * @brief Makes the prop list URL parameters from a vector of the
         * desired properties.
         *
         * @param prop_list the desired properties.
         * @return std::string the parameters to be added to the URL.
         */
        static std::string makePropListParams(std::vector<std::string> const& prop_list);

        /**
         * @brief Throws whenever the response json doesnt contain a field with
         * field_name.
         *
         * @param json the response json
         * @param field_name the field to search for.
         * @throw runtime_error
         */
        static void throwOnInvalidFieldName(Json::Value const& json,
            std::string const& field_name);

        /** @brief throws an exception that indicatest that the given field does not exist
         */
        static void throwFieldNameInvalid(std::string const& field_name);

        /**
         * @brief Throws whenever the response code is different from 200. We
         * treat any other code as failure, since all we do is a GET request.
         *
         * @param response the rest client response
         * @param json_response the response parsed into json
         * @throw runtime_error
         */
        static void throwOnRequestFailure(RestClient::Response const& response,
            Json::Value const& json_response);
        /**
         * @brief Throws whenever the response code represents a CURL error (<
         * 100).
         *
         * @param response the rest client response
         * @throw runtime_error
         */
        static void throwOnCurlError(RestClient::Response const& response);

        /**
         * @brief Generates the error message when parsing a field failed.
         *
         * @param field_name the name of the field that failed during parsing.
         * @param value the value that we tried to parse.
         * @param error_message the error message caught
         * @return std::string a detailed error message
         */
        static std::string detailedParsingErrorMessage(std::string const& field_name,
            std::string const& value,
            std::string const& error_message);

        /**
         * @brief Parses a string value read from a json field into uint32_t.
         *
         * @param json the json data.
         * @param field_name the name of the field to read from.
         * @return uint32_t the parsed value.
         */
        static uint32_t parseUint32Field(Json::Value const& json,
            std::string const& field_name);

        /**
         * @brief Parses a string value read from a json field into uint64_t.
         *
         * @param json the json data.
         * @param field_name the name of the field to read from.
         * @return uint64_t the parsed value.
         */
        static uint64_t parseUint64Field(Json::Value const& json,
            std::string const& field_name,
            std::optional<uint64_t> default_value = std::optional<uint64_t>());

        /**
         * @brief Parses a string value read from a json field into bool.
         *
         * @param json the json data.
         * @param field_name the name of the field to read from.
         * @return bool the parsed value.
         */
        static bool parseBooleanField(Json::Value const& json,
            std::string const& field_name);

        /**
         * @brief Parses a string value read from a json field into base::Time.
         *
         * @param json the json data.
         * @param field_name the name of the field to read from.
         * @return base::Time the parsed value.
         */
        static base::Time parseTimeField(Json::Value const& json,
            std::string const& field_name);
    };

} // end namespace net_mikrotik

#endif // NET_MIKROTIK_RESTAPI_HPP
