///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_METADATA_OF_HPP )
#define CHARMED_QUERY_METADATA_OF_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/metadata_storage.hpp>
#include <typeinfo>

namespace charmed { namespace query
{
    // TODO: It should be possible to associate more than one attribute to a type.
    template <typename M>
    inline M const* single_metadata_of(void const* type_data)
    {
        typedef typename metadata_storage<M>::container_type container_type;

        enum { last_index = container_type::index_count - 1 };

        typedef typename container_type::template nth_index<last_index>::type::const_iterator iterator;

        iterator i = metadata_storage<M>::metadata().template get<last_index>().find(type_data);
        return i == metadata_storage<M>::metadata().template get<last_index>().end() ? 0 : &*i;
    }

    template <typename M>
    inline M const* single_metadata_of(std::type_info const& type_data)
    {
        return single_metadata_of<M>(&type_data);
    }
}}

#endif // #if !defined( CHARMED_QUERY_METADATA_OF_HPP )
