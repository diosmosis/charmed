///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_COMMON_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_COMMON_INDEX_HPP

#include <boost/multi_index/intrusive/detail/core_operations.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    // indices forward decls
    template <typename MultiIndexTypes, int N>
    struct common_index
    {
        typedef mpl::int_<N>                                        index_n;

        typedef typename MultiIndexTypes::self_type                 multi_index_type;

        typedef typename MultiIndexTypes::local_index_tuple_type    impl_list;
        typedef typename MultiIndexTypes::index_specifier_list      specifier_list;

        typedef typename mpl::at_c<impl_list, N>::type::data_type   impl_type;
        typedef typename mpl::at_c<specifier_list, N>::type         specifier;

        typedef typename impl_type::value_type                      value_type;
        typedef typename impl_type::iterator                        iterator;
        typedef typename impl_type::const_iterator                  const_iterator;
        typedef typename impl_type::reference                       reference;
        typedef typename impl_type::const_reference                 const_reference;
        typedef typename impl_type::pointer                         pointer;
        typedef typename impl_type::const_pointer                   const_pointer;
        typedef typename impl_type::difference_type                 difference_type;
        typedef typename impl_type::size_type                       size_type;

        common_index(multi_index_type & x, impl_type & i)
            : container(x)
            , impl_(i)
        {}

        iterator begin()
        {
            return impl().begin();
        }

        iterator end()
        {
            return impl().end();
        }

        const_iterator begin() const
        {
            return impl().begin();
        }

        const_iterator end() const
        {
            return impl().end();
        }

        const_iterator cbegin() const
        {
            return impl().cbegin();
        }

        size_type size() const
        {
            return impl().size();
        }

        bool empty() const
        {
            return impl().empty();
        }

        const_iterator cend() const
        {
            return impl().cend();
        }

        void clear()
        {
            detail::clear(container);
        }

        void swap(common_index & x)
        {
            container.swap(x.container);
        }

        iterator erase(iterator position)
        {
            return detail::erase(container, *this, position);
        }

        iterator erase(iterator first, iterator last)
        {
            return detail::erase(container, *this, first, last);
        }

        bool replace(iterator position, value_type const& x)
        {
            return detail::replace(container, position, x);
        }

        template <typename Modifier>
        bool modify(iterator position, Modifier m)
        {
            return detail::modify(container, position, m);
        }

        impl_type & impl()
        {
            return impl_;
        }

        impl_type const& impl() const
        {
            return impl_;
        }

        iterator iterator_to(reference x)
        {
            return impl().iterator_to(x);
        }

        const_iterator iterator_to(const_reference x) const
        {
            return impl().iterator_to(x);
        }

        multi_index_type & container;
        impl_type & impl_;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_COMMON_INDEX_HPP )
