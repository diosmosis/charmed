///////////////////////////////////////////////////////////////////////////////
//
/// \file tagged_type_of.hpp
/// Contains the <c>tagged_type_of()</c> and <c>tagged_type_of_raw()</c>
/// attribute querying functions.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_QUERY_TAGGED_TYPE_OF_HPP )
#define CHARMED_QUERY_TAGGED_TYPE_OF_HPP

#include <charmed/metadata.hpp>
#include <charmed/metadata_storage.hpp>
#include <charmed/metadata_iterator.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_function.hpp>

#if defined( CHARMED_NOTHROW )
#   include <boost/assert.hpp>
#else
#   include <charmed/bad_metadata_cast.hpp>
#endif

namespace charmed { namespace query
{
    // TODO: It is possible to tag more than just types, so tagged_type_of must be renamed.
    namespace detail
    {
        // TODO: Won't boost::add_const take care of this?
        template <typename T>
        struct add_const
            : boost::mpl::eval_if<
                boost::is_function<T>,
                boost::mpl::identity<T>,
                boost::add_const<T>
            >
        {};
    }

    // TODO: Should everything be named 'metadata'? Or 'attribute'?
    /// \brief Gets the runtime representation of a compile time entity associated with the
    ///        supplied attribute.
    ///
    /// \tparam T the type of the runtime representation of <c>md</c>'s associated compile time info.
    ///           This will be types such as 'std::type_info', a function pointer or a pointer-to-member.
    /// \param md the attribute whose associated compile time data is being queried.
    /// \return a <c>T const</c> pointer to <c>md</c>'s associated compile time data, or <c>0</c> if none
    ///         exists.
    /// \exception bad_metadata_cast if <c>md</c> is associated with data that is not of type <c>T</c>.
    template <typename T, typename M>
    inline typename detail::add_const<T>::type * tagged_type_of(M const& md)
    {
        typedef typename detail::add_const<T>::type consted_T;

        metadata<M> const& meta = static_cast<metadata<M> const&>(md);

#if defined( CHARMED_NOTHROW )
        BOOST_ASSERT(meta.tagged_type == typeid(T));
#else
        if (meta.tagged_type != typeid(T))
        {
            throw bad_metadata_cast(typeid(T), meta.tagged_type);
        }
#endif // #if defined( CHARMED_NOTHROW )

        return static_cast<consted_T *>(meta.tagged_data);
    }

    /// \brief Gets the compile-time info associated with the supplied attribute as a <c>void const*</c>.
    ///
    /// \param md the attribute whose associated compile time data is being queried.
    template <typename M>
    inline void const* tagged_type_of_raw(M const& md)
    {
        return static_cast<metadata<M> const&>(md).tagged_data;
    }
}}

#endif // #if !defined( CHARMED_QUERY_TAGGED_TYPE_OF_HPP )
