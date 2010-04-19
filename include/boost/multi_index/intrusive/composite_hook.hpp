///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP

#include <boost/multi_index/intrusive/detail/hook_type_of.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/fusion/include/as_vector.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    template <typename IndexSpecifierList>
    struct composite_hook
        : fusion::result_of::as_vector<
            typename mpl::transform<
                IndexSpecifierList,
                detail::hook_type_of<mpl::_1>
            >::type
        >::type
    {};
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP )