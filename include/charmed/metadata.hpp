///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_METADATA_HPP )
#define CHARMED_METADATA_HPP

#include <charmed/index_specifier_of.hpp>

#include <boost/multi_index/intrusive/composite_hook.hpp>

#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/utility/enable_if.hpp>

namespace charmed
{
    template <typename D>
    struct immediate_metadata_base : D
    {
        immediate_metadata_base(void const* x, D const& d)
            : D(d), type_data_hook(x)
        {}

        void const* type_data_hook;
    };

    template <typename D>
    struct metadata : immediate_metadata_base<D>
    {
        typedef boost::multi_index::intrusive::composite_hook<typename real_index_specifier_of<D>::type> hook_type;

        template <typename T>
        metadata(T const& x, D const& d)
            : immediate_metadata_base(x, d)
        {}

        hook_type hook;
    };

    template <typename D>
    struct index_specifier_of<metadata<D> >
        : index_specifier_of<D>
    {};
}

#endif // #if !defined( CHARMED_METADATA_HPP )