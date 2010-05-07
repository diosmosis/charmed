///////////////////////////////////////////////////////////////////////////////
//
/// \file swap_index.hpp
/// Contains the <c>swap_index\<\></c> function object.
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
    /// \brief Swaps an index in a fusion sequence with a corresponding index in another
    ///        fusion sequence.
    ///
    /// <c>swap_index\<\></c> is used by <c>multi_index_container\<\></c>'s swap method
    /// to swap every index in its index tuple with those of another
    /// <c>multi_index_container\<\></c>. <c>multi_index_container\<\></c> executes a
    /// <c>fusion::for_each()</c> over an <c>mpl::range_c\<\></c>, so <c>swap_index\<\></c>
    /// accepts <c>mpl::int_\<\></c> instances, which it uses to access elements of two
    /// fusion sequences supplied during construction.
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