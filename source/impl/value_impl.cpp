/// @author Владимир Керимов

#include "value_impl.hpp"

namespace data
{
    value::impl::impl()
        : m_kind(of_null)
    {
    }

    value::impl::impl(impl const& another)
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

    void value::impl::set_value(null_type)
    {
        m_kind = of_null;
        m_holder.reset(null);
    }

    bool value::impl::is_null() const
    {
        return !m_holder;
    }
}
