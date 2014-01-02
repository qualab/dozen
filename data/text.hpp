/// @author Владимир Керимов

#pragma once

#include <data/lazy.hpp>

namespace data
{
    class text
    {
    public:
        text();

        text(const char* value);
        text(const wchar_t* value);

        text

    private:
        class impl;
        lazy<impl> m_impl;
    };
}
