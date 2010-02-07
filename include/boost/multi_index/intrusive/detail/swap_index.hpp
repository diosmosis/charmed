///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SWAP_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SWAP_INDEX_HPP

#include <boost/config.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename IndexTuple>
    struct swap_index
    {
        swap_index(IndexTuple & x_, IndexTuple & y_)
            : x(x_)
            , y(y_)
        {}

        template <typename N>
        void operator()(N) const
        {
            fusion::at<N>(x).swap(fusion::at<N>(y));
        }

        IndexTuple & x;
        IndexTuple & y;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SWAP_INDEX_HPP )