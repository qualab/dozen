/// @author Владимир Керимов

#include <data/text.hpp>

namespace data
{
    class text::symbol_ref::impl
    {
    public:
        impl(text& owner, int index);

        text const& get_owner() const;
        text& get_owner();
        int get_index() const;

        void set_code(int code);
        int get_code() const;

        void set_as_wide_char(wchar_t code);
        wchar_t get_as_wide_char() const;

        void set_as_byte_char(char code, char const* encoding);
        char get_as_byte_char(char const* encoding) const;

        void set_as_text(text single);
        text get_as_text() const;

    private:
        text& m_owner;
        int m_index;
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

    int text::get_symbol_at(int index) const
    {
        return m_impl->get_symbol_at(index);
    }

    int text::operator [] (int index) const
    {
        return m_impl->get_symbol_at(index);
    }

/// text::symbol_ref::impl -----------------------------------------------------

    text::symbol_ref::impl::impl(text& owner, int index)
        : m_owner(owner), m_index(index)
    {
    }

    text const& text::symbol_ref::impl::get_owner() const
    {
        return m_owner;
    }

    text& text::symbol_ref::impl::get_owner()
    {
        return m_owner;
    }

    int text::symbol_ref::impl::get_index() const
    {
        return m_index;
    }

    void text::symbol_ref::impl::set_code(int code)
    {
    }

    int text::symbol_ref::impl::get_code() const
    {
        return m_owner.get_symbol_at(m_index);
    }

    void text::symbol_ref::impl::set_as_wide_char(wchar_t code)
    {
    }

    wchar_t text::symbol_ref::impl::get_as_wide_char() const
    {
        int code = m_owner.get_symbol_at(m_index);
        if (code > std::numeric_limits<wchar_t>::max())
            DOZEN_THROW(out_of_range, "Code of symbol is out of range of the wide character type");
        return static_cast<wchar_t>(code);
    }

    void text::symbol_ref::impl::set_as_byte_char(char code, char const* encoding)
    {
    }

    char text::symbol_ref::impl::get_as_byte_char(char const* encoding) const
    {
    }

    void text::symbol_ref::impl::set_as_text(text single)
    {
    }

    text text::symbol_ref::impl::get_as_text() const
    {
        return text(); // TODO: text from symbol
    }

/// text::symbol_ref -----------------------------------------------------------

    text::symbol_ref::symbol_ref(text& owner, int index)
        : m_impl(new impl(owner, index))
    {
    }
}
