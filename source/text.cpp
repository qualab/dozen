/// @author Владимир Керимов

#include <data/text.hpp>
#include <data/null.hpp>

#if defined(DOZEN_USES_ICU)
#include <unicode/unistr.h>
#elif defined(DOZEN_USES_ICONV)
#include <iconv.h>
#else
#error Choose Unicode encoder: DOZEN_USES_ICU to use libicu or DOZEN_USES_ICONV to use libiconv
#endif

#ifdef _MSC_VER
#define DOZEN_WCHAR_SIZE 2
#endif

namespace data
{
    class text::impl
    {
    public:
        impl();

        impl(wchar_t const* wide_string);
        impl(char const* byte_string);
        impl(char const* byte_string, char const* encoding);

        std::wstring get_wide_string() const;
        std::string const& get_byte_string_ref(char const* encoding) const;

        int get_length() const;
        int get_symbol_at(int index) const;

    private:
#if defined(DOZEN_USES_ICU) && DOZEN_WCHAR_SIZE > 1
        mutable nullable<icu::UnicodeString> m_unicode_string;
#endif
        mutable nullable<std::string> m_byte_string;
        mutable nullable<std::string> m_encoding;

        void ensure_unicode_string_exists() const;
        void ensure_byte_string_match(char const* encoding) const;
    };

    text::impl::impl()
    {
    }

    text::impl::impl(wchar_t const* wide_string)
        : m_unicode_string(wide_string)
    {
    }

    text::impl::impl(char const* byte_string)
        : m_byte_string(byte_string)
    {
    }

    text::impl::impl(char const* byte_string, char const* encoding)
        : m_byte_string(byte_string), m_encoding(encoding)
    {
    }

    void text::impl::ensure_unicode_string_exists() const
    {
#if DOZEN_WCHAR_SIZE == 1
        ensure_byte_string_exists();
#endif
        if (m_unicode_string.is_null())
        {
            if (m_byte_string.is_null() || m_byte_string->empty())
            {
                m_unicode_string = icu::UnicodeString();
            }
            else
            {
                if (m_encoding.is_null())
                {
                    m_unicode_string = icu::UnicodeString(m_byte_string->c_str());
                }
                else
                {
                    m_unicode_string = icu::UnicodeString(m_byte_string->c_str(), m_encoding->c_str());
                }
            }
        }
    }

    void text::impl::ensure_byte_string_match(char const* encoding) const
    {
        if (m_byte_string.is_null() || m_encoding != nullable<std::string>(encoding))
        {
            m_encoding = encoding;
            m_byte_string = std::string();
            if (m_unicode_string.is_not_null())
            {
                m_byte_string->resize(4 * m_byte_string->size());
                int len = m_unicode_string->extract(0, get_length(), &m_byte_string->at(0));
                m_byte_string->resize(len);
            }
        }
    }

    std::wstring text::impl::get_wide_string() const
    {
#if   DOZEN_WCHAR_SIZE == 1
        static_assert(sizeof(wchar_t) == 1, "Macro defined as sizeof(wchar_t) == 1, but this condition is false!");
        return get_byte_string();
#elif DOZEN_WCHAR_SIZE == 2
        static_assert(sizeof(wchar_t) == 2, "Macro defined as sizeof(wchar_t) == 2, but this condition is false!");
        return std::wstring(m_unicode_string->getBuffer());
#elif DOZEN_WCHAR_SIZE == 4
        static_assert(sizeof(wchar_t) == 4, "Macro defined as sizeof(wchar_t) == 4, but this condition is false!");
        return std::wstring(m_unicode_string->toUTF32());
#else
#       error Unexpected size of wchar_t, out of 1, 2, 4 bytes variants
#endif
    }

    std::string const& text::impl::get_byte_string_ref(char const* encoding) const
    {
#if DOZEN_WCHAR_SIZE == 1
        static_assert(sizeof(wchar_t) == 1, "Macro defined as sizeof(wchar_t) == 1, but this condition is false!");
        return std::string(); // TODO: encode to specific encoding
#endif
        ensure_byte_string_match(encoding);
        return m_byte_string.get_value_ref();
    }

    int text::impl::get_length() const
    {
        return m_unicode_string->length();
    }

    int text::impl::get_symbol_at(int index) const
    {
        ensure_unicode_string_exists();
        if (index >= 0)
        {
            if (index >= get_length())
                throw 1; // TODO: exception "out of range"
            else
                return m_unicode_string->char32At(index);
        }
        else
        {
            if (index < get_length())
                throw 1; // TODO: exception "out of range"
            else
                return m_unicode_string->char32At(get_length() - index);
        }
    }

    text::text()
    {
    }

    text::~text()
    {
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
        return m_impl->get_wide_string();
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

    int text::get_symbol_at(int index) const
    {
        return m_impl->get_symbol_at(index);
    }

    int text::operator [] (int index) const
    {
        return m_impl->get_symbol_at(index);
    }
}
