///////////////////////////////////////////////////////////////////////////////
//
/// \file hashed_index_args.hpp
/// Contains the <c>hashed_index_args\<\></c> utility type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HASHED_INDEX_ARGS_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HASHED_INDEX_ARGS_HPP

#include <boost/multi_index/detail/hash_index_args.hpp>
#include <boost/multi_index/intrusive/bucket_count.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief A utility that determines the compile-time attributes of a hashed index
    ///        based on the supplied compile-time arguments.
    ///
    /// <c>hashed_index_args</c> uses Boost.Multi-Index's <c>hashed_index_args</c>
    /// to determine the KeyFromValue functor, the hashing functor and the
    /// comparison predicate and determines the bucket count itself. If nothing
    /// is supplied, a default bucket count of 256 is used.
    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct hashed_index_args
        : multi_index::detail::hashed_index_args<Arg1, Arg2, Arg3, boost::mpl::na>
    {
        typedef Arg4 supplied_bucket_count;
        typedef typename mpl::if_<
            mpl::is_na<supplied_bucket_count>,
            bucket_count<256>,
            supplied_bucket_count
        >::type bucket_count;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HASHED_INDEX_ARGS_HPP )
