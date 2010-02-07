///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_IS_SET_UNIQUE_CONTAINER_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_IS_SET_UNIQUE_CONTAINER_HPP

#include <boost/mpl/bool.hpp>

#include <boost/intrusive/intrusive_fwd.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename S>
    struct is_set_unique_container
        : mpl::false_
    {};

#if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
    template <typename T, typename O1, typename O2, typename O3, typename O4>
    struct is_set_unique_container<boost::intrusive::set<T, O1, O2, O3, O4> >
        : mpl::true_
    {};

    template <typename T, typename O1, typename O2, typename O3, typename O4>
    struct is_set_unique_container<boost::intrusive::unordered_set<T, O1, O2, O3, O4> >
        : mpl::true_
    {};
#else
    template <typename T, typename ...Options>
    struct is_set_unique_container<boost::intrusive::set<T, Options> >
        : mpl::true_
    {};

    template <typename T, typename ...Options>>
    struct is_set_unique_container<boost::intrusive::unordered_set<T, Options> >
        : mpl::true_
    {};
#endif
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_IS_SET_UNIQUE_CONTAINER_HPP )