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

#if defined(DOZEN_USES_ICU) && DOZEN_WCHAR_SIZE > 1
#define DOZEN_NEED_UNICODE_STRING
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

        std::wstring const& get_wide_string_ref() const;
        std::string const& get_byte_string_ref(char const* encoding) const;
        std::string const& get_encoding_string_ref() const;

#ifdef DOZEN_NEED_UNICODE_STRING
        icu::UnicodeString const& get_unicode_string_ref() const;
#endif

        int get_length() const;
        int get_symbol_at(int index) const;

    private:
        mutable nullable<std::string> m_byte_string;
        mutable nullable<std::string> m_encoding;

        void ensure_byte_string_match(char const* encoding) const;

#if DOZEN_WCHAR_SIZE > 1
        mutable nullable<std::wstring> m_wide_string;

        void ensure_wide_string_exists() const;
#endif

#ifdef DOZEN_NEED_UNICODE_STRING
        mutable nullable<icu::UnicodeString> m_unicode_string;

        void ensure_unicode_string_exists() const;
#endif
    };

    template <typename char_type>
    struct string_of;

    template<> struct string_of<wchar_t> { typedef std::wstring type; };
    template<> struct string_of<char>    { typedef std::string  type; };

    template <typename char_type>
    nullable<typename string_of<char_type>::type> ptr_to_nullable(char_type const* argument)
    {
        return argument ? nullable<typename string_of<char_type>::type>(argument)
                        : nullable<typename string_of<char_type>::type>(null);
    }

    template <typename char_type>
    char_type const* nullable_to_ptr(nullable<typename string_of<char_type>::type> const& argument)
    {
        return argument.is_null() ? null : argument->c_str();
    }

    text::impl::impl()
    {
    }

    text::impl::impl(wchar_t const* wide_string)
        : m_wide_string(ptr_to_nullable(wide_string))
    {
    }

    text::impl::impl(char const* byte_string)
        : m_byte_string(ptr_to_nullable(byte_string))
    {
    }

    text::impl::impl(char const* byte_string, char const* encoding)
        : m_byte_string(ptr_to_nullable(byte_string)), m_encoding(ptr_to_nullable(encoding))
    {
    }

#ifdef DOZEN_NEED_UNICODE_STRING
    void text::impl::ensure_unicode_string_exists() const
    {
        if (m_unicode_string.is_null())
        {
            if (m_wide_string.is_not_null())
            {
                if (m_wide_string->empty())
                    m_unicode_string = icu::UnicodeString();
                else
                {
#   if DOZEN_WCHAR_SIZE == 2
                    static_assert(sizeof(wchar_t) == 2, "Macro defined as sizeof(wchar_t) == 2, but this condition is false!");
                    m_unicode_string = icu::UnicodeString(m_wide_string->c_str(), m_wide_string->length());
#   elif DOZEN_WCHAR_SIZE == 4
                    static_assert(sizeof(wchar_t) == 4, "Macro defined as sizeof(wchar_t) == 4, but this condition is false!");
                    m_unicode_string = icu::UnicodeString::fromUTF32(m_wide_string->c_str(), m_wide_string->length());
#   else
#                   error Unexpected sizeof(wchar_t) for translate wide string to Unicode
#   endif
                }
            }
            else if (m_byte_string.is_null() || m_byte_string->empty())
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
#endif

    void text::impl::ensure_wide_string_exists() const
    {
#if DOZEN_USES_ICU
        ensure_unicode_string_exists();
#   if DOZEN_WCHAR_SIZE == 2
        static_assert(sizeof(wchar_t) == 2, "Macro defined as sizeof(wchar_t) == 2, but this condition is false!");
        m_wide_string = std::wstring(m_unicode_string->getBuffer(), m_unicode_string->length());
#   elif DOZEN_WCHAR_SIZE == 4
        static_assert(sizeof(wchar_t) == 4, "Macro defined as sizeof(wchar_t) == 4, but this condition is false!");
        m_wide_string = std::wstring(m_unicode_string->toUTF32(), m_unicode_string->length());
#   else
#       error Unexpected size of wchar_t; not one of follows: 1, 2, 4 bytes.
#   endif
#endif
    }

    void text::impl::ensure_byte_string_match(char const* encoding) const
    {
        if (m_byte_string.is_null() || m_encoding != ptr_to_nullable(encoding))
        {
            m_encoding = ptr_to_nullable(encoding);
            if (m_unicode_string.is_null() && m_wide_string.is_null())
            {
                m_byte_string = std::string();
            }
            else
            {
                ensure_unicode_string_exists();
                char const* encoding_ptr = nullable_to_ptr<char>(m_encoding);
                m_byte_string = std::string(4 * m_unicode_string->length(), '\0');
                int length = m_unicode_string->extract(0, m_unicode_string->length(), &m_byte_string->at(0), encoding_ptr);
                m_byte_string->resize(length);
            }
        }
    }

    std::wstring const& text::impl::get_wide_string_ref() const
    {
#if   DOZEN_WCHAR_SIZE == 1
        static_assert(sizeof(wchar_t) == 1, "Macro defined as sizeof(wchar_t) == 1, but this condition is false!");
        return get_byte_string_ref();
#endif
        if (m_wide_string.is_null())
        {
            ensure_unicode_string_exists();
#if DOZEN_WCHAR_SIZE == 2
            static_assert(sizeof(wchar_t) == 2, "Macro defined as sizeof(wchar_t) == 2, but this condition is false!");
            m_wide_string = std::wstring(m_unicode_string->getBuffer(), m_unicode_string->length());
#elif DOZEN_WCHAR_SIZE == 4
            static_assert(sizeof(wchar_t) == 4, "Macro defined as sizeof(wchar_t) == 4, but this condition is false!");
            m_wide_string = std::wstring(m_unicode_string->toUTF32());
#else
#       error Unexpected size of wchar_t; not one of follows: 1, 2, 4 bytes.
#endif
        }
        return m_wide_string.get_value_ref();
    }

    std::string const& text::impl::get_byte_string_ref(char const* encoding) const
    {
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
                DOZEN_THROW(out_of_range, "Index of symbol in text is positive and out of range [0..length).");
            else
                return m_unicode_string->char32At(index);
        }
        else
        {
            if (index < get_length())
                DOZEN_THROW(out_of_range, "Index of symbol in text is negatitive and out of range [-length..0).");
            else
                return m_unicode_string->char32At(get_length() - index);
        }
    }

//  text -------------------------------------------------------------------------

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

    int text::get_symbol_at(int index) const
    {
        return m_impl->get_symbol_at(index);
    }

    int text::operator [] (int index) const
    {
        return m_impl->get_symbol_at(index);
    }
}
