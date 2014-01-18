/// @author Владимир Керимов

#include <data/text.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_dozen)
    BOOST_AUTO_TEST_SUITE(test_text)

        BOOST_AUTO_TEST_CASE(test_text_create)
        {
            data::text first = L"Владимир";
            BOOST_CHECK(first.get_wide_string() == L"Владимир");
            BOOST_CHECK_EQUAL(first.get_byte_string(), "Владимир");

            data::text last = "Керимов";
            BOOST_CHECK_EQUAL(last.get_byte_string(), "Керимов");
            BOOST_CHECK(last.get_wide_string() == L"Керимов");
        }

    BOOST_AUTO_TEST_SUITE_END() // test_text
BOOST_AUTO_TEST_SUITE_END() // test_dozen
