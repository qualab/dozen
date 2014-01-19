/// @author Владимир Керимов

#include <data/exception.hpp>
#include <data/text.hpp>

namespace data
{
    class exception::impl
    {
    public:
        impl()
            : m_message(), m_file(), m_line()
        {
        }

        impl(text message)
            : m_message(message), m_file(), m_line()
        {
        }

        impl(text message, text file, int line)
            : m_message(message), m_file(file), m_line(line)
        {
        }

        text get_message() const
        {
            return m_message;
        }

        text get_file() const
        {
            return m_file;
        }

        int get_line() const
        {
            return m_line;
        }

    private:
        text m_message;
        text m_file;
        int  m_line;
    };

//  exception --------------------------------------------------------------------

    exception::exception()
    {
    }

    exception::~exception()
    {
    }

    exception::exception(text message)
        : m_impl(new impl(message))
    {
    }

    exception::exception(text message, text file, int line)
        : m_impl(new impl(message, file, line))
    {
    }

    char const* exception::what() const
    {
        return m_impl->get_message().get_byte_string_ptr();
    }

    std::unique_ptr<object> exception::clone() const
    {
        return std::unique_ptr<object>(new exception(*this));
    }

    text exception::get_message() const
    {
        return m_impl->get_message();
    }

    text exception::get_file() const
    {
        return m_impl->get_file();
    }

    int exception::get_line() const
    {
        return m_impl->get_line();
    }

//  out_of_range -----------------------------------------------------------------

    out_of_range::out_of_range()
    {
    }

    out_of_range::out_of_range(text message)
        : base(message)
    {
    }

    out_of_range::out_of_range(text message, text file, int line)
        : base(message, file, line)
    {
    }

    std::unique_ptr<object> out_of_range::clone() const
    {
        return std::unique_ptr<object>(new out_of_range(*this));
    }

//  null_is_not_value -----------------------------------------------------------------

    null_is_not_value::null_is_not_value()
    {
    }

    null_is_not_value::null_is_not_value(text message)
        : base(message)
    {
    }

    null_is_not_value::null_is_not_value(text message, text file, int line)
        : base(message, file, line)
    {
    }

    std::unique_ptr<object> null_is_not_value::clone() const
    {
        return std::unique_ptr<object>(new null_is_not_value(*this));
    }

//  unexpected_value_type -----------------------------------------------------------------

    unexpected_value_type::unexpected_value_type()
    {
    }

    unexpected_value_type::unexpected_value_type(text message)
        : base(message)
    {
    }

    unexpected_value_type::unexpected_value_type(text message, text file, int line)
        : base(message, file, line)
    {
    }

    std::unique_ptr<object> unexpected_value_type::clone() const
    {
        return std::unique_ptr<object>(new unexpected_value_type(*this));
    }
}
