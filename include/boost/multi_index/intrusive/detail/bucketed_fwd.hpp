///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKETED_FWD_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKETED_FWD_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Value, typename Hook, typename Hash, typename Equal, typename BucketCount>
    struct bucketed_unordered_set;

    template <typename Value, typename Hook, typename Hash, typename Equal, typename BucketCount>
    struct bucketed_unordered_multiset;
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKETED_FWD_HPP )
