#include <data/exception.hpp>
#include <data/text.hpp>

namespace data
{
    class exception::impl
    {
    public:
        static const int line_is_not_set = 0;

        impl()
            : m_line(line_is_not_set)
        {
        }

        impl(text const& message, char const* file, int line)
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

    exception::exception()
    {
    }

    exception::~exception()
    {
    }

    exception::exception(char const* message, char const* file, int line)
        : m_impl(new impl(message, file, line))
    {
    }

    exception::exception(wchar_t const* message, char const* file, int line)
        : m_impl(new impl(message, file, line))
    {
    }

    exception::exception(text const& message, char const* file, int line)
        : m_impl(new impl(message, file, line))
    {
    }

    char const* exception::what() const
    {
        return m_impl->get_message().get_ansi_string_ptr();
    }

    std::unique_ptr<object> exception::clone() const
    {
        return std::unique_ptr<object>(new exception(*this));
    }
}
