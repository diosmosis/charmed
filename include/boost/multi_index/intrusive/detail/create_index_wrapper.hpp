///////////////////////////////////////////////////////////////////////////////
//
/// \file create_index_wrapper.hpp
/// Contains the <c>create_index_wrapper\<\></c> function object type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CREATE_INDEX_WRAPPER_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CREATE_INDEX_WRAPPER_HPP

#include <boost/mpl/at.hpp>
#include <boost/fusion/include/at.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief Used by <c>multi_index_container</c> to wrap intrusive containers on the fly.
    ///
    /// 
    template <typename MultiIndexTypes>
    struct create_index_wrapper
    {
        typedef typename MultiIndexTypes::index_specifier_list index_specifier_list;

        template <typename N>
        struct result;

        template <typename N>
        struct result<create_index_wrapper(N)>
        {
            typedef typename mpl::at<index_specifier_list, N>::type specifier;
            typedef typename specifier::template index_class<MultiIndexTypes, N::value>::type type;
        };

        create_index_wrapper(typename MultiIndexTypes::self_type & mi_)
            : mi(mi_)
        {}

        template <typename N>
        typename result<create_index_wrapper(N)>::type operator()(N) const
        {
            typedef typename result<create_index_wrapper(N)>::type index_type;
            return index_type(mi, fusion::at<N>(mi.index_storage));
        }

        typename MultiIndexTypes::self_type & mi;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CREATE_INDEX_WRAPPER_HPP )