#define BOOST_UT_DISABLE_MODULE // until all toolchains are ready
//#include <https://raw.githubusercontent.com/boost-experimental/ut/master/include/boost/ut.hpp>
#include "ut.hpp" // import boost.ut;
namespace ut = boost::ut;

#define TEST_UNITS
#include "ascii_predicates.hpp" // ascii::is_*

int main()
{
}
