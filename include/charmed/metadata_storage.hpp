///////////////////////////////////////////////////////////////////////////////
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
#include <charmed/hook_specifier_of.hpp>

#include <boost/multi_index/intrusive/multi_index_container.hpp>

namespace charmed
{
    template <typename D>
    struct metadata_storage
    {
        typedef metadata_storage<D> self_type;

        typedef typename real_index_specifier_of<D>::type real_index_specifier;
        typedef typename hook_specifier_of<metadata<D> >::type hook_specifier;

        typedef boost::multi_index::intrusive::multi_index_container<
            metadata<D>, real_index_specifier, hook_specifier> container_type;

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

    template <typename D>
    struct metadata_storage_access
    {
        static typename metadata_storage<D>::container_type & mutable_metadata()
        {
            return metadata_storage<D>::mutable_metadata();
        }
    };
}

#endif // #if !defined( CHARMED_DETAIL_METADATA_STORAGE_HPP )
