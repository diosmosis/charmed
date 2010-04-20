///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_BAD_METADATA_CAST_HPP )
#define CHARMED_BAD_METADATA_CAST_HPP

#include <stdexcept>

namespace charmed
{
    struct bad_metadata_cast : std::runtime_error
    {
        bad_metadata_cast(std::type_info const& requested_, std::type_info const& actual_)
            : std::runtime_error("requested type is not tagged with supplied metadata instance")
            , requested(requested_)
            , actual(actual_)
        {}

        std::type_info const& requested;
        std::type_info const& actual;
    };
}

#endif // #if !defined( CHARMED_BAD_METADATA_CAST_HPP )
