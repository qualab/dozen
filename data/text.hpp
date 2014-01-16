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

        /// Trevial destructor inherited from object
        virtual ~text() override;

        /// Create text object by wide string
        text(wchar_t const* wide_string);

        /// Create text object by byte string using default encoding
        text(char const* byte_string);

        /// Create text object by byte string using specified encoding
        text(char const* byte_string, const char* encoding);

        /// Create text object by wide string
        text(std::wstring const& wide_string);

        /// Create text object by byte string using default encoding
        text(std::string const& byte_string);

        /// Create text object by byte string using specified encoding
        text(std::string const& byte_string, char const* encoding);

        /// Create text object by byte string using specified encoding
        text(std::string const& byte_string, std::string const& encoding);

        /// Get wide string
        std::wstring get_wide_string() const;

        /// Get byte string using default encoding
        std::string get_byte_string() const;

        /// Get byte string using specified encoding
        std::string get_byte_string(char const* encoding) const;

        /// Get byte string using specified encoding
        std::string get_byte_string(std::string const& encoding) const;

        /// Get pointer to byte string data in pure C style
        char const* get_byte_string_ptr() const;

        /// Get pointer to byte string data in pure C style
        char const* get_byte_string_ptr(char const* encoding) const;

        /// Get pointer to byte string data in pure C style
        char const* get_byte_string_ptr(std::string const& encoding) const;

        /// Get length of text in symbols
        int get_length() const;

        /// Get symbol specified by its index in the text
        int get_symbol_at(int index) const;

        /// Operator gets symbol specified by its index in the text
        int operator [] (int index) const;

    private:
        /// Forward declaration of implementation class in text.cpp file
        class impl;

        /// Lazy initialization and copy-on-write model of the implementation object
        lazy<impl> m_impl;
    };
}
