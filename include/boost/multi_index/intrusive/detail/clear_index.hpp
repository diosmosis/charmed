///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_CLEAR_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_CLEAR_INDEX_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    struct clear_index
    {
        template <typename Index>
        void operator()(Index & idx) const
        {
            idx.impl().clear();
        }
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_CLEAR_INDEX_HPP )
