#include <boost/test/unit_test.hpp>
#include <network_status_monitoring/Dummy.hpp>

using namespace network_status_monitoring;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    network_status_monitoring::DummyClass dummy;
    dummy.welcome();
}
