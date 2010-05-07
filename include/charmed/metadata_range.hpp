///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata_range.hpp
/// Contains the <c>metadata_range\<M,N\></c> metafunction.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_METADATA_RANGE_TYPE_HPP )
#define CHARMED_METADATA_RANGE_TYPE_HPP

#include <charmed/metadata_iterator.hpp>
#include <boost/range.hpp>

namespace charmed
{
    /// \brief A metafunction that returns a range type that iterates over a
    ///        certain attribute.
    ///
    /// \tparam M the attribute type.
    /// \tparam N the attribute index to use. Defaults to <c>0</c>.
    template <typename M, int N = 0>
    struct metadata_range
    {
        typedef boost::iterator_range<typename metadata_iterator<M, N>::type> type;
    };
}

#endif // #if !defined( CHARMED_METADATA_RANGE_TYPE_HPP )