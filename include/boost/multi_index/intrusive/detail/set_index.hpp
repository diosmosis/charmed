///////////////////////////////////////////////////////////////////////////////
//
/// \file set_index.hpp
/// Contains the <c>set_index\<\></c> container wrapper type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SET_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SET_INDEX_HPP

#include <boost/multi_index/intrusive/detail/common_index.hpp>
#include <boost/multi_index/intrusive/detail/key_from_value_composite.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    // TODO need rollback modify_key?
    /// \brief The base of all associative index types.
    ///
    /// <c>set_index\<\></c> is a container wrapper that derives from <c>common_index\<\></c> and
    /// defines methods that are unique to associative containers. Methods that modify the wrapped
    /// container are routed to the free functions in \ref core_operations.hpp, so other indices can
    /// be modified appropriately.
    template <typename MultiIndexTypes, int N>
    struct set_index : common_index<MultiIndexTypes, N>
    {
        typedef typename specifier::key_from_value_type key_from_value_type;
        typedef typename key_from_value_type::result_type key_type;

        set_index(multi_index_type & x, impl_type & i)
            : common_index(x, i)
        {}

        template <typename CompatibleKey>
        size_type count(CompatibleKey const& x) const
        {
            return impl().count(x);
        }

        template <typename CompatibleKey>
        std::pair<iterator, iterator> equal_range(CompatibleKey const& x) const
        {
            return impl().equal_range(x);
        }

        template <typename Modifier>
        bool modify_key(iterator position, Modifier m)
        {
            return detail::modify(container, position, key_from_value_composite<key_from_value_type, Modifier>(m));
        }

        std::pair<iterator, bool> insert(value_type & x)
        {
            return detail::insert_associative(container, *this, x);
        }

        iterator insert(iterator position, value_type & x)
        {
            return detail::insert_associative(container, *this, position, x);
        }

        template <typename Iterator>
        void insert(Iterator const& f, Iterator const& l)
        {
            for (; f != l; ++f)
            {
                insert(*f);
            }
        }

        size_type erase(value_type const& x)
        {
            // get all items w/ the key 'x'
            std::pair<iterator, iterator> rng = impl().equal_range(x);

            // get the count of all items w/ the key 'x'
            size_type result = std::distance(rng.first, rng.second);

            // erase the items found
            erase(rng.first, rng.second);

            // return the count of items erased
            return result;
        }

        iterator erase(iterator position)
        {
            return common_index::erase(position);
        }

        iterator erase(iterator first, iterator last)
        {
            return common_index::erase(first, last);
        }

        bool contains(value_type const& x) const
        {
            return impl().find(x) != impl().end();
        }
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SET_INDEX_HPP )
