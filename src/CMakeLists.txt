rock_library(net_mikrotik
    SOURCES RESTAPI.cpp
    HEADERS RESTAPI.hpp
    RESTAPIConfig.hpp
    InterfaceStats.hpp
    DEPS_PKGCONFIG restclient-cpp base-types jsoncpp)

rock_executable(net_mikrotik_bin Main.cpp DEPS net_mikrotik)