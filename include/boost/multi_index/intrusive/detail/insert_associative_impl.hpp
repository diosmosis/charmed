///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOCIATIVE_IMPL_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOCIATIVE_IMPL_HPP

#include <boost/multi_index/intrusive/detail/is_set_unique_container.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Container>
    inline typename boost::enable_if<
        is_set_unique_container<Container>,
        std::pair<typename Container::iterator, bool>
    >::type insert_associative_impl(Container & container, typename Container::value_type & x)
    {
        return container.insert(x);
    }

    template <typename Container>
    inline typename boost::disable_if<
        is_set_unique_container<Container>,
        std::pair<typename Container::iterator, bool>
    >::type insert_associative_impl(Container & container, typename Container::value_type & x)
    {
        return std::pair<typename Container::iterator, bool>(container.insert(x), true);
    }
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOCIATIVE_IMPL_HPP )
