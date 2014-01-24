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
}
