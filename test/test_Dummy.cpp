#include <boost/test/unit_test.hpp>
#include <net_mikrotik/Dummy.hpp>

using namespace net_mikrotik;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    net_mikrotik::DummyClass dummy;
    dummy.welcome();
}
