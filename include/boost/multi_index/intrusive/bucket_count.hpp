///////////////////////////////////////////////////////////////////////////////
//
/// \file bucket_count.hpp
/// Contains the <c>bucket_count\<N\></c> Intrusive Multi-Index option.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_BUCKET_COUNT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_BUCKET_COUNT_HPP

#include <boost/mpl/int.hpp>

namespace boost { namespace multi_index
{
    // TODO: multi_index_container must allow N == 0, and must allow the use of a user-specified bucket array.
    /// \brief An option used with the intrusive <c>multi_index_container\<\></c>'s unordered indices
    ///        that specifies how many buckets should initially be stored.
    ///
    /// \remarks The buckets created as a result of the use of this option, will be allocated on the stack.
    ///
    /// \tparam N the number of buckets to store.
    template <unsigned int N>
    struct bucket_count
        : mpl::int_<N>
    {};
}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_BUCKET_COUNT_HPP )
