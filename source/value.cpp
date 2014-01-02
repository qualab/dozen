/// @author Владимир Керимов

#include <data/value.hpp>
#include <data/scalar.hpp>

namespace data
{
    class value::impl
    {
    public:
        class holder_base;

    private:
        holder_base* m_holder;
    };

    class holder_base
    {
    public:
    private:
        int m_flags;
    };

    template<typename T>
    class holder : public holder_base
    {
    public:
        typedef T value_type;

        static const size_t VALUE_SIZE = sizeof(value_type);

    private:
        value_type* m_value;
        byte m_buffer[VALUE_SIZE];
    };

    value::value()
    {
    }
}
