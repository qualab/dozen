/// @author Владимир Керимов

#pragma once

#include <data/api>
#include <memory>

namespace data
{
    class DOZEN_API object
    {
    public:
        virtual ~object();

        virtual std::unique_ptr<object> clone() const = 0;
    };
}
