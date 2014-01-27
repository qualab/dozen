/// @author Владимир Керимов

#include <data/text.hpp>
#include "impl/text_impl.hpp"

namespace data
{

//  text -------------------------------------------------------------------------

    text::text()
    {
    }

    text::~text()
    {
    }

    std::unique_ptr<object> text::clone() const
    {
        return std::unique_ptr<object>(new text(*this));
    }

    text::text(wchar_t const* wide_string)
        : m_impl(new impl(wide_string))
    {
    }

    text::text(char const* byte_string)
        : m_impl(new impl(byte_string, null))
    {
    }

    text::text(char const* byte_string, char const* encoding)
        : m_impl(new impl(byte_string, encoding))
    {
    }

    text::text(std::wstring const& wide_string)
        : m_impl(new impl(wide_string.c_str()))
    {
    }

    text::text(std::string const& ansi_string)
        : m_impl(new impl(ansi_string.c_str(), null))
    {
    }

    text::text(std::string const& ansi_string, char const* encoding)
        : m_impl(new impl(ansi_string.c_str(), encoding))
    {
    }

    text::text(std::string const& ansi_string, std::string const& encoding)
        : m_impl(new impl(ansi_string.c_str(), encoding.c_str()))
    {
    }

    std::wstring text::get_wide_string() const
    {
        return m_impl->get_wide_string_ref();
    }

    std::string text::get_byte_string() const
    {
        return m_impl->get_byte_string_ref(null);
    }

    std::string text::get_byte_string(char const* encoding) const
    {
        return m_impl->get_byte_string_ref(encoding);
    }

    std::string text::get_byte_string(std::string const& encoding) const
    {
        return m_impl->get_byte_string_ref(encoding.c_str());
    }

    char const* text::get_byte_string_ptr() const
    {
        return m_impl->get_byte_string_ref(null).c_str();
    }

    char const* text::get_byte_string_ptr(char const* encoding) const
    {
        return m_impl->get_byte_string_ref(encoding).c_str();
    }

    char const* text::get_byte_string_ptr(std::string const& encoding) const
    {
        return m_impl->get_byte_string_ref(encoding.c_str()).c_str();
    }

    int text::get_length() const
    {
        return m_impl->get_length();
    }

    symbol text::get_symbol_at(int index) const
    {
        return m_impl->get_symbol_at(index);
    }

    symbol text::operator [] (int index) const
    {
        return m_impl->get_symbol_at(index);
    }

/// symbol_base ----------------------------------------------------------------

    wchar_t symbol_base::get_as_wide_char() const
    {
        int code = get_code();
        if (code > static_cast<int>(std::numeric_limits<wchar_t>::max()))
            DOZEN_THROW(out_of_range, "Symbol code is out of the wide character's range.");
        return static_cast<wchar_t>(code);
    }

    char symbol_base::get_as_byte_char(std::string const& encoding) const
    {
        return get_as_byte_char(encoding.c_str());
    }

    char symbol_base::get_as_byte_char(char const* encoding) const
    {
        int code = get_code();
        icu::UnicodeString unicode(code);
        char result[2];
        unicode.extract(0, 1, result, encoding);
        return result[0];
    }

    char symbol_base::get_as_byte_char() const
    {
        return get_as_byte_char(nullptr);
    }

    text symbol_base::get_as_text() const
    {
        return text(*this);
    }

    symbol_base::operator int() const
    {
        return get_code();
    }

    symbol_base::operator wchar_t() const
    {
        return get_as_wide_char();
    }

    symbol_base::operator char() const
    {
        return get_as_byte_char();
    }

    symbol_base::operator bool() const
    {
        return get_code() != 0;
    }

    symbol_base::operator text() const
    {
        return get_as_text();
    }

    void symbol_base::set_as_wide_char(wchar_t code)
    {
        return set_code(code);
    }

    void symbol_base::set_as_byte_char(char code, std::string const& encoding)
    {
        return set_as_byte_char(code, encoding.c_str());
    }

    void symbol_base::set_as_byte_char(char code, char const* encoding)
    {
        icu::UnicodeString unicode(&code, 1, encoding);
    }

    void symbol_base::set_as_byte_char(char code)
    {
        return set_as_byte_char(code, nullptr);
    }

    void symbol_base::set_as_text(text single)
    {
        *this = single.get_symbol_at(0);
    }

    symbol_base& symbol_base::operator = (int code)
    {
        set_code(code);
        return *this;
    }

    symbol_base& symbol_base::operator = (wchar_t code)
    {
        set_as_wide_char(code);
        return *this;
    }

    symbol_base& symbol_base::operator = (char code)
    {
        set_as_byte_char(code);
        return *this;
    }

    symbol_base& symbol_base::operator = (text single)
    {
        set_as_text(single);
        return *this;
    }

//  symbol ---------------------------------------------------------------------

    symbol::symbol()
    {
    }

    symbol::~symbol()
    {
    }

    std::unique_ptr<object> symbol::clone() const
    {
        return std::unique_ptr<object>(new symbol(*this));
    }

    int symbol::get_code() const
    {
        return m_impl->get_code();
    }

    void symbol::set_code(int code)
    {
        return m_impl->set_code(code);
    }

    symbol::symbol(int code)
        : m_impl(new impl(code))
    {
    }

    symbol::symbol(wchar_t code)
        : m_impl(new impl(code))
    {
    }

    symbol::symbol(char code, std::string const& encoding)
        : m_impl(new impl(code, encoding.c_str()))
    {
    }

    symbol::symbol(char code, char const* encoding)
        : m_impl(new impl(code, encoding))
    {
    }

    symbol::symbol(char code)
        : m_impl(new impl(code))
    {
    }

//  symbol_ref -----------------------------------------------------------------

    symbol_ref::symbol_ref(text source, int index)
        : m_impl(new impl(source, index))
    {
    }

    symbol_ref::~symbol_ref()
    {
    }

    std::unique_ptr<object> symbol_ref::clone() const
    {
        return std::unique_ptr<object>(new symbol_ref(*this));
    }

    int symbol_ref::get_code() const
    {
        return m_impl->get_code();
    }

    void symbol_ref::set_code(int code)
    {
        return m_impl->set_code(code);
    }
}
