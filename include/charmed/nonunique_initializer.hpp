///////////////////////////////////////////////////////////////////////////////
//
/// \file nonunique_initializer.hpp
/// Contains the <c>nonunique_initializer\<\></c> type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_NONUNIQUE_INITIALIZER_HPP )
#define CHARMED_NONUNIQUE_INITIALIZER_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/metadata.hpp>
#include <charmed/type_index_of.hpp>

namespace charmed
{
    /// \brief Stores an attribute instance and inserts it into it's intrusive storage
    ///        container upon construction, if the container does not already contain the
    ///        attribute.
    ///
    /// \tparam M the attribute type.
    template <typename M>
    struct nonunique_initializer
    {
        template <typename T>
        nonunique_initializer(T const* x, M const& data_)
            : data(x, data_)
            , actual(0)
        {
            typedef typename result_of::type_index_of<M>::type index_type;

            index_type idx = type_index_of<M>();

            // TODO: thread safety issue!
            // attempt to insert the attribute. if one already exists, actual will point to that, otherwise,
            // actual will point to data.
            actual = &(*idx.insert(data).first);
        }

        metadata<M> data;
        metadata<M> * actual;
    };
}

#endif // #if !defined( CHARMED_NONUNIQUE_INITIALIZER_HPP )
