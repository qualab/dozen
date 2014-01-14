/// @author Владимир Керимов

#pragma once

#include <data/api.hpp>
#include <memory>

namespace data
{
    class DOZEN_API object
    {
    public:
        virtual ~object();

        virtual std::unique_ptr<object> clone() const;
    };
}
