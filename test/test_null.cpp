#include <data/null.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_dozen)
    BOOST_AUTO_TEST_SUITE(test_null)

        BOOST_AUTO_TEST_CASE(test_nullable_create)
        {
            data::nullable<int> intable; // :)
            BOOST_CHECK(intable.is_null());
            BOOST_CHECK(!intable.is_not_null());
            BOOST_CHECK(!intable);

            static const float FLOAT_VALUE = 1.1f;

            data::nullable<float> floatable(FLOAT_VALUE); // :)
            BOOST_REQUIRE(floatable.is_not_null());
            BOOST_CHECK(!floatable.is_null());
            BOOST_CHECK(!!floatable);
            BOOST_CHECK_EQUAL(floatable.get_value(), FLOAT_VALUE);
            BOOST_CHECK_EQUAL(*floatable, FLOAT_VALUE);
            BOOST_CHECK_EQUAL(floatable.get_value_ref(), FLOAT_VALUE);
            BOOST_CHECK_EQUAL(floatable.get_value_const_ref(), FLOAT_VALUE);
            BOOST_CHECK_EQUAL(&floatable.get_value_ref(), &*floatable);
            BOOST_CHECK_EQUAL(&floatable.get_value_const_ref(), &*floatable);

            data::nullable<bool> tribool(data::null);
            BOOST_CHECK(tribool.is_null());
            BOOST_CHECK(!tribool.is_not_null());
            BOOST_CHECK(!intable);
        }

        BOOST_AUTO_TEST_CASE(test_nullable_assignment)
        {
            static const int INT_NUMBER = 12345;

            data::nullable<int> number(INT_NUMBER);
            BOOST_REQUIRE(number.is_not_null());
            BOOST_REQUIRE(!number.is_null());
            BOOST_REQUIRE(!!number);
            BOOST_CHECK_EQUAL(number.get_value(), INT_NUMBER);
            BOOST_CHECK_EQUAL(*number, INT_NUMBER);
            BOOST_CHECK_EQUAL(number.get_value_ref(), INT_NUMBER);
            BOOST_CHECK_EQUAL(number.get_value_const_ref(), INT_NUMBER);
            BOOST_CHECK_EQUAL(&number.get_value_ref(), &*number);
            BOOST_CHECK_EQUAL(&number.get_value_const_ref(), &*number);

            static const int BIG_INT_NUMBER = 100500;

            BOOST_REQUIRE_NO_THROW((number = BIG_INT_NUMBER));
            BOOST_REQUIRE(!!number);
            BOOST_REQUIRE(!number.is_null());
            BOOST_REQUIRE(number.is_not_null());
            BOOST_CHECK_EQUAL(*number, BIG_INT_NUMBER);
            BOOST_CHECK_EQUAL(number.get_value_const_ref(), BIG_INT_NUMBER);
            BOOST_CHECK_EQUAL(number.get_value(), BIG_INT_NUMBER);
            BOOST_CHECK_EQUAL(number.get_value_ref(), BIG_INT_NUMBER);
        }

    BOOST_AUTO_TEST_SUITE_END() // test_null
BOOST_AUTO_TEST_SUITE_END() // test_dozen
