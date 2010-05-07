///////////////////////////////////////////////////////////////////////////////
//
/// \file type_index_of.hpp
/// Contains the <c>type_index_of()</c> free function.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_TYPE_INDEX_OF_HPP )
#define CHARMED_TYPE_INDEX_OF_HPP

#include <charmed/metadata_storage.hpp>
#include <charmed/index_specifier_of.hpp>

#include <boost/mpl/size.hpp>

namespace charmed
{
    namespace result_of
    {
        /// \brief A metafuction that computes the result type of the <c>type_index_of</c>
        ///        free function.
        ///
        /// \tparam M the attribute type.
        template <typename M>
        struct type_index_of
            : metadata_storage_access<M>::container_type::template nth_index<
                boost::mpl::size<
                    typename index_specifier_of<M>::type
                >::value
            >
        {};
    }

    /// \brief Retrieves the intrusive associative container that maps compile time entities
    ///        with their associated attributes.
    ///
    /// \tparam M the metadata type.
    /// \return an intrusive associative container.
    template <typename M>
    inline typename result_of::type_index_of<M>::type type_index_of()
    {
        typedef typename index_specifier_of<M>::type index_specifier;

        return metadata_storage_access<M>::mutable_metadata().get<boost::mpl::size<index_specifier>::value>();
    }
}

#endif // #if !defined( CHARMED_TYPE_INDEX_OF_HPP )
