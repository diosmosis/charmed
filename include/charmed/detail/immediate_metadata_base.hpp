///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_DETAIL_IMMEDIATE_METADATA_BASE_HPP )
#define CHARMED_DETAIL_IMMEDIATE_METADATA_BASE_HPP

#include <charmed/charmed_fwd.hpp>

namespace charmed
{
    template <typename D>
    struct immediate_metadata_base : D
    {
        immediate_metadata_base(void const* x, D const& d)
            : D(d), type_data_hook(x)
        {}

        void const* type_data_hook;
    };
}

#endif // #if !defined( CHARMED_DETAIL_IMMEDIATE_METADATA_BASE_HPP )
