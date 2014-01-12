/// @author Владимир Керимов

#pragma once

#include <exception>
#include <data/object.hpp>
#include <data/lazy.hpp>

namespace data
{
    class exception : public std::exception, public object
    {
    public:
        enum level
        {
            AS_ERROR,
            AS_WARNING,
            AS_INFO,
            AS_DEBUG
        };

        exception(const char* message, level how, const char* file_from, int line_no);

        exception(const wchar_t* message, level how, const char* file_from, int line_no);

        virtual ~exception();

        const char* what() const;

        const wchar_t* message() const;

    private:
        class impl;

        lazy<impl> m_impl;
    };
}

#define DOZEN_THROW_ERROR(exception_class, error_message) throw exception_class(error_message, exception::IS_ERROR, __FILE__, __LINE__)

#define DOZEN_THROW_INFO(exception_class, info_message) throw exception_class(info_message, exception::IS_INFO, __FILE__, __LINE__)

#define DOZEN_THROW_WARNING(exception_class, warning_message) throw exception_class(warning_message, exception::IS_WARNING, __FILE__, __LINE__)

#define DOZEN_THROW_DEBUG(exception_class, debug_message) throw exception_class(debug_message, exception::IS_DEBUG, __FILE__, __LINE__)
