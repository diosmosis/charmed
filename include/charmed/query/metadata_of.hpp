///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata_of.hpp
/// Contains the <c>metadata_of\<M\>()</c> function.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_METADATA_OF_HPP )
#define CHARMED_QUERY_METADATA_OF_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/type_index_of.hpp>
#include <typeinfo>

namespace charmed { namespace query
{
    // TODO: It should be possible to associate more than one attribute to a type.
    /// \brief Retrieves the attribute associated with the supplied type data, if any.
    ///
    /// \tparam M the attribute type.
    /// \param type_data the runtime representation of a compile time entity, such as
    ///                  <c>std::type_info</c> pointer or a function pointer.
    /// \return a pointer to the attribute associated with <c>*type_data</c> or <c>0</c>
    ///         if there is none.
    template <typename M>
    inline M const* metadata_of(void const* type_data)
    {
        typedef typename charmed::result_of::type_index_of<M>::type index_type;

        BOOST_ASSERT(type_data);

        index_type ti = type_index_of<M>();

        typename index_type::iterator i = ti.find(type_data);
        return i == ti.end() ? 0 : &*i;
    }

    /// \brief Retrieves the attribute associated with the supplied type, if any.
    ///
    /// \tparam M the attribute type.
    /// \param type_data the runtime representation of a type.
    /// \return a pointer to the attribute associated with <c>type_data</c> or <c>0</c>
    ///         if there is none.
    template <typename M>
    inline M const* metadata_of(std::type_info const& type_data)
    {
        return metadata_of<M>(&type_data);
    }
}}

#endif // #if !defined( CHARMED_QUERY_METADATA_OF_HPP )
