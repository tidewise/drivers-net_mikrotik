find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS})

find_package (CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

rock_library(network_status_monitoring
SOURCES SatcommsStatus.cpp
HEADERS SatcommsStatus.hpp)

target_link_libraries(network_status_monitoring ${Boost_LIBRARIES})
target_link_libraries(network_status_monitoring ${CURL_LIBRARIES})

rock_executable(network_status_monitoring_bin Main.cpp
    DEPS network_status_monitoring)

