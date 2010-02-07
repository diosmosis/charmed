///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HOOK_TYPE_OF_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HOOK_TYPE_OF_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename IndexSpecifier>
    struct hook_type_of
    {
        typedef typename IndexSpecifier::hook_type type;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_HOOK_TYPE_OF_HPP )