///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata.hpp
/// Contains the <c>metadata\<D\></c> type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_METADATA_HPP )
#define CHARMED_METADATA_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/index_specifier_of.hpp>
#include <boost/multi_index/intrusive/composite_hook.hpp>

namespace charmed
{
    /// \brief The value type of the container that stores and indexes attributes.
    ///
    /// \tparam D the attribute type.
    template <typename D>
    struct metadata : D
    {
        typedef boost::multi_index::intrusive::composite_hook<typename real_index_specifier_of<D>::type> hook_type;

        template <typename T>
        metadata(T const* x, D const& d)
            : D(d)
            , tagged_data(x)
            , tagged_type(typeid(T))
        {}

        void const* tagged_data;
        std::type_info const& tagged_type;
        hook_type hook;
    };

    template <typename D>
    struct index_specifier_of<metadata<D> >
        : index_specifier_of<D>
    {};
}

#endif // #if !defined( CHARMED_METADATA_HPP )