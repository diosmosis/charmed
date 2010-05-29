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
        template <typename M, int N = 0>
        struct all_attributes
        {
            typedef typename metadata_range<M, N>::type type;
        };
    }

    /// \brief Retrieves all attributes of type <c>M</c> using the <c>N</c>th attribute index.
    ///
    /// \tparam M the attribute type.
    /// \tparam N the index of the attribute index to use.
    /// \return a range of type <c>metadata_range\<M, N\>::type</c> that iterates over every
    ///         attribute of type <c>M</c>.
    template <typename M, int N>
    inline typename result_of::all_attributes<M, N>::type all_attributes()
    {
        typedef typename result_of::all_attributes<M, N>::type result_type;
        return result_type(metadata_storage<M>::metadata().get<N>());
    }

    /// \brief Retrieves all attributes of type <c>M</c> using the first attribute index.
    ///
    /// \tparam M the attribute type.
    /// \return a range of type <c>metadata_range\<M\>::type</c> that iterates over every
    ///         attribute of type <c>M</c>.
    template <typename M>
    inline typename result_of::all_attributes<M>::type all_attributes()
    {
        return all_attributes<M, 0>();
    }
}}

#endif // #if !defined( CHARMED_QUERY_GET_ALL_ATTRIBUTES_HPP )