/// @author Владимир Керимов

#include <data/value.hpp>
#include <data/null.hpp>
#include <typeinfo>
#include <memory>

namespace data
{
    class value::impl
    {
    public:
        impl()
            : m_kind(of_null)
        {
        }

        impl(impl const& another)
            : m_kind(another.m_kind)
        {
            if (!another.is_null())
            {
                m_holder = another.m_holder->clone();
            }
            else
            {
                m_holder.reset();
            }
        }

        void set_value(null_type)
        {
            m_kind = of_null;
            m_holder.reset(null);
        }

        template <typename value_type>
        void set_value(value_type another)
        {
            m_kind = typename value::kind_of<value_type>::value_kind;
            m_holder.reset(new nullable<value_type>(another));
        }

        template <typename value_type>
        static value_type extract_value(nullable<value_type> const& holder) 
        {
            return holder.get_value();
        }

        template <typename value_type>
        value_type get_value() const
        {
            if (!m_holder)
                throw 1; // TODO: exception throw
            return extract_value(dynamic_cast<nullable<value_type> const&>(*m_holder));
        }

        bool is_null() const
        {
            return !m_holder;
        }

    private:
        kind m_kind;
        std::unique_ptr<object> m_holder;
    };

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
        return m_impl->get_value<value::type_of<value::of_bool>::value_type>();
    }

    template<> value::type_of<value::of_float>::value_type value::get_value() const
    {
        return m_impl->get_value<value::type_of<value::of_bool>::value_type>();
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
