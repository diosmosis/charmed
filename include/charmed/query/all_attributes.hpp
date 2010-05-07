///////////////////////////////////////////////////////////////////////////////
//
/// \brief all_attributes.hpp
/// Contains the <c>all_attributes\<M\>()</c> attribute querying function.
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
        /// \brief A metafunction that returns the result type of the <c>all_attributes\<M\>()</c>
        ///        free function.
        ///
        /// <c>all_attributes\<\></c> will return an iterator range that provides immutable access
        /// to an attribute and iterates over the first index of the attribute's storage container.
        ///
        /// \tparam M the attribute type.
        template <typename M>
        struct all_attributes
        {
            typedef typename metadata_range<M>::type type;
        };
    }

    /// \brief Retrieves all attributes of type <c>M</c>.
    ///
    /// \tparam M The attribute type.
    /// \return a range of type <c>metadata_range\<M\>::type</c> that iterates over every
    ///         attribute of type <c>M</c>.
    template <typename M>
    inline typename result_of::all_attributes<M>::type all_attributes()
    {
        typedef typename result_of::all_attributes<M>::type result_type;
        return result_type(metadata_storage<M>::metadata());
    }
}}

#endif // #if !defined( CHARMED_QUERY_GET_ALL_ATTRIBUTES_HPP )