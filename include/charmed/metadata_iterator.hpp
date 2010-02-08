///////////////////////////////////////////////////////////////////////////////
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
    template <typename M>
    struct metadata_iterator
    {
        struct type
            : boost::iterator_adaptor<
                type,
                typename metadata_storage<M>::container_type::const_iterator,
                M const
            >
        {
            typedef typename type::iterator_adaptor_ base_type;
            typedef typename metadata_storage<M>::container_type::const_iterator adapted_type;

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