///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_DETAIL_TYPE_INDEX_OF_HPP )
#define CHARMED_DETAIL_TYPE_INDEX_OF_HPP

#include <charmed/index_specifier_of.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>

namespace charmed { namespace detail
{
    template <typename M>
    struct type_index_of
        : boost::mpl::size<
            typename index_specifier_of<M>::type
        >
    {};
}}

#endif // #if !defined( CHARMED_DETAIL_TYPE_INDEX_OF_HPP )
