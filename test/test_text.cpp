/// @author Владимир Керимов

#include <data/text>
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

            data::text second("\x80\xad\xa4\xe0\xa5\xa5\xa2\xa8\xe7", "cp866");
            BOOST_CHECK(second.get_wide_string() == L"Андреевич");
            BOOST_CHECK_EQUAL(second.get_byte_string("cp1251"), "\xc0\xed\xe4\xf0\xe5\xe5\xe2\xe8\xf7");
            BOOST_CHECK(second.get_wide_string() == L"Андреевич");
            BOOST_CHECK(second.get_byte_string() == "Андреевич");
        }

    BOOST_AUTO_TEST_SUITE_END() // test_text
BOOST_AUTO_TEST_SUITE_END() // test_dozen
