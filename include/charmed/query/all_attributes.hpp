///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_GET_ALL_ATTRIBUTES_HPP )
#define CHARMED_QUERY_GET_ALL_ATTRIBUTES_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_range.hpp>

namespace charmed { namespace query
{
    namespace result_of
    {
        template <typename M>
        struct all_attributes
        {
            typedef typename metadata_range<M>::type type;
        };
    }

    // TODO: there should be a way to specify the index to use when getting all attributes
    template <typename M>
    inline typename result_of::all_attributes<M>::type all_attributes()
    {
        typedef typename result_of::all_attributes<M>::type result_type;
        return result_type(metadata_storage<M>::metadata());
    }
}}

#endif // #if !defined( CHARMED_QUERY_GET_ALL_ATTRIBUTES_HPP )