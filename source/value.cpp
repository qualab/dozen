/// @author Владимир Керимов

#include <data/value.hpp>
#include <data/null.hpp>

namespace data
{
    class holder_base;

    class value::impl
    {
    public:

    private:
        holder_base* m_holder;
    };

    class holder_base
    {
    };

    template<typename value_type>
    class holder : public holder_base
    {
    public:
        nullable<value_type> m_value;
    };
}
