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

        class symbol_ref;

        symbol_ref operator [] (int index);

        text& operator += (text another);
        text& operator *= (int count);

        friend text operator + (text first, text second);
        friend text operator * (text source, int count);
        friend text operator * (int count, text source);

        text get_substring(int start, int end) const;

        class iterator;
        class const_iterator;
        class reverse_iterator;
        class const_reverse_iterator;

        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        reverse_iterator rbegin();
        reverse_iterator rend();

        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;

        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

        iterator find(text substring);
        const_iterator find(text substring) const;

    private:
        /// Forward declaration of implementation class in text.cpp file
        class impl;

        /// Lazy initialization and copy-on-write model of the implementation object
        lazy<impl> m_impl;
    };
}
