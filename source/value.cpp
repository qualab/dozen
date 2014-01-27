/// @author Владимир Керимов

#include <data/value.hpp>
#include "impl/value_impl.hpp"
#include <data/null.hpp>
#include <typeinfo>
#include <memory>

namespace data
{
    value::value()
    {
    }

    value::~value()
    {
    }

    value::value(null_type)
    {
    }

    void value::set_value(null_type)
    {
        m_impl->set_value(null);
    }

    template<> void value::set_value(value::type_of<value::of_bool>::value_type another)
    {
        m_impl->set_value(another);
    }

    template<> void value::set_value(value::type_of<value::of_int>::value_type another)
    {
        m_impl->set_value(another);
    }

    template<> void value::set_value(value::type_of<value::of_float>::value_type another)
    {
        m_impl->set_value(another);
    }

    template<> value::type_of<value::of_bool>::value_type value::get_value() const
    {
        return m_impl->get_value<value::type_of<value::of_bool>::value_type>();
    }

    template<> value::type_of<value::of_int>::value_type value::get_value() const
    {
        return m_impl->get_value<value::type_of<value::of_int>::value_type>();
    }

    template<> value::type_of<value::of_float>::value_type value::get_value() const
    {
        return m_impl->get_value<value::type_of<value::of_float>::value_type>();
    }

    bool value::is_null() const
    {
        return m_impl->is_null();
    }

    bool value::is_not_null() const
    {
        return !is_null();
    }

    bool value::is_true() const
    {
        return get_value<bool>();
    }

    bool value::is_false() const
    {
        return !is_true();
    }
}
