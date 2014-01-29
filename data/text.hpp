/// @author Владимир Керимов

#pragma once

#include <data/object>
#include <data/lazy>
#include <string>

namespace data
{
    class symbol;
    class symbol_ref;

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
        symbol get_symbol_at(int index) const;

        /// Operator gets symbol specified by its index in the text
        symbol operator [] (int index) const;

        symbol_ref get_symbol_at(int index);

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

    private:
        /// Forward declaration of implementation class in text.cpp file
        class impl;

        /// Lazy initialization and copy-on-write model of the implementation object
        lazy<impl> m_impl;

        // friend symbol_ref to get access to text::impl
        friend class symbol_ref;
    };

//  symbol_base ----------------------------------------------------------------

    /// Base class for symbol and symbol_ref classes
    class DOZEN_API symbol_base : public object
    {
    public:
        /// Get code of this symbol in Unicode
        virtual int get_code() const = 0;

        /// Set symbol as code of Unicode
        virtual void set_code(int code) = 0;

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

        /// Set symbol as wide character (wchar_t)
        void set_as_wide_char(wchar_t code);

        /// Set symbol as ANSI character using specified encoding
        void set_as_byte_char(char code, std::string const& encoding);

        /// Set symbol as ANSI character using specified encoding
        void set_as_byte_char(char code, char const* encoding);

        /// Set symbol as ANSI character using default encoding
        void set_as_byte_char(char code);

        /// Set symbol by single symbol of text object
        void set_as_text(text single);

        /// Initialize symbol by code of Unicode
        symbol_base& operator = (int code);

        /// Initialize symbol by wide character (wchar_t)
        symbol_base& operator = (wchar_t code);

        /// Initialize symbol by ANSI charactoer (char)
        symbol_base& operator = (char code);

        /// Initialize symbol as single symbol of text object
        symbol_base& operator = (text single);
    };

//  symbol ---------------------------------------------------------------------

    /// Symbol of text
    class DOZEN_API symbol : public symbol_base
    {
    public:
        typedef symbol_base base;

        symbol();

        virtual ~symbol();

        virtual std::unique_ptr<object> clone() const override;

        virtual int get_code() const override;

        virtual void set_code(int code) override;

        explicit symbol(int code);

        explicit symbol(wchar_t code);

        symbol(char code, char const* encoding);

        symbol(char code, std::string const& encoding);

        explicit symbol(char code);

    private:
        class impl;

        lazy<impl> m_impl;
    };

//  symbol_ref -----------------------------------------------------------------

    /// Reference to a symbol in the text
    class DOZEN_API symbol_ref : public symbol_base
    {
    public:
        typedef symbol_base base;

        symbol_ref(text source, int index);

        virtual ~symbol_ref() override;

        virtual std::unique_ptr<object> clone() const override;

        virtual int get_code() const override;

        virtual void set_code(int code) override;

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
