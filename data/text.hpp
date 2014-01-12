/// @author Владимир Керимов

#pragma once

#include <data/object.hpp>
#include <data/lazy.hpp>

namespace data
{
    class text : public object
    {
    public:
        text();

        text(char const* value);
        text(wchar_t const* value);

    private:
        class impl;
        lazy<impl> m_impl;
    };
}
