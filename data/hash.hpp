/// @author Владимир Керимов

#pragma once

#include <data/object>
#include <data/lazy>

namespace data
{
    class DOZEN_API hash : public object
    {
    public:
        hash();

        virtual ~hash() override;

        virtual std::unique_ptr<object> clone() const override;

    private:
        class impl;

        lazy<impl> m_impl;
    };
}
