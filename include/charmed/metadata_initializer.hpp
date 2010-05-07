///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata_initializer.hpp
/// Contains the <c>metadata_initializer\<\></c> type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_METADATA_INITIALIZER_HPP )
#define CHARMED_METADATA_INITIALIZER_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata.hpp>
#include <charmed/type_index_of.hpp>
#include <stdexcept>

namespace charmed
{
    /// \brief Holds a <c>metadata\<D\></c> instance. The instance is inserted into its
    ///        associated <c>multi_index_container\<\></c> upon construction of the
    ///        <c>metadata_initializer\<\></c> instance.
    ///
    /// \tparam D the attribute type.
    template <typename D>
    struct metadata_initializer
    {
        template <typename T>
        metadata_initializer(T const& x, D const& data_)
            : data(x, data_)
        {
            if (!type_index_of<D>().insert(data).second)
            {
                throw std::runtime_error("this compile time entity has already been tagged with an equal attribute");
            }
        }

        metadata<D> data;
    };
}

#endif //#if !defined( CHARMED_METADATA_INITIALIZER_HPP )
