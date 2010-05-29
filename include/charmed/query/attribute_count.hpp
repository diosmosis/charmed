///////////////////////////////////////////////////////////////////////////////
//
/// \file attribute_count.hpp
/// Contains the <c>attribute_count()</c> attribute querying functions.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_ATTRIBUTE_COUNT_HPP )
#define CHARMED_QUERY_ATTRIBUTE_COUNT_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/detail/index_key_type.hpp>

namespace charmed { namespace query
{
    /// \brief Returns the number of attributes of type <c>M</c> that exist.
    ///
    /// \tparam M the attribute type.
    /// \return the number of attributes of type <c>M</c> that exist.
    template <typename M>
    std::size_t attribute_count()
    {
        return metadata_storage<M>::metadata().size();
    }

    /// \brief Returns the number of attributes of type <c>M</c> that have a key equal to <c>key</c>.
    ///
    /// \tparam M the attribute type.
    /// \tparam N the attribute index to search.
    /// \param key the key to look for.
    /// \return the number of attributes of type <c>M</c> that have a key equal to <c>key</c>.
    template <typename M, int N>
    std::size_t attribute_count(typename detail::index_key_type<M, N>::type const& key)
    {
        return metadata_storage<M>::metadata().get<N>().count(key);
    }
}}

#endif // #if !defined( CHARMED_QUERY_ATTRIBUTE_COUNT_HPP )
