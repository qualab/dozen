/// @author Владимир Керимов

#include <data/exception.hpp>

namespace data
{
    class exception::impl
    {
    public:
        impl();
        impl(text message);
        impl(text message, text file, int line);

        text get_message() const;
        text get_file() const;
        int get_line() const;

    private:
        text m_message;
        text m_file;
        int  m_line;
    };
}
