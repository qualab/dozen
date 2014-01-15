/// @author Владимир Керимов

#include <data/text.hpp>
#include <data/null.hpp>

namespace data
{
    class text::impl
    {
    public:
        impl();

        impl(wchar_t const* wide_string);
        impl(char    const* ansi_string);

        impl(std::wstring const& wide_string);
        impl(std::string  const& ansi_string);

        std::wstring const& get_wide_string_ref() const;
        std::string  const& get_ansi_string_ref() const;

        int get_symbol_at(int index) const;
        int get_length() const;

    private:
        mutable nullable<std::wstring> m_wide_string;
        mutable nullable<std::string>  m_ansi_string;
    };

    text::impl::impl()
    {
    }

    text::impl::impl(char const* ansi_string)
        : m_ansi_string(ansi_string)
    {
    }

    text::impl::impl(wchar_t const* wide_string)
        : m_wide_string(wide_string)
    {
    }

    text::impl::impl(std::string const& ansi_string)
        : m_ansi_string(ansi_string)
    {
    }

    text::impl::impl(std::wstring const& wide_string)
        : m_wide_string(wide_string)
    {
    }

    std::wstring const& text::impl::get_wide_string_ref() const
    {
        if (!m_wide_string)
        {
            if (!m_ansi_string)
            {
                m_wide_string = std::wstring();
            }
            else
            {
                // TODO: use libicu or libiconv
                m_wide_string = std::wstring(m_ansi_string->size(), L'\0');
                auto count = std::mbstowcs(&m_wide_string->at(0), m_ansi_string->c_str(), m_wide_string->size());
                m_wide_string->resize(count);
            }
        }
        return *m_wide_string;
    }

    std::string const& text::impl::get_ansi_string_ref() const
    {
        if (!m_ansi_string)
        {
            if (!m_wide_string)
            {
                m_ansi_string = std::string();
            }
            else
            {
                // TODO: use libicu or libiconv
                m_ansi_string = std::string(m_wide_string->size(), '\0');
                auto count = std::wcstombs(&m_ansi_string->at(0), m_wide_string->c_str(), m_ansi_string->size());
                m_ansi_string->resize(count);
            }
        }
        return *m_ansi_string;
    }

    int text::impl::get_length() const
    {
        return get_wide_string_ref().size();
    }

    int text::impl::get_symbol_at(int index) const
    {
        if (index >= 0)
        {
            if (index >= get_length())
                throw 1; // TODO: exception "out of range"
            else
                return get_wide_string_ref().at(index);
        }
        else
        {
            if (index < get_length())
                throw 1; // TODO: exception "out of range"
            else
                return get_wide_string_ref().at(get_length() - index);
        }
    }

    text::text()
    {
    }

    text::~text()
    {
    }

    text::text(char const* ansi_string)
        : m_impl(new impl(ansi_string))
    {
    }

    text::text(wchar_t const* wide_string)
        : m_impl(new impl(wide_string))
    {
    }

    text::text(std::string const& ansi_string)
        : m_impl(new impl(ansi_string))
    {
    }

    text::text(std::wstring const& wide_string)
        : m_impl(new impl(wide_string))
    {
    }

    std::string text::get_ansi_string() const
    {
        return m_impl->get_ansi_string_ref();
    }

    std::wstring text::get_wide_string() const
    {
        return m_impl->get_wide_string_ref();
    }

    char const* text::get_ansi_string_ptr() const
    {
        return m_impl->get_ansi_string_ref().c_str();
    }

    wchar_t const* text::get_wide_string_ptr() const
    {
        return m_impl->get_wide_string_ref().c_str();
    }

    int text::operator [] (int index) const
    {
        return m_impl->get_symbol_at(index);
    }

    int text::get_symbol_at(int index) const
    {
        return m_impl->get_symbol_at(index);
    }

    int text::get_length() const
    {
        return m_impl->get_length();
    }
}
