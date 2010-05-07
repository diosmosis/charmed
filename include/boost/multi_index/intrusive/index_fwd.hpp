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
    namespace detail
    {
        template <typename Value, typename Hook, typename Hash, typename Equal, typename BucketCount>
        struct bucketed_unordered_set;

        template <typename Value, typename Hook, typename Hash, typename Equal, typename BucketCount>
        struct bucketed_unordered_multiset;
    }

    template <typename Value, typename IndexSpecifierList, typename HookSpecifier>
    struct multi_index_container;

    // TODO: make index customization work like Boost.Intrusive's container options mechanisms.
    // specifier forward decls
    // Arg1 -> KeyFromValue
    // Arg2 -> Compare
    template <typename Arg1, typename Arg2 = mpl::na>
    struct ordered_unique;

    template <typename Arg1, typename Arg2 = mpl::na>
    struct ordered_non_unique;

    // Arg1 -> KeyFromValue
    // Arg2 -> Hash
    // Arg3 -> Pred
    // Arg4 -> BucketCount
    template <typename Arg1 = mpl::na, typename Arg2 = mpl::na, typename Arg3 = mpl::na, typename Arg4 = mpl::na>
    struct unordered_unique;

    template <typename Arg1 = mpl::na, typename Arg2 = mpl::na, typename Arg3 = mpl::na, typename Arg4 = mpl::na>
    struct unordered_non_unique;

    struct sequenced;
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP )
