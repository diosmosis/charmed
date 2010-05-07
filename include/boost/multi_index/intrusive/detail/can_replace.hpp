///////////////////////////////////////////////////////////////////////////////
//
/// \file can_replace.hpp
/// Contains the <c>can_insert\<\></c> function object.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_REPLACE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_REPLACE_HPP

#include <boost/multi_index/intrusive/detail/is_set_unique_container.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    /// \brief Checks if a certain value can be replaced by another value in an associative
    ///        index of a <c>multi_index_container\<\></c>.
    ///
    /// <c>can_replace\<\></c> is used by the <c>replace\<\></c> free function in \ref core_operations.hpp
    /// to make sure certain value can be inserted into every index assuming another value is missing.
    template <typename Value>
    struct can_replace
    {
        can_replace(Value & nv, Value & ov, bool & r)
            : new_value(nv)
            , old_value(ov)
            , result(r)
        {
            result = true;
        }

        template <typename Index>
        typename disable_if<
            is_set_unique_container<Index>, void
        >::type operator()(Index & x) const
        {
            // empty
        }

        template <typename Index>
        typename enable_if<
            is_set_unique_container<Index>, void
        >::type operator()(Index & x) const
        {
            if (result)
            {
                typename Index::const_iterator i = x.find(new_value);

                result = &*i != &old_value && i != x.end();
            }
        }

        Value & new_value;
        Value & old_value;
        bool & result;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CAN_REPLACE_HPP )