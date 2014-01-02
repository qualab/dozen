/// @author Владимир Керимов

#pragma once

#include <data/scalar.hpp>
#include <cstddef>

namespace data
{
    typedef std::nullptr_t null_type;

    const null_type null = nullptr;

    template<typename T>
    class nullable
    {
    public:
        /// Type of value of which nullable<T> origin from
        typedef T value_type;

        /// Size of value (in bytes) of which nullable<T> origin from
        static const size_t VALUE_SIZE = sizeof(value_type);

        /// Create nullable without value, in another words it equals to null
        nullable();

        /// Clear inplaced value (scalar destructor!)
        ~nullable();

        /// Create nullable with specific value, so it is not null
        nullable(const value_type& value);

        /// Create nullable with value of null
        nullable(null_type);

        /// Copy value of nullable without temporary object magic
        nullable(const nullable& another);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (const nullable& another);

        /// Copy value of nullable without temporary object magic
        nullable(nullable&& temporary);

        /// Copy value of nullable without temporary object magic
        nullable& operator = (nullable&& temporary);

        /// Access to the methods of contained value object (must be not null!)
        value_type* operator -> ();

        /// Access to the constant methods of contained value object (must be not null!)
        const value_type* operator -> () const;

        /// Access to the contained value object (must be not null!)
        value_type* operator -> ();

        /// Access to the constant contained value object (must be not null!)
        const value_type* operator -> () const;

        /// Check is has no value and this object equals null
        bool is_null() const;

        /// Check is has a value and this object not equals null
        bool is_not_null() const;

    private:
        value_type* m_value;
        byte m_buffer[VALUE_SIZE];
    };

// -------------------------------------------------------------------------- //

    template<typename T>
    nullable<T>::nullable()
        : m_value(null)
    {
    }

    template<typename T>
    nullable<T>::~nullable()
    {
        m_value->~value_type();
    }

    template<typename T>
    nullable<T>::nullable(const nullable<T>::value_type& value)
        : m_value(new(m_buffer) value_type(value))
    {
    }

    template<typename T>
    nullable<T>::nullable(null_type)
        : m_value(null)
    {
    }

    template<typename T>
    nullable<T>::nullable(const nullable<T>& another)
        : m_value(new(m_buffer) value_type(*another))
    {
    }

    template<typename T>
    nullable<T>& nullable<T>::nullable(const nullable<T>& another)
    {
        m_value = new(m_buffer) value_type(*another);
        return *this;
    }

    template<typename T>
    nullable<T>::nullable(nullable<T>&& temporary)
        : m_value(new(m_buffer) value_type(std::move(*temporary))
    {
    }

    template<typename T>
    nullable<T>& nullable<T>::operator = (const nullable<T>& temporary)
    {
        m_value = new(m_buffer) value_type(std::move(*temporary));
        return *this;
    }

    template<typename T>
    nullable<T>::value_type* nullable<T>::operator -> ()
    {
        return m_value;
    }

    template<typename T>
    const nullable<T>::value_type* nullable<T>::operator -> () const
    {
        return m_value;
    }
}
