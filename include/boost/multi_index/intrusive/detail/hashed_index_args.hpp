///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
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
    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    struct hashed_index_args
        : multi_index::detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>
    {
        typedef typename hashed_index_args::full_form full_form;

        typedef typename mpl::if_<full_form, Arg4, Arg5>::type supplied_bucket_count;
        typedef typename mpl::if_<
            mpl::is_na<supplied_bucket_count>,
            bucket_count<256>,
            supplied_bucket_count
        >::type bucket_count;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HASHED_INDEX_ARGS_HPP )
