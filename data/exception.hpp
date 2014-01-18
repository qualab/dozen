/// @author Владимир Керимов

#pragma once

#include <data/text.hpp>
#include <exception>

#define DOZEN_THROW(exception, message) throw exception(message, __FILE__, __LINE__)

namespace data
{
    /// Base class for exceptions in the data handling process
    class DOZEN_API exception : public std::exception, public object
    {
    public:
        /// Lazy initialized exception object without any information
        exception();

        /// Trivial destructor inherited from base object class
        virtual ~exception() override;

        /// Clone exception object, must be overriden in any child class
        virtual std::unique_ptr<object> clone() const override;

        /// 
        exception(text const& message);

        /// Create
        exception(text const& message, text const& file, int line);

        virtual char const* what() const override;

        text get_message() const;

        text get_file() const;

        int get_line() const;

    private:
        class impl;

        lazy<impl> m_impl;
    };

    class DOZEN_API out_of_range : public exception
    {
    public:
        out_of_range();

    };
}
