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
        can_insert(Value & nv, Value & ov, bool & r)
            : new_value(nv)
            , old_value(ov)
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
            // TODO: Instead of using find, for unordered indices, we can just use the key_eq() function object.
            if (result)
            {
                typename Index::const_iterator i = x.impl().find(new_value);

                result = &*i != &old_value && i != x.impl().end();
            }
        }

        Value & new_value;
        Value & old_value;
        bool & result;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_INSERT_HPP )