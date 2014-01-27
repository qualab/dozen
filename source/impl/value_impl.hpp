/// @author Владимир Керимов

#include <data/value.hpp>

namespace data
{
    class value::impl
    {
    public:
        impl();
        impl(impl const& another);

        void set_value(null_type);

        bool is_null() const;

        template <typename value_type>
        void set_value(value_type another)
        {
            m_kind = typename value::kind_of<value_type>::value_kind;
            m_holder.reset(new nullable<value_type>(another));
        }

        template <typename value_type>
        static value_type extract_value(nullable<value_type> const& holder)
        {
            return holder.get_value();
        }

        template <typename value_type>
        value_type get_value() const
        {
            if (!m_holder)
                throw 1; // TODO: exception throw
            return extract_value(dynamic_cast<nullable<value_type> const&>(*m_holder));
        }

    private:
        kind m_kind;
        std::unique_ptr<object> m_holder;
    };
}
