///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP

#include <boost/multi_index/tag.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    // specifier forward decls
    // Arg1 -> !TagList
    // Arg2 -> KeyFromValue
    // Arg3 -> Compare
    template <typename Arg1, typename Arg2 = mpl::na, typename Arg3 = mpl::na>
    struct ordered_unique;

    template <typename Arg1, typename Arg2 = mpl::na, typename Arg3 = mpl::na>
    struct ordered_non_unique;

    // Arg1 -> !TagList
    // Arg2 -> KeyFromValue
    // Arg3 -> Hash
    // Arg4 -> Pred
    template <typename Arg1 = mpl::na, typename Arg2 = mpl::na, typename Arg3 = mpl::na, typename Arg4 = mpl::na,
              typename Arg5 = mpl::na>
    struct unordered_unique;

    template <typename Arg1 = mpl::na, typename Arg2 = mpl::na, typename Arg3 = mpl::na, typename Arg4 = mpl::na,
              typename Arg5 = mpl::na>
    struct unordered_non_unique;

    template <typename TagList = tag<> >
    struct sequenced;
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP )
