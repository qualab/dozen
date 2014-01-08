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
                std::strncpy(m_message, message, sizeof(m_message));

            }

            static const char* default_message() {
                return ">> no message <<";
            }

            static const int MAX_MESSAGE_LENGTH = 0x100;

        private:
            char m_message[MAX_MESSAGE_LENGTH];
        };

        BOOST_AUTO_TEST_CASE(test_lazy_create)
        {
            data::lazy<test_impl> lazy_impl;
            BOOST_CHECK(lazy_impl.is_not_created());
            BOOST_CHECK_EQUAL(0, std::strcmp(lazy_impl->get_message(), test_impl::default_message()));
            BOOST_CHECK(lazy_impl.is_created());
        }

        BOOST_AUTO_TEST_CASE(test_lazy_copy)
        {
            static const char* check_me_message = "!check me!";

            data::lazy<test_impl> lazy_impl(new test_impl(check_me_message));
            BOOST_REQUIRE_EQUAL(0, std::strcmp(lazy_impl->get_message(), check_me_message));

            data::lazy<test_impl> another_lazy_impl = lazy_impl;

            BOOST_CHECK_EQUAL(2, lazy_impl.ref_count());
            BOOST_CHECK_EQUAL(2, another_lazy_impl.ref_count());

            data::lazy<test_impl> const& ref_lazy_impl = lazy_impl;

            BOOST_REQUIRE_EQUAL(0, std::strcmp(ref_lazy_impl->get_message(), check_me_message));

            BOOST_CHECK_EQUAL(2, lazy_impl.ref_count());
            BOOST_CHECK_EQUAL(2, another_lazy_impl.ref_count());

            static const char* another_message = "another message";

            BOOST_REQUIRE_NO_THROW(another_lazy_impl->set_message(another_message));

            BOOST_CHECK_EQUAL(0, std::strcmp(another_lazy_impl->get_message(), another_message));
            BOOST_CHECK_EQUAL(0, std::strcmp(lazy_impl->get_message(), check_me_message));

        }

    BOOST_AUTO_TEST_SUITE_END() // test_lazy
BOOST_AUTO_TEST_SUITE_END() // test_dozen
