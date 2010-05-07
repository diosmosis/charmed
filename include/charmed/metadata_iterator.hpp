///////////////////////////////////////////////////////////////////////////////
//
/// \file metadata_iterator.hpp
/// Contains the <c>metadata_iterator\<M,N\></c> metafunction.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_METADATA_ITERATOR_HPP )
#define CHARMED_METADATA_ITERATOR_HPP

#include <charmed/metadata_storage.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

namespace charmed
{
    /// \brief A metafunction that returns an iterator that iterates over
    ///        attributes of type <c>M</c>.
    ///
    /// \tparam M the attribute type.
    /// \tparam N the attribute index to use. Defaults to <c>0</c>.
    template <typename M, int N = 0>
    struct metadata_iterator
    {
        typedef typename metadata_storage<M>::container_type::template nth_index<N>::type index_type;

        struct type
            : boost::iterator_adaptor<
                type,
                typename index_type::const_iterator,
                M const
            >
        {
            typedef typename type::iterator_adaptor_ base_type;
            typedef typename index_type::const_iterator adapted_type;

            type()
                : base_type()
            {}

            type(type const& x)
                : base_type(x)
            {}

            type(adapted_type const& x)
                : base_type(x)
            {}

            type & operator = (type const& x)
            {
                base_type::operator = (x);
                return *this;
            }

            type & operator = (adapted_type const& x)
            {
                base_type::operator = (x);
                return *this;
            }

        private:
            friend class boost::iterator_core_access;

            M const& dereference() const
            {
                return static_cast<M const&>(*base());
            }
        };
    };
}

#endif // #if !defined( CHARMED_METADATA_ITERATOR_HPP )