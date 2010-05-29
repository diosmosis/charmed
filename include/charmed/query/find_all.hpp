///////////////////////////////////////////////////////////////////////////////
//
/// \file find_all.hpp
/// Contains the <c>find_all()</c> attribute querying function.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_FIND_ALL_HPP )
#define CHARMED_QUERY_FIND_ALL_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_range.hpp>
#include <charmed/detail/index_key_type.hpp>

namespace charmed { namespace query
{
    /// \brief Retrieves every attribute of type <c>M</c> whose key is equal to <c>key</c>.
    ///
    /// \tparam M the attribute type.
    /// \tparam N the attribute index to query.
    /// \param key the key value to look for.
    /// \return every attribute of type <c>M</c> whose key is equal to <c>key</c>.
    template <typename M, int N>
    typename metadata_range<M, N>::type find_all(typename detail::index_key_type<M, N>::type const& key)
    {
        typedef typename metadata_range<M, N>::type result_type;

        return result_type(metadata_storage<M>::metadata().get<N>().equal_range(key));
    }
}}

#endif // #if !defined( CHARMED_QUERY_FIND_ALL_HPP )
