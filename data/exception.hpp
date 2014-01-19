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

        /// Inherited from std::exception, returns the pointer to byte string of message
        virtual char const* what() const override;

        /// Create exception object with message but without information about its origin point
        exception(text message);

        /// Create exception object with message and information about its origin point
        exception(text message, text file, int line);

        /// Get message of exception
        text get_message() const;

        /// Get file name of the exception origin
        text get_file() const;

        /// Get line number in the file of the exception origin
        int get_line() const;

    private:
        class impl;

        lazy<impl> m_impl;
    };

    /// Out of range exception occurs when some value is out of its possible range
    class DOZEN_API out_of_range : public exception
    {
    public:
        /// Type of base class
        typedef exception base;

        /// Lazy initialized exception object without any information
        out_of_range();

        /// Create exception object with message but without information about its origin point
        out_of_range(text message);

        /// Create exception object with message and information about its origin point
        out_of_range(text message, text file, int line);

        /// Clone exception object, must be overriden in any child class
        virtual std::unique_ptr<object> clone() const override;
    };

    /// If nullable variable is null but something try to work with value this exception occurs
    class DOZEN_API null_is_not_value : public exception
    {
    public:
        /// Type of base class
        typedef exception base;

        /// Lazy initialized exception object without any information
        null_is_not_value();

        /// Create exception object with message but without information about its origin point
        null_is_not_value(text message);

        /// Create exception object with message and information about its origin point
        null_is_not_value(text message, text file, int line);

        /// Clone exception object, must be overriden in any child class
        virtual std::unique_ptr<object> clone() const override;
    };

    /// When unexpected type of value received this exception occurs
    class DOZEN_API unexpected_value_type : public exception
    {
    public:
        /// Type of base class
        typedef exception base;

        /// Lazy initialized exception object without any information
        unexpected_value_type();

        /// Create exception object with message but without information about its origin point
        unexpected_value_type(text message);

        /// Create exception object with message and information about its origin point
        unexpected_value_type(text message, text file, int line);

        /// Clone exception object, must be overriden in any child class
        virtual std::unique_ptr<object> clone() const override;
    };
}
