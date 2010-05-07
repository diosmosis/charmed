///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata_storage.hpp
/// Contains the metadata_storage\<D\></c> and <c>metadata_storage_access\<D\></c>
/// types.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_DETAIL_METADATA_STORAGE_HPP )
#define CHARMED_DETAIL_METADATA_STORAGE_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/index_specifier_of.hpp>

#include <boost/multi_index/intrusive/multi_index_container.hpp>
#include <boost/multi_index/intrusive/fusion_hook.hpp>

namespace charmed
{
    namespace detail
    {
        template <typename T>
        struct hook_specifier_of
        {
            typedef boost::multi_index::intrusive::fusion_hook_specifier<T, typename T::hook_type, &T::hook> type;
        };
    }

    // TODO: Not really sure how to document this type...
    /// \brief Holds the container that indexes all attributes of a certain type.
    ///
    /// \tparam D the attribute type.
    template <typename D>
    struct metadata_storage
    {
        typedef metadata_storage<D> self_type;

        typedef typename real_index_specifier_of<D>::type real_index_specifier;
        typedef typename detail::hook_specifier_of<metadata<D> >::type hook_specifier;

        /// \brief The attribute container type.
        typedef boost::multi_index::intrusive::multi_index_container<
            metadata<D>, real_index_specifier, hook_specifier> container_type;

        /// \brief Provides immutable access to the attribute container.
        static container_type const& metadata()
        {
            return mutable_metadata();
        }

    private:
        template <typename D0>
        friend struct metadata_storage_access;

        static container_type & mutable_metadata()
        {
            static container_type metadata_container;
            return metadata_container;
        }
    };

    /// \brief Provides mutable access to an attribute container.
    ///
    /// \tparam The attribute type.
    template <typename D>
    struct metadata_storage_access
    {
        typedef typename metadata_storage<D>::container_type container_type;

        /// \brief Provides mutable access to the container that holds attributes of type
        ///        <c>D</c>.
        static container_type & mutable_metadata()
        {
            return metadata_storage<D>::mutable_metadata();
        }
    };
}

#endif // #if !defined( CHARMED_DETAIL_METADATA_STORAGE_HPP )
