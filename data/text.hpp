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

        /// Clone text data to a new text object
        virtual std::unique_ptr<object> clone() const override;

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

        symbol_ref get_symbol_at(int index);
        symbol_ref get_symbol_ref_at(int index);

        symbol_ref operator [] (int index);

        text& operator += (text another);
        text& operator *= (int count);

        friend text operator + (text first, text second);
        friend text operator * (text source, int count);
        friend text operator * (int count, text source);

        text get_substring(int start, int end) const;
        int find_substring(text substring) const;

        template <class function>
        void for_each_symbol(function const& functor);

        template <class function>
        void for_each_symbol(function const& functor) const;

        class iterator;

        iterator begin();
        iterator end();

        iterator begin() const;
        iterator end() const;

    private:
        /// Forward declaration of implementation class in text.cpp file
        class impl;

        /// Lazy initialization and copy-on-write model of the implementation object
        lazy<impl> m_impl;
    };

//  ------------------------------------------------------------------------------

    /// Reference to a symbol in text
    class DOZEN_API text::symbol_ref
    {
    public:
        /// Initialize symbol reference by reference to a text and index of this symbol
        symbol_ref(text& owner, int index);

        /// Get code of this symbol in Unicode
        int get_code() const;

        /// Get this symbol as wide symbol (wchar_t)
        wchar_t get_as_wide_char() const;

        /// Get this symbol as ANSI symbol (char) using specified encoding
        char get_as_byte_char(std::string const& encoding) const;

        /// Get this symbol as ANSI symbol (char) using specified encoding
        char get_as_byte_char(char const* encoding) const;

        /// Get this symbol as ANSI symbol (char) using defult encoding
        char get_as_byte_char() const;

        /// Get this symbol as the text object which contains single symbol
        text get_as_text() const;
        
        /// Represent symbol as code in Unicode
        operator int() const;

        /// Represent symbol as wide char
        operator wchar_t() const;

        /// Represent symbol as ANSI char using default encoding
        operator char() const;

        /// Cast symbol to a bool same as integer code of Unicode casts
        operator bool() const;

        /// Represent symbol as text object
        operator text() const;

        /// Set symbol as code of Unicode
        void set_code(int code);

        /// Set symbol as wide character (wchar_t)
        void set_as_wide_char(wchar_t code);

        /// Set symbol as ANSI character using specified encoding
        void set_as_byte_char(char code, std::string const& encoding);

        /// Set symbol as ANSI character using specified encoding
        void set_as_byte_char(char code, char const* encoding);

        /// Set symbol as ANSI character using default encoding
        void set_as_byte_char(char code);

        /// Set symbol by single symbol of text object
        void set_as_text(text const& single);

        /// Initialize symbol by code of Unicode
        symbol_ref operator = (int code);

        /// Initialize symbol by wide character (wchar_t)
        symbol_ref operator = (wchar_t code);

        /// Initialize symbol by ANSI charactoer (char)
        symbol_ref operator = (char code);

        /// Initialize symbol as single symbol of text object
        symbol_ref operator = (text single);

    private:
        class impl;

        lazy<impl> m_impl;
    };

//  ------------------------------------------------------------------------------

    template <class function>
    void text::for_each_symbol(function const& functor)
    {
        for (int i = 0; i < get_length(); ++i)
        {
            functor(get_symbol_at(i));
        }
    }

    template <class function>
    void text::for_each_symbol(function const& functor) const
    {
        for (int i = 0; i < get_length(); ++i)
        {
            functor(get_symbol_at(i));
        }
    }
}
