///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_INDEX_HPP

#include <boost/fusion/include/at.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename IndexTuple, typename CtorTuple>
    struct lazy_construct_index
    {
        lazy_construct_index(IndexTuple & i, CtorTuple & ct)
            : indices(i)
            , ctor_tuple(ct)
        {}

        template <typename N>
        void operator()(N) const
        {
            fusion::at<N>(indices).construct_(fusion::at<N>(ctor_tuple));
        }

        IndexTuple & indices;
        CtorTuple & ctor_tuple;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_INDEX_HPP )