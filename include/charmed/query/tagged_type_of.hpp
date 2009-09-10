///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_TAGGED_TYPE_OF_HPP )
#define CHARMED_QUERY_TAGGED_TYPE_OF_HPP

#include <charmed/metadata.hpp>
#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_iterator.hpp>

namespace charmed { namespace query
{
    template <typename T, typename M, typename K>
    inline T * tagged_type_of(K const& key)
    {
        typename metadata_iterator<M>::type i = metadata_storage<M>::metadata().find(key);

        if (i == metadata_storage<M>::metadata().end())
        {
            return 0;
        }

        // TODO need runtime type checking (in an assert)
        return static_cast<T *>(i->type_data_hook);
    }

    template <typename T, typename M>
    inline T * tagged_type_of(M const& md)
    {
        // TODO need runtime type checking (in an assert)
        return static_cast<T *>(static_cast<metadata<M> const&>(md).type_data_hook);
    }
}}

#endif // #if !defined( CHARMED_QUERY_TAGGED_TYPE_OF_HPP )