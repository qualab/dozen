#include <data/null.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_dozen)
    BOOST_AUTO_TEST_SUITE(test_null)

        BOOST_AUTO_TEST_CASE(test_nullable_create)
        {
            data::nullable<int> intable;
            BOOST_CHECK(intable.is_null());
            BOOST_CHECK(!intable.is_not_null());
        }

    BOOST_AUTO_TEST_SUITE_END() // test_null
BOOST_AUTO_TEST_SUITE_END() // test_dozen
