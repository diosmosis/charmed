///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_INDEX_SPECIFIER_OF_HPP )
#define CHARMED_INDEX_SPECIFIER_OF_HPP

#include <charmed/charmed_fwd.hpp>

#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/intrusive/sequenced_index.hpp>
#include <boost/multi_index/intrusive/ordered_index.hpp>

#include <boost/mpl/push_back.hpp>

namespace charmed
{
    template <typename T>
    struct index_specifier_of
    {
        typedef boost::multi_index::indexed_by<boost::multi_index::intrusive::sequenced<> > type;
    };

    template <typename T>
    struct real_index_specifier_of
        : boost::mpl::push_back<
            typename index_specifier_of<T>::type,
            boost::multi_index::intrusive::ordered_unique<
                boost::multi_index::member<
                    immediate_metadata_base<T>, void const*, &immediate_metadata_base<T>::type_data_hook
                >
            >
        >
    {};

    template <typename D>
    struct real_index_specifier_of<metadata<D> >
        : real_index_specifier_of<D>
    {};
}

#endif // #if !defined( CHARMED_INDEX_SPECIFIER_OF_HPP )