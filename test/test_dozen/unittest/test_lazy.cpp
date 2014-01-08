#include <data/lazy.hpp>
#include <boost/test/unit_test.hpp>
#include <string>

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

        class huge
        {
        public:
            huge();
            huge(std::string const& name);

            std::string const& get_name() const;
            void set_name(std::string const& name);

            bool is_impl_created() const;
            bool is_impl_unique() const;
            int ref_impl_count() const;
            bool is_impl_same_as(huge const& another);

        private:
            class impl;
            data::lazy<impl> m_impl;
        };

        class huge::impl
        {
        public:
            impl();
            impl(std::string const& name);

            std::string const& get_name() const;
            void set_name(std::string const& name);

        private:
            std::string m_name;
        };

// -- implementation of huge::* methods --

        huge::huge()
        {
        }

        huge::huge(std::string const& name)
            : m_impl(new huge::impl(name))
        {
        }

        std::string const& huge::get_name() const
        {
            return m_impl->get_name();
        }

        void huge::set_name(std::string const& name)
        {
            m_impl->set_name(name);
        }

        bool huge::is_impl_created() const
        {
            return m_impl.is_created();
        }

        bool huge::is_impl_unique() const
        {
            return m_impl.is_unique();
        }

        int huge::ref_impl_count() const
        {
            return m_impl.ref_count();
        }

        bool huge::is_impl_same_as(huge const& another)
        {
            return m_impl.is_same_as(another.m_impl);
        }

// -- implementation of huge::impl::* methods --

        huge::impl::impl()
        {
        }

        huge::impl::impl(std::string const& name)
            : m_name(name)
        {
        }

        std::string const& huge::impl::get_name() const
        {
            return m_name;
        }

        void huge::impl::set_name(std::string const& name)
        {
            m_name = name;
        }

// -- continue test --

        BOOST_AUTO_TEST_CASE(test_huge_impl)
        {
            huge first;
            BOOST_CHECK(!first.is_impl_created());
            BOOST_CHECK_EQUAL(first.get_name(), std::string());
            BOOST_CHECK(first.is_impl_created());
            BOOST_CHECK(first.is_impl_unique());

            huge second = first;
            BOOST_CHECK(second.is_impl_same_as(first));
            BOOST_CHECK_EQUAL(2, second.ref_impl_count());
            BOOST_CHECK_EQUAL(2, first.ref_impl_count());

            BOOST_CHECK_EQUAL(second.get_name(), std::string());
            BOOST_CHECK_EQUAL(first.get_name(), std::string());
            BOOST_CHECK(first.is_impl_same_as(second));
            BOOST_CHECK_EQUAL(2, first.ref_impl_count());
            BOOST_CHECK_EQUAL(2, second.ref_impl_count());

            static const std::string FIRST_NAME = "first";

            BOOST_REQUIRE_NO_THROW(first.set_name(FIRST_NAME));
            BOOST_CHECK(!first.is_impl_same_as(second));
            BOOST_CHECK(first.is_impl_unique());
            BOOST_CHECK(second.is_impl_unique());
            BOOST_CHECK_EQUAL(first.get_name(), FIRST_NAME);
            BOOST_CHECK_EQUAL(second.get_name(), std::string());
            BOOST_CHECK_EQUAL(1, first.ref_impl_count());
            BOOST_CHECK_EQUAL(1, second.ref_impl_count());

            static const std::string SECOND_NAME = "second";

            BOOST_REQUIRE_NO_THROW(second.set_name(SECOND_NAME));
            BOOST_CHECK_EQUAL(second.get_name(), SECOND_NAME);
            BOOST_CHECK_EQUAL(first.get_name(), FIRST_NAME);
            BOOST_CHECK(second.is_impl_unique());
            BOOST_CHECK(first.is_impl_unique());
            BOOST_CHECK_EQUAL(1, first.ref_impl_count());
            BOOST_CHECK_EQUAL(1, second.ref_impl_count());

            huge third = second;

            BOOST_CHECK(second.is_impl_same_as(third));
            BOOST_CHECK(first.is_impl_unique());
            BOOST_CHECK(!second.is_impl_unique());
            BOOST_CHECK(!third.is_impl_unique());
        }

    BOOST_AUTO_TEST_SUITE_END() // test_lazy
BOOST_AUTO_TEST_SUITE_END() // test_dozen
