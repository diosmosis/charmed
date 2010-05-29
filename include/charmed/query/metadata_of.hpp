///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata_of.hpp
/// Contains the <c>metadata_of\<M\>()</c> attribute querying function.
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
#include <charmed/tag/member.hpp>
#include <charmed/tag/member_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/utility/enable_if.hpp>
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

    // overload that retrieves metadata attached to a pointer-to-member
    template <typename M, typename P>
    inline typename boost::enable_if<
        boost::is_member_object_pointer<P>,
        M const*
    >::type metadata_of(P const pointer_to_member)
    {
        typedef typename member_association<M, P>::ptm_set_type set_type;

        set_type & ptm_set = member_association<M, P>::get_ptm_set();

        typename set_type::const_iterator i = ptm_set.find(pointer_to_member, pointer_to_member_comp());

        return i == ptm_set.end() ? 0 : &i->_.data;
    }

    // overload that retrieves metadata attached to a pointer-to-member-function
    template <typename M, typename F>
    inline typename boost::enable_if<
        boost::is_member_function_pointer<F>,
        M const*
    >::type metadata_of(F mem_fun_ptr)
    {
        typedef typename member_function_association<M, F>::mem_fun_set_type set_type;

        set_type & mf_set = member_function_association<M, F>::get_mf_set();

        typename set_type::const_iterator i = mf_set.find(mem_fun_ptr, mem_fun_comp());

        return i == mf_set.end() ? 0 : &i->_.data;
    }
}}

#endif // #if !defined( CHARMED_QUERY_METADATA_OF_HPP )
