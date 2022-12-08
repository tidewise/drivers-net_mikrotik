rock_library(network_status_monitoring
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(network_status_monitoring_bin Main.cpp
    DEPS network_status_monitoring)

