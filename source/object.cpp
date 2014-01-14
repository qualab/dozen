#include <data/object.hpp>
#include <utility>

namespace data
{
    object::~object()
    {
    }

    std::unique_ptr<object> object::clone() const
    {
        return std::unique_ptr<object>(new object(*this));
    }
}
