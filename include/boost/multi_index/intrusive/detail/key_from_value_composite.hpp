///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_KEY_FROM_VALUE_COMPOSITE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_KEY_FROM_VALUE_COMPOSITE_HPP

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename KeyFromValue, typename Function>
    struct key_from_value_composite
    {
        typedef typename KeyFromValue::result_type key_type;
        typedef typename Function::result_type result_type;

        explicit key_from_value_composite(Function f = Function())
            : function(f)
        {}

        template <typename LHS, typename RHS>
        result_type operator()(LHS & lhs, RHS & rhs) const
        {
            return function(key_from_value(lhs), key_from_value(rhs));
        }

        template <typename LHS>
        result_type operator()(LHS & lhs, key_type const& rhs) const
        {
            return function(key_from_value(lhs), rhs);
        }

        template <typename RHS>
        result_type operator()(key_type const& lhs, RHS & rhs) const
        {
            return function(lhs, key_from_value(rhs));
        }

        template <typename Value>
        result_type operator()(Value & x) const
        {
            return function(key_from_value_composite(x));
        }

        KeyFromValue key_from_value;
        Function function;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_KEY_FROM_VALUE_COMPOSITE_HPP )