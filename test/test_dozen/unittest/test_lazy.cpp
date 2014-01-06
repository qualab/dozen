#include <data/lazy.hpp>
#include <boost/test/unit_test.hpp>
#include <cstring>

BOOST_AUTO_TEST_SUITE(test_dozen)
    BOOST_AUTO_TEST_SUITE(test_lazy)

        class test_impl
        {
        public:
            test_impl() {
                set_message(default_message());
            }

            test_impl(const char* message) {
                set_message(message);
            }
            
            const char* get_message() const {
                return m_message;
            }

            void set_message(const char* message) {
                std::strcpy(m_message, message);
            }

            static const char* default_message() {
                return ">> no message <<";
            }

        private:
            char m_message[256];
        };

        BOOST_AUTO_TEST_CASE(test_lazy_create)
        {
            data::lazy<test_impl> lazy_impl;
            BOOST_CHECK_EQUAL(0, std::strcmp(lazy_impl->get_message(), test_impl::default_message()));
        }

        BOOST_AUTO_TEST_CASE(test_lazy_copy)
        {
            static const char* check_me_message = "!check me!";

            data::lazy<test_impl> lazy_impl(new test_impl(check_me_message));
            BOOST_REQUIRE_EQUAL(0, std::strcmp(lazy_impl->get_message(), check_me_message));

            data::lazy<test_impl> another_lazy_impl = lazy_impl;
            BOOST_REQUIRE_EQUAL(0, std::strcmp(another_lazy_impl->get_message(), check_me_message));
        }

    BOOST_AUTO_TEST_SUITE_END() // test_lazy
BOOST_AUTO_TEST_SUITE_END() // test_dozen
