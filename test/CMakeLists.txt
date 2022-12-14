rock_gtest(test_net_mikrotik
    suite.cpp
    test_RESTAPI.cpp
    DEPS net_mikrotik)

set_tests_properties(test-test_net_mikrotik-cxx PROPERTIES ENVIRONMENT
    "NET_MIKROTIK_RESOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/")
