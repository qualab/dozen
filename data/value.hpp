/// @author Владимир Керимов

#pragma once

#include <data/api.hpp>
#include <data/object.hpp>
#include <data/null.hpp>
#include <data/lazy.hpp>
#include <type_traits>

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

        /// Trivial destructor of data object
        virtual ~value();

        /// Initialize value as null, same as default constructor
        value(null_type);

        /// Initialize value by nullable holder of specified type
        template <typename value_type>
        value(nullable<value_type> const& holder);

        /// Initialize value by another of specified type
        template <typename value_type>
        value(value_type another);

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

        /// Kind of value, enum specifies value type
        enum kind
        {
            of_null     =  0,
            of_bool     =  1,
            of_int      =  2,
            of_float    =  3,
            of_text     =  4,
            of_unknown  = -1
        };

        template <kind of_kind>
        struct type_of;

        template <typename value_type>
        struct kind_of;

    private:
        /// Class of implementation, data is hidden.
        class impl;

        /// The only data in class with lazy initialization and copy-on-write system
        lazy<impl> m_impl;
    };

    template<> struct value::type_of<value::of_null>  { typedef null_type value_type; };
    template<> struct value::type_of<value::of_bool>  { typedef bool      value_type; };
    template<> struct value::type_of<value::of_int>   { typedef long long value_type; };
    template<> struct value::type_of<value::of_float> { typedef double    value_type; };

    template<> struct value::kind_of<value::type_of<value::of_null >::value_type> { static const kind value_kind = of_null;  };
    template<> struct value::kind_of<value::type_of<value::of_bool >::value_type> { static const kind value_kind = of_bool;  };
    template<> struct value::kind_of<value::type_of<value::of_int  >::value_type> { static const kind value_kind = of_int;   };
    template<> struct value::kind_of<value::type_of<value::of_float>::value_type> { static const kind value_kind = of_float; };

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

    template<> DOZEN_API void value::set_value(value::type_of<value::of_bool >::value_type another);
    template<> DOZEN_API void value::set_value(value::type_of<value::of_int  >::value_type another);
    template<> DOZEN_API void value::set_value(value::type_of<value::of_float>::value_type another);

    template <typename value_type>
    void value::set_value(value_type another)
    {
        if (std::is_integral<value_type>::value)
        {
            set_value(static_cast<value::type_of<value::of_int>::value_type>(another));
        }
        else if (std::is_floating_point<value_type>::value)
        {
            set_value(static_cast<value::type_of<value::of_float>::value_type>(another));
        }
        else
        {
            throw 1; // TODO: exception throw
        }
    }

    template <typename value_type>
    nullable<value_type> value::get_value_or_null() const
    {
        return is_null() ? null : get_value<value_type>();
    }

    template<> DOZEN_API value::type_of<value::of_bool >::value_type value::get_value() const;
    template<> DOZEN_API value::type_of<value::of_int  >::value_type value::get_value() const;
    template<> DOZEN_API value::type_of<value::of_float>::value_type value::get_value() const;

    template <typename value_type>
    value_type value::get_value() const
    {
        if (std::is_integral<value_type>::value)
        {
            return get_value<value::type_of<value::of_int>::value_type>();
        }
        else if (std::is_floating_point<value_type>::value)
        {
            return get_value<value::type_of<value::of_float>::value_type>();
        }
        else
        {
            throw 1; // TODO: exception throw
        }
    }
}
