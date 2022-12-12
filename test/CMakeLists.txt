rock_gtest(test_suite suite.cpp
   test_SatcommsStatus.cpp
   DEPS network_status_monitoring)

set_tests_properties(test-test_suite-cxx PROPERTIES ENVIRONMENT
   "SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}")