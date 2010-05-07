///////////////////////////////////////////////////////////////////////////////
//
/// \file key_from_value_composite.hpp
/// Contains the <c>key_from_value_composite\<\></c> function object.
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
    /// \brief A composite function object that executes one function using the key(s)
    ///        of value(s) extracted using another function object.
    ///
    /// <c>key_from_value_composite\<\></c> acts as intermediary between the intrusive
    /// <c>multi_index_container\<\></c>, which is supplied function objects that act
    /// on keys, and Boost.Intrusive containers which expect function objects that act
    /// on values. <c>key_from_value_composite\<\></c> executes the function object
    /// supplied by Multi-Index users (such as a hasher or comparison predicate) using
    /// the key of the value Boost.Intrusive passes it.
    ///
    /// \remark <c>key_from_value_composite\<\></c> acts as both a unary and binary
    ///         function object. So if two arguments are passed to it, <c>Function</c>
    ///         will be invoked as a binary function object.
    /// \remark <c>key_from_value_composite\<\></c> will detect when the value's key
    ///         type is passed, and, in such a case, will not execute <c>KeyFromValue</c>.
    ///
    /// \tparam KeyFromValue a function object that extracts the key type of a value.
    /// \tparam Function a function that operates on one or two keys extracted using
    ///                  <c>KeyFromValue</c>.
    template <typename KeyFromValue, typename Function>
    struct key_from_value_composite
    {
        typedef typename KeyFromValue::result_type key_type;
        typedef typename Function::result_type result_type;

        explicit key_from_value_composite(Function const& f = Function())
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
            return function(key_from_value(x));
        }

        result_type operator()(key_type const& x) const
        {
            return function(x);
        }

        KeyFromValue key_from_value;
        Function function;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_KEY_FROM_VALUE_COMPOSITE_HPP )