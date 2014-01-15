/// @author Владимир Керимов

#pragma once

#include <data/text.hpp>
#include <exception>

#define DOZEN_THROW(exception, message) throw exception(message, __FILE__, __LINE__)

namespace data
{
    class DOZEN_API exception : public std::exception, public object
    {
    public:
        exception();

        virtual ~exception() override;

        exception(char const* message, char const* file, int line);

        exception(wchar_t const* message, char const* file, int line);

        exception(text const& message, char const* file, int line);

        virtual char const* what() const override;

        virtual std::unique_ptr<object> clone() const override;

    private:
        class impl;

        lazy<impl> m_impl;
    };
}
