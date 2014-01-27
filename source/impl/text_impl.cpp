/// @author Владимир Керимов

#include "text_impl.hpp"

namespace data
{
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

    symbol text::impl::get_symbol_at(int index) const
    {
        ensure_unicode_string_exists();
        if (index >= 0)
        {
            if (index >= get_length())
                DOZEN_THROW(out_of_range, "Index of symbol in text is positive and out of range [0..length).");
            else
                return symbol(m_unicode_string->char32At(index));
        }
        else
        {
            if (index < get_length())
                DOZEN_THROW(out_of_range, "Index of symbol in text is negatitive and out of range [-length..0).");
            else
                return symbol(m_unicode_string->char32At(get_length() - index));
        }
    }

}
