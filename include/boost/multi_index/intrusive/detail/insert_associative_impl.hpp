///////////////////////////////////////////////////////////////////////////////
//
/// \file insert_associative_impl.hpp
/// Contains the <c>insert_associative_impl\<\></c> utility function.
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
#if defined( DOXYGEN_INVOKED )

    /// \brief Inserts <c>x</c> into any sort of associative container.
    ///
    /// <c>insert_associative_impl()</c> is used to insert into sets and multisets without knowing
    /// exactly which one the container in question is. Since these containers have differing return
    /// types for their respective insert methods, <c>insert_associative_impl()</c> must be used when
    /// the exact type is unknown.
    ///
    /// \param container the container to insert into. Can be any associative container.
    /// \param x the value to be inserted into <c>container</c>.
    /// \result If <c>container</c> only contains unique values (ie, <c>set\<\></c>/<c>unordered_set\<\></c>,
    ///         the result of this function will be, on successful insertion, a pair holding an iterator
    ///         to <c>x</c> and the value <c>true</c>. If insertion fails, the pair will hold an iterator
    ///         referencing the value that caused insertion to fail and the value <c>false</c>. If
    ///         <c>container</c> can contain identical values (ie, <c>multiset\<\></c>/<c>unordered_multiset\<\></c>,
    ///         insertion will always succeed, so the pair will hold an iterator to <c>x</c> and the value
    ///         <c>true</c>.
    template <typename Container>
    inline std::pair<typename Container::iterator, bool> insert_associative_impl(
        Container & container, typename Container::value_type & x)
    {
    }

#endif

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
