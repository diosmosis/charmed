///////////////////////////////////////////////////////////////////////////////
//
/// \file ref_metadata_initializer.hpp
/// Contains the <c>ref_metadata_initializer\<\></c> type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_REF_METADATA_INITIALIZER_HPP )
#define CHARMED_REF_METADATA_INITIALIZER_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata.hpp>
#include <charmed/type_index_of.hpp>
#include <boost/noncopyable.hpp>
#include <stdexcept>

namespace charmed
{
    // TODO: document
    template <typename A>
    struct ref_metadata_initializer
        : boost::noncopyable
    {
        ref_metadata_initializer(metadata<A> & data)
        {
            if (!type_index_of<A>().insert(data).second)
            {
                // TODO: Should this ever happen? If not, replace this with an assert in every initializer.
                throw std::runtime_error("this compile time entity has already been tagged with an equal attribute");
            }
        }
    };
}

#endif // #if !defined( CHARMED_REF_METADATA_INITIALIZER_HPP )
