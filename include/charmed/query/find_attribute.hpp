///////////////////////////////////////////////////////////////////////////////
//
/// \file find_attribute.hpp
/// Contains the <c>find_attribute\<M, N\>()</c> attribute querying function.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_FIND_ATTRIBUTE_HPP )
#define CHARMED_QUERY_FIND_ATTRIBUTE_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_iterator.hpp>
#include <charmed/detail/index_key_type.hpp>

namespace charmed { namespace query
{
    /// \brief Finds the attribute associated with <c>key</c> in the <c>N</c>th index of the
    ///        attribute's storage container.
    ///
    /// \tparam M the attribute type.
    /// \tparam N the index of the <c>multi_index_container\<\></c> index to search.
    /// \param key the key the attribute is associated with.
    /// \return the attribute associated with <c>key</c> or <c>0</c> if none exists.
    template <typename M, int N>
    inline M const* find_attribute(typename detail::index_key_type<M, N>::type const& key)
    {
        typename metadata_iterator<M, N>::type
           i = metadata_storage<M>::metadata().get<N>().find(key),
           end = metadata_storage<M>::metadata().get<N>().end();

        if (i == end)
        {
            return 0;
        }

        return &(*i);
    }
}}

#endif // #if !defined( CHARMED_QUERY_FIND_ATTRIBUTE_HPP )