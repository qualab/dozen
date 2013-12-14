/// @author Владимир Керимов

#pragma once

#include <memory>

namespace data
{
    /// Lazy copy (on write) and lazy creation (on demand) of data of type uses as template attribute.
    template <typename T>
    class lazy
    {
    public:
        /// Default constructor does not create T, it is designed for nothing to do
        lazy();

        /// Constructor where pointer to created T stored
        lazy(T* pointer);

        /// Copy-on-write call of the method of T. After such call there will be only one reference to the data.
        T* operator -> ();

        /// Proxy-call of the const-method of T. If data was not constructed, it will be created before the method calls.
        const T* operator -> () const;

        /// Copy-on-write getting reference to the data of T. After such call there will be only one reference to the data.
        T& operator * ();

        /// Get const-reference to the data of T. If data was not constructed, it will be created before the reference returns.
        const T& operator * () const;

        /// Check is the reference to the data of T is unique.
        bool is_unique() const;

        /// Get reference count to the data of T.
        int ref_count() const;

    private:
        /// To create really lazy data, it should be able to initialize even in const-methods.
        mutable std::shared_ptr<T>  m_pointer;

        /// Create data of T if it's still does not exist.
        bool ensure_created();

        /// Makes copy of data of T which it was referenced to and gets the unique reference to the new copy.
        bool ensure_unique();
    };

// ------------------------------------------------------------------------- //

    template <typename T>
    lazy<T>::lazy()
    {
    }

    template <typename T>
    lazy<T>::lazy(T* pointer)
        : m_pointer(pointer)
    {
    }

    template <typename T>
    T* lazy<T>::operator -> ()
    {
        ensure_unique();
        return m_pointer.get();
    }

    template <typename T>
    const T* lazy<T>::operator -> () const
    {
        ensure_created();
        return m_pointer.get();
    }

    template <typename T>
    T& lazy<T>::operator * ()
    {
        ensure_unique();
        return *m_pointer;
    }

    template <typename T>
    const T& lazy<T>::operator * () const
    {
        ensure_created();
        return *m_pointer;
    }

    template <typename T>
    bool lazy<T>::is_unique() const
    {
        return m_pointer.unique();
    }

    template <typename T>
    int lazy<T>::ref_count() const
    {
        return m_pointer.use_count();
    }

    template <typename T>
    bool lazy<T>::ensure_created()
    {
        if(!m_pointer)
        {
            m_pointer.reset(new T());
            return true;
        }
        else
            return false;
    }

    template <typename T>
    bool lazy<T>::ensure_unique()
    {
        if(!ensure_created() && !is_unique())
        {
            m_pointer.reset(new T(*m_pointer));
            return true;
        }
        else
            return false;
    }
}
