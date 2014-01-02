/// @author Владимир Керимов

#pragma once

#include <memory>

namespace data
{
    /// Lazy copy (on write) and lazy creation (on demand) of data of type uses as template attribute.
    template <class impl>
    class lazy
    {
    public:
        /// Default constructor does not create T, it is designed for nothing to do
        lazy();

        /// Constructor where created_impl is the pointer to the just created implementation
        lazy(impl* created_impl);

        /// Copy-on-write call of the method of T. After such call there will be only one reference to the data.
        impl* operator -> ();

        /// Proxy-call of the const-method of T. If data was not constructed, it will be created before the method calls.
        const impl* operator -> () const;

        /// Copy-on-write getting reference to the data of T. After such call there will be only one reference to the data.
        impl& operator * ();

        /// Get const-reference to the data of T. If data was not constructed, it will be created before the reference returns.
        const impl& operator * () const;

        /// Check is the reference to the data of T is unique.
        bool is_unique() const;

        /// Get reference count to the data of T.
        int ref_count() const;

    private:
        /// To create really lazy data, it should be able to initialize even in const-methods.
        mutable std::shared_ptr<impl> m_shared_impl;

        /// Create data of T if it's still does not exist.
        bool ensure_created();

        /// Makes copy of data of T which it was referenced to and gets the unique reference to the new copy.
        bool ensure_unique();
    };

// ------------------------------------------------------------------------- //

    template <class impl>
    lazy<impl>::lazy()
        : m_shared_impl()
    {
    }

    template <class impl>
    lazy<impl>::lazy(impl* created_impl)
        : m_shared_impl(created_impl)
    {
    }

    template <class impl>
    impl* lazy<impl>::operator -> ()
    {
        ensure_unique();
        return m_shared_impl.get();
    }

    template <class impl>
    const impl* lazy<impl>::operator -> () const
    {
        ensure_created();
        return m_shared_impl.get();
    }

    template <class impl>
    impl& lazy<impl>::operator * ()
    {
        ensure_unique();
        return *m_shared_impl;
    }

    template <class impl>
    const impl& lazy<impl>::operator * () const
    {
        ensure_created();
        return *m_shared_impl;
    }

    template <class impl>
    bool lazy<impl>::is_unique() const
    {
        return m_shared_impl.unique();
    }

    template <class impl>
    int lazy<impl>::ref_count() const
    {
        return m_shared_impl.use_count();
    }

    template <class impl>
    bool lazy<impl>::ensure_created()
    {
        if(!m_shared_impl)
        {
            m_shared_impl.reset(new impl());
            return true;
        }
        else
            return false;
    }

    template <class impl>
    bool lazy<impl>::ensure_unique()
    {
        if(!ensure_created() && !is_unique())
        {
            m_shared_impl.reset(new impl(*m_shared_impl));
            return true;
        }
        else
            return false;
    }
}
