/// @author Владимир Керимов

#include <data/value.hpp>
#include <data/null.hpp>
#include <memory>

namespace data
{
    class value::impl
    {
    public:
        impl()
        {
        }

        impl(impl const& another)
        {
            if (!another.is_null())
            {//set_value_or_null(*another.m_holder);
            }//TODO: use overload (same as in visitor)
        }

        void set_value(null_type)
        {
            m_holder.reset(null);
        }

        template <typename value_type>
        void set_value(value_type another)
        {
            m_holder.reset(new nullable<value_type>(another));
        }

        template <typename value_type>
        void set_value_or_null(nullable<value_type> const& holder)
        {
            m_holder.reset(new nullable<value_type>(holder));
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
        std::unique_ptr<object> m_holder;
    };

    value::value()
    {
    }

    value::value(null_type)
    {
    }

    value::~value()
    {
    }

    void value::set_value(null_type)
    {
        m_impl->set_value(null);
    }

    template<> void value::set_value(bool another)
    {
        m_impl->set_value(another);
    }

    template<> void value::set_value(long long another)
    {
        m_impl->set_value(another);
    }

    template<> void value::set_value(double another)
    {
        m_impl->set_value(another);
    }

    template<> bool value::get_value() const
    {
        return m_impl->get_value<bool>();
    }

    template<> long long value::get_value() const
    {
        return m_impl->get_value<long long>();
    }

    template<> double value::get_value() const
    {
        return m_impl->get_value<double>();
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
