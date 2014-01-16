/// @author Владимир Керимов

#pragma once

#include <data/object.hpp>
#include <data/scalar.hpp>
#include <utility>

namespace data
{
    /// Constant of null value
    const auto null = nullptr;

    /// Type of null value
    typedef decltype(nullptr) null_type;

    /// Type allows null as variant of value for any type
    template <typename value_type>
    class nullable : public object
    {
    public:
        /// Size of type which is parameter of nullable template
        static const int value_size = sizeof(value_type);

        /// Create nullable without value, in another words it equals to null
        nullable();

        /// Clear inplaced value (scalar destructor!)
        virtual ~nullable();

        /// Clone this object to new nullable
        virtual std::unique_ptr<object> clone() const;

        /// Create nullable with value of null
        nullable(null_type);

        /// Create nullable with specific value, so it is not null
        nullable(value_type const& value);

        /// Copy value of nullable without temporary object magic
        nullable(value_type&& temporary);

        /// Copy value of nullable without temporary object magic
        nullable(nullable const& another);

        /// Copy value of nullable without temporary object magic
        nullable(nullable&& temporary);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (null_type);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (value_type const& another);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (value_type&& temporary);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (nullable const& another);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (nullable&& temporary);

        /// Access to the methods of contained value object, if object is not exist then exception occurs
        value_type* operator -> ();

        /// Access to the constant methods of contained value object, if object is not exist then exception occurs
        const value_type* operator -> () const;

        /// Reference to the contained value if not null, if it is null then exception occurs
        value_type& operator * ();

        /// Constant reference to the contained value if not null, if it is null then exception occurs
        const value_type& operator * () const;

        /// Determine when nullable is "false" and contains null value.
        bool operator ! () const;

        /// Check is two nullable contains equal values or both are null
        bool operator == (nullable const& another) const;

        /// Check is two nullable contains different values or one of them is null
        bool operator != (nullable const& another) const;

        /// Clear value if exists and set value to null
        void clear();

        /// Check is has no value and this object equals null
        bool is_null() const;

        /// Check is has a value and this object not equals null
        bool is_not_null() const;

        /// Return value if it is exist, if not then exception occurs
        value_type get_value() const;

        /// Reference to the contained value if not null, if it is null then exception occurs
        value_type& get_value_ref();

        /// Constant reference to the contained value if not null, if it is null then exception occurs
        const value_type& get_value_ref() const;

        /// Constant reference to the contained value if not null, if it is null then exception occurs
        const value_type& get_value_const_ref() const;

    private:
        value_type* m_value;
        byte m_buffer[value_size];
    };

    // -------------------------------------------------------------------------- //

    template <typename value_type>
    nullable<value_type>::nullable()
        : m_value(null)
    {
    }

    template <typename value_type>
    nullable<value_type>::~nullable()
    {
        clear();
        if (m_value)
            m_value->~value_type();
    }

    template <typename value_type>
    std::unique_ptr<object> nullable<value_type>::clone() const
    {
        return std::unique_ptr<object>(new nullable<value_type>(*this));
    }

    template <typename value_type>
    nullable<value_type>::nullable(null_type)
        : m_value(null)
    {
    }

    template <typename value_type>
    nullable<value_type>::nullable(value_type const& value)
        : m_value(new(m_buffer) value_type(value))
    {
    }

    template <typename value_type>
    nullable<value_type>::nullable(value_type&& temporary)
        : m_value(new(m_buffer) value_type(std::move(temporary)))
    {
    }

    template <typename value_type>
    nullable<value_type>::nullable(nullable<value_type> const& another)
        : m_value(new(m_buffer) value_type(*another))
    {
    }

    template <typename value_type>
    nullable<value_type>::nullable(nullable<value_type>&& temporary)
        : m_value(new(m_buffer) value_type(std::move(*temporary)))
    {
    }

    template <typename value_type>
    nullable<value_type>& nullable<value_type>::operator = (null_type)
    {
        clear();
        return *this;
    }

    template <typename value_type>
    nullable<value_type>& nullable<value_type>::operator = (value_type const& another)
    {
        clear();
        m_value = new(m_buffer) value_type(another);
        return *this;
    }

    template <typename value_type>
    nullable<value_type>& nullable<value_type>::operator = (value_type&& temporary)
    {
        clear();
        m_value = new(m_buffer) value_type(std::move(temporary));
        return *this;
    }

    template <typename value_type>
    nullable<value_type>& nullable<value_type>::operator = (nullable<value_type> const& another)
    {
        return *this = *another;
    }

    template <typename value_type>
    nullable<value_type>& nullable<value_type>::operator = (nullable<value_type>&& temporary)
    {
        return *this = std::move(*temporary);
    }

    template <typename value_type>
    value_type* nullable<value_type>::operator -> ()
    {
        return m_value;
    }

    template <typename value_type>
    const value_type* nullable<value_type>::operator -> () const
    {
        return m_value;
    }

    template <typename value_type>
    value_type& nullable<value_type>::operator * ()
    {
        return get_value_ref();
    }

    template <typename value_type>
    value_type const& nullable<value_type>::operator * () const
    {
        return get_value_ref();
    }

    template <typename value_type>
    bool nullable<value_type>::operator ! () const
    {
        return is_null();
    }

    template <typename value_type>
    bool nullable<value_type>::operator == (nullable<value_type> const& another) const
    {
        return (is_null() && another.is_null()) || 
            (is_not_null() && another.is_not_null() && get_value_ref() == another.get_value_ref());
    }

    template <typename value_type>
    bool nullable<value_type>::operator != (nullable<value_type> const& another) const
    {
        return !(*this == another);
    }

    template <typename value_type>
    void nullable<value_type>::clear()
    {
        if (m_value)
        {
            value_type* temporary = m_value;
            m_value = null;
            if (temporary)
                temporary->~value_type();
        }
    }

    template <typename value_type>
    bool nullable<value_type>::is_null() const
    {
        return m_value == null;
    }

    template <typename value_type>
    bool nullable<value_type>::is_not_null() const
    {
        return !is_null();
    }

    template <typename value_type>
    value_type nullable<value_type>::get_value() const
    {
        return get_value_ref();
    }

    template <typename value_type>
    value_type& nullable<value_type>::get_value_ref()
    {
        if (!m_value)
            throw 1; // TODO: exception
        return *m_value;
    }

    template <typename value_type>
    const value_type& nullable<value_type>::get_value_ref() const
    {
        if (!m_value)
            throw 1; // TODO: exception
        return *m_value;
    }

    template <typename value_type>
    const value_type& nullable<value_type>::get_value_const_ref() const
    {
        return get_value_ref();
    }
}
