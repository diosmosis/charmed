///////////////////////////////////////////////////////////////////////////////
//
/// \file index_specifier_of.hpp
/// Contains the <c>index_specifier_of\<T\></c> and <c>real_index_specifier_of\<T\></c>
/// metafunctions.
//
//  Copyright (c) 2010 Benaka Moorthi
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
    namespace detail
    {
        struct get_tagged_data
        {
            typedef void const* result_type;

            template <typename M>
            void const* operator()(M & m) const
            {
                return m.tagged_data;
            }
        };
    }

    /// \brief A metafunction that returns the index specifier list used to index
    ///        an attribute type.
    ///
    /// By default, this metafunction specifies a sequenced index only. To specify
    /// a different one, or more than one, specialize this metafunction for the specific
    /// attribute type.
    ///
    /// \tparam T the attribute type.
    template <typename T>
    struct index_specifier_of
    {
        typedef boost::multi_index::indexed_by<boost::multi_index::intrusive::sequenced> type;
    };

    /// \brief A metafunction that returns the actual index specifier list used by
    ///        <c>metadata_storage\<\></c>.
    ///
    /// This metafunction adds an index to the index specifier list returned by the
    /// <c>index_specifier_of\<\></c> metafunction that indexes attributes using the
    /// type data the attributes are associated with.
    ///
    /// \t T the attribute type.
    template <typename T>
    struct real_index_specifier_of
        : boost::mpl::push_back<
            typename index_specifier_of<T>::type,
            // TODO: should this be unordered or ordered?
            boost::multi_index::intrusive::ordered_unique<detail::get_tagged_data>
        >
    {};

    template <typename D>
    struct real_index_specifier_of<metadata<D> >
        : real_index_specifier_of<D>
    {};
}

#endif // #if !defined( CHARMED_INDEX_SPECIFIER_OF_HPP )