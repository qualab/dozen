/// @author Владимир Керимов

#include <data/null>
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

            static const int ANOTHER_NUMBER = 9001;

            data::nullable<int> another = ANOTHER_NUMBER;
            BOOST_REQUIRE(another.is_not_null());
            BOOST_CHECK_EQUAL(another.get_value(), ANOTHER_NUMBER);

            BOOST_REQUIRE_NO_THROW((number = another));
            BOOST_CHECK_EQUAL(number.get_value_ref(), another.get_value_const_ref());
            BOOST_CHECK_EQUAL(number.get_value_const_ref(), ANOTHER_NUMBER);
            BOOST_CHECK_NE(&*number, &*another);
        }

        class temporary_checker
        {
        public:
            temporary_checker()
                : m_from_temporary(false) {
            }

            temporary_checker(temporary_checker const& another)
                : m_from_temporary(false) {
            }

            temporary_checker(temporary_checker&& temporary)
                : m_from_temporary(true) {
            }

            temporary_checker& operator = (temporary_checker const& another) {
                m_from_temporary = false;
                return *this;
            }

            temporary_checker& operator = (temporary_checker&& temporary) {
                m_from_temporary = true;
                return *this;
            }

            bool is_from_temporary() const {
                return m_from_temporary;
            }

            bool is_not_from_temporary() const {
                return !is_from_temporary();
            }

        private:
            bool m_from_temporary;
        };

        BOOST_AUTO_TEST_CASE(test_nullable_from_temporary)
        {
            data::nullable<temporary_checker> first;
            BOOST_CHECK(first.is_null());

            data::nullable<temporary_checker> second = temporary_checker();
            BOOST_REQUIRE(second.is_not_null());
            BOOST_CHECK(second->is_from_temporary());

            temporary_checker some_checker;

            data::nullable<temporary_checker> third = some_checker;
            BOOST_REQUIRE(third.is_not_null());
            BOOST_CHECK(third->is_not_from_temporary());

            BOOST_REQUIRE_NO_THROW((first = second));
            BOOST_CHECK(first->is_not_from_temporary());

            BOOST_REQUIRE_NO_THROW((first = std::move(third)));
            BOOST_CHECK(first->is_from_temporary());
        }

    BOOST_AUTO_TEST_SUITE_END() // test_null
BOOST_AUTO_TEST_SUITE_END() // test_dozen
