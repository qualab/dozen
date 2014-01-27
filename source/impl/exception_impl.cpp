/// @author Владимир Керимов

#include "exception_impl.hpp"

namespace data
{
    exception::impl::impl()
        : m_message(), m_file(), m_line()
    {
    }

    exception::impl::impl(text message)
        : m_message(message), m_file(), m_line()
    {
    }

    exception::impl::impl(text message, text file, int line)
        : m_message(message), m_file(file), m_line(line)
    {
    }

    text exception::impl::get_message() const
    {
        return m_message;
    }

    text exception::impl::get_file() const
    {
        return m_file;
    }

    int exception::impl::get_line() const
    {
        return m_line;
    }
}
