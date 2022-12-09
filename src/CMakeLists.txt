rock_library(network_status_monitoring
    SOURCES MikrotikRouterStatusMonitor.cpp
    HEADERS MikrotikRouterStatusMonitor.hpp
    MikrotikRouterStatusMonitorConfig.hpp
    MikrotikRouterStatusResponse.hpp
    DEPS_PKGCONFIG restclient-cpp base-types jsoncpp)

