/// @author Владимир Керимов

#pragma once

#include <data/object>
#include <data/lazy>

namespace data
{
    class DOZEN_API array : public object
    {
    public:
        array();
        
        virtual ~array() override;
        
        virtual std::unique_ptr<object> clone() const override;
    
    private:
        class impl;
        
        lazy<impl> m_impl;
    };
}
