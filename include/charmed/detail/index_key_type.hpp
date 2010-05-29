///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_DETAIL_INDEX_KEY_TYPE_HPP )
#define CHARMED_DETAIL_INDEX_KEY_TYPE_HPP

#include <charmed/metadata_storage.hpp>

namespace charmed { namespace detail
{
    template <typename M, int N>
    struct index_key_type
    {
        typedef typename metadata_storage<M>::container_type::template nth_index<N>::type::key_type type;
    };
}}

#endif // #if !defined( CHARMED_DETAIL_INDEX_KEY_TYPE_HPP )
