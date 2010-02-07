///////////////////////////////////////////////////////////////////////////////
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

namespace charmed { namespace query
{
    template <typename M, typename K>
    inline M const* find_attribute(K const& key)
    {
        typename metadata_iterator<M>::type i = metadata_storage<M>::metadata().find(key);

        if (i == metadata_storage<M>::metadata().end())
        {
            return 0;
        }

        return &(*i);
    }
}}

#endif // #if !defined( CHARMED_QUERY_FIND_ATTRIBUTE_HPP )