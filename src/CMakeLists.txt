rock_library(net_mikrotik
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(net_mikrotik_bin Main.cpp
    DEPS net_mikrotik)

