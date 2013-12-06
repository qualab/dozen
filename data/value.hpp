/// @author Vladimir Kerimov

#pragma once

#include <data/lazy.hpp>

namespace data
{
    /// Data value which is able to be a cell in query result or value in array of mixing types.
    /// Able to be one of the types: type of NULL, boolean, integer, floating-point, text, array, hash-array, nested data set or nested data unit.
    /// Able to be one of type described by developer using templates.
    class value
    {
    public:
        /// Lazy initialization, does not create any data but class object
        value();

        /// Initialize by value without any references in the template
        template<typename T>
        value(T another);

    private:
        /// Class of implementation, data is hidden.
        class impl;

        /// The only data in class with lazy initialization and copy-on-write system
        lazy<impl> m_impl;
    };
}
