# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 3.9)
project(net_mikrotik
        VERSION 0.1
        DESCRIPTION "Monitoring libraries for network devices")
find_package(Rock)

set(CMAKE_CXX_STANDARD 17)

rock_init()
rock_standard_layout()
