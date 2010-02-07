///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKET_COUNT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKET_COUNT_HPP

#include <boost/mpl/int.hpp>

namespace boost { namespace multi_index
{
    template <int N>
    struct bucket_count
        : mpl::int_<N>
    {};
}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKET_COUNT_HPP )
