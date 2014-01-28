/// @author Владимир Керимов

#pragma once

#include <data/object>
#include <data/lazy>

namespace data
{
    class DOZEN_API set : public object
    {
    public:
        set();

        virtual ~set() override;

        virtual std::unique_ptr<object> clone() const override;

    private:
        class impl;

        lazy<impl> m_impl;
    };
}
