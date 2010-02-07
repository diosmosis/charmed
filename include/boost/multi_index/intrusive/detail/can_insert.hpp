///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_INSERT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_INSERT_HPP

#include <boost/multi_index/intrusive/detail/is_set_unique_container.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Value>
    struct can_insert
    {
        can_insert(Value & v, bool & r)
            : value(v)
            , result(r)
        {
            result = true;
        }

        template <typename Index>
        typename disable_if<
            is_set_unique_container<typename Index::impl_type>, void
        >::type operator()(Index & x) const
        {
            // empty
        }

        template <typename Index>
        typename enable_if<
            is_set_unique_container<typename Index::impl_type>, void
        >::type operator()(Index & x) const
        {
            if (result)
            {
                result = !x.impl().contains(value);
            }
        }

        Value & value;
        bool & result;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_INSERT_HPP )