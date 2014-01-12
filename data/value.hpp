/// @author Владимир Керимов

#pragma once

#include <data/object.hpp>
#include <data/null.hpp>
#include <data/lazy.hpp>
#include <data/api.hpp>

namespace data
{
    /// Data value which is able to be a cell in query result or value in array of mixing types.
    /// Able to be one of the types: type of NULL, boolean, integer, floating-point, text, array, hash-array, nested data set or nested data unit.
    /// Able to be one of type described by developer using templates.
    class DOZEN_API value : public object
    {
    public:
        /// Lazy initialization, does not create any data but class object
        value();

        /// Initialize value as null, same as default constructor
        value(null_type);

        /// Initialize value by nullable holder of specified type
        template <typename value_type>
        value(nullable<value_type> const& holder);

        /// Initialize value by another of specified type
        template <typename value_type>
        value(value_type another);

        /// Trivial destructor of data object
        virtual ~value();

        /// Set value as null
        void set_value(null_type);

        /// Set value by nullable holder of specified type
        template <typename value_type>
        void set_value(nullable<value_type> const& holder);

        /// Set value by another of specified type
        template <typename value_type>
        void set_value(value_type another);

        /// Get value of specified type
        template <typename value_type>
        value_type get_value() const;

        /// Get value of specified type or null if null contained
        template <typename value_type>
        nullable<value_type> get_value_or_null() const;

        /// Check is value contains null
        bool is_null() const;

        /// Check is value not contains null
        bool is_not_null() const;

        /// Check is value contain true
        bool is_true() const;

        /// Check is value contain false
        bool is_false() const;

    private:
        /// Class of implementation, data is hidden.
        class impl;

        /// The only data in class with lazy initialization and copy-on-write system
        lazy<impl> m_impl;
    };

    template <typename value_type>
    value::value(nullable<value_type> const& holder)
    {
        set_value(holder);
    }

    template <typename value_type>
    value::value(value_type another)
    {
        set_value(another);
    }

    template<> DOZEN_API void value::set_value(bool      another);
    template<> DOZEN_API void value::set_value(long long another);
    template<> DOZEN_API void value::set_value(double    another);

    template<> void value::set_value(int another)
    {
        return set_value(static_cast<long long>(another));
    }

    template<> void value::set_value(float another)
    {
        return set_value(static_cast<double>(another));
    }

    template <typename value_type>
    nullable<value_type> value::get_value_or_null() const
    {
        return is_null() ? null : get_value();
    }

    template<> DOZEN_API bool      value::get_value() const;
    template<> DOZEN_API long long value::get_value() const;
    template<> DOZEN_API double    value::get_value() const;

    template<> int value::get_value() const
    {
        return get_value<long long>(); // TODO: safe cast
    }

    template<> float value::get_value() const
    {
        return get_value<double>(); // TODO: safe cast
    }
}
