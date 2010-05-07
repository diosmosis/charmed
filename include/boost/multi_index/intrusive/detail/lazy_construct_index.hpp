///////////////////////////////////////////////////////////////////////////////
//
/// \file lazy_construct_index.hpp
/// Contains the <c>lazy_construct_index\<\></c> function object type.
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
    // TODO: Add a binary for_each and submit it to Boost.Fusion.
    /// \brief Function object used to construct <c>lazy_construct_from_tuple\<\></c> instances
    ///        held in a fusion sequence.
    ///
    /// <c>lazy_construct_index\<\></c> accepts <c>mpl::int_\<\></c> instances which it uses
    /// to access two fusion sequences that are supplied during construction. The first of
    /// these sequences holds the indices of a <c>multi_index_contaier\<\></c> and the second
    /// holds the constructor arguments for each index. The <c>construct_</c> method of 
    /// each <c>lazy_construct_from_tuple\<\></c> in the first instance is called using 
    /// its corresponding constructor args in the second sequence.
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