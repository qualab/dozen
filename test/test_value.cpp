/// @author Владимир Керимов

#include <data/value.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_dozen)
    BOOST_AUTO_TEST_SUITE(test_value)

        BOOST_AUTO_TEST_CASE(test_value_create)
        {
            data::value first;
            BOOST_CHECK(first.is_null());

            data::value second(data::null);
            BOOST_CHECK(second.is_null());

            data::value third(true);
            BOOST_REQUIRE(third.is_not_null());
            BOOST_REQUIRE(!third.is_null());
            BOOST_CHECK(third.get_value<bool>());
            BOOST_CHECK(third.is_true());
            BOOST_CHECK(!third.is_false());

            static const int INT_VALUE = 123;

            data::value fourth(INT_VALUE);
            BOOST_REQUIRE(fourth.is_not_null());
            BOOST_REQUIRE(!fourth.is_null());
            BOOST_CHECK_EQUAL(fourth.get_value<long long>(), INT_VALUE);
            BOOST_CHECK_EQUAL(fourth.get_value<int>(), INT_VALUE);

            static const double FLOAT_VALUE = -1.024;

            data::value fifth(FLOAT_VALUE);
            BOOST_REQUIRE(!fifth.is_null());
            BOOST_REQUIRE(fifth.is_not_null());
            BOOST_CHECK_EQUAL(fifth.get_value<double>(), FLOAT_VALUE);
        }

    BOOST_AUTO_TEST_SUITE_END() // test_value
BOOST_AUTO_TEST_SUITE_END() // test_dozen
