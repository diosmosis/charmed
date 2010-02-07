///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_METADATA_ITERATOR_HPP )
#define CHARMED_METADATA_ITERATOR_HPP

#include <charmed/metadata_storage.hpp>

namespace charmed
{
    template <typename M>
    struct metadata_iterator
    {
        typedef typename metadata_storage<M>::container_type::const_iterator type;
    };
}

#endif // #if !defined( CHARMED_METADATA_ITERATOR_HPP )