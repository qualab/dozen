/// @author Владимир Керимов

#pragma once

#include <data/object.hpp>
#include <data/lazy.hpp>
#include <string>

namespace data
{
    /// Continer for text in Unicode or in ANSI string with lazy implementation and copy-on-write
    class DOZEN_API text : public object
    {
    public:
        /// Text object lazy initialization by empty text string, actually none of implementation objects will created before be referenced
        text();

        /// 
        virtual ~text() override;

        text(char const* another);

        text(wchar_t const* another);

        text(std::string const& another);

        text(std::wstring const& another);

        std::string get_ansi_string() const;

        std::wstring get_wide_string() const;

        char const* get_ansi_string_ptr() const;

        wchar_t const* get_wide_string_ptr() const;

        int operator [] (int index) const;

        int get_symbol_at(int index) const;

        int get_length() const;

    private:
        class impl;

        lazy<impl> m_impl;
    };
}
