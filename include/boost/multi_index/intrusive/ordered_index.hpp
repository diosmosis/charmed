///////////////////////////////////////////////////////////////////////////////
//
/// \file sequenced_index.hpp
/// Contains the <c>ordered_index\<\></c> index type and the <c>ordered_unique\<\></c>
/// and <c>ordered_non_unique\<\></c> index specifier types.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_ORDERED_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_ORDERED_INDEX_HPP

#include <boost/multi_index/intrusive/index_fwd.hpp>
#include <boost/multi_index/intrusive/detail/set_index.hpp>
#include <boost/multi_index/intrusive/detail/key_from_value_composite.hpp>
#include <boost/multi_index/detail/ord_index_args.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/intrusive/set_hook.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    // TODO: document index functions for all index types.
    /// \brief The container wrapper type for sorted associative containers.
    ///
    /// <c>ordered_index\<\></c> wraps an intrusive container and defines methods that are only
    /// provided by sorted associative containers. Methods that modify the wrapped container
    /// are routed to the free functions in \ref core_operations.hpp, so other indices can be
    /// modified appropriately.
    template <typename MultiIndexTypes, int N>
    struct ordered_index : detail::set_index<MultiIndexTypes, N>
    {
        typedef typename impl_type::reverse_iterator            reverse_iterator;
        typedef typename impl_type::const_reverse_iterator      const_reverse_iterator;

        typedef typename specifier::compare_type key_compare_type;

        ordered_index(multi_index_type & x, impl_type & i)
            : set_index(x, i)
        {}

        reverse_iterator rbegin()
        {
            return impl().rbegin();
        }

        reverse_iterator rend()
        {
            return impl().rend();
        }

        const_reverse_iterator rend() const
        {
            return impl().rend();
        }

        const_reverse_iterator rbegin() const
        {
            return impl().rbegin();
        }

        template <typename CompatibleKey>
        iterator find(CompatibleKey const& x)
        {
            return impl().find(x, detail::key_from_value_composite<key_from_value_type, key_compare_type>());
        }

        template <typename CompatibleKey>
        const_iterator find(CompatibleKey const& x) const
        {
            return impl().find(x, detail::key_from_value_composite<key_from_value_type, key_compare_type>());
        }

        template <typename CompatibleKey, typename CompatibleCompare>
        iterator find(CompatibleKey const& x, CompatibleCompare const& comp) const
        {
            return impl().find(x, detail::key_from_value_composite<key_from_value_type, CompatibleCompare>(comp));
        }

        template <typename CompatibleKey>
        size_type count(CompatibleKey const& x) const
        {
            return impl().count(x, detail::key_from_value_composite<key_from_value_type, key_compare_type>());
        }

        template <typename CompatibleKey, typename CompatibleCompare>
        size_type count(CompatibleKey const& x, CompatibleCompare const& comp) const
        {
            return impl().count(x, detail::key_from_value_composite<key_from_value_type, CompatibleCompare>(comp));
        }

        template <typename CompatibleKey>
        iterator lower_bound(CompatibleKey const& x) const
        {
            return impl().lower_bound(x, detail::key_from_value_composite<key_from_value_type, key_compare_type>());
        }

        template <typename CompatibleKey, typename CompatibleCompare>
        iterator lower_bound(CompatibleKey const& x, CompatibleCompare const& comp) const
        {
            return impl().lower_bound(x, detail::key_from_value_composite<key_from_value_type, CompatibleCompare>(comp));
        }

        template <typename CompatibleKey>
        iterator upper_bound(CompatibleKey const& x)const
        {
            return impl().upper_bound(x, detail::key_from_value_composite<key_from_value_type, key_compare_type>());
        }

        template <typename CompatibleKey, typename CompatibleCompare>
        iterator upper_bound(CompatibleKey const& x, CompatibleCompare const& comp) const
        {
            return impl().upper_bound(x, detail::key_from_value_composite<key_from_value_type, CompatibleCompare>(comp));
        }

        template <typename CompatibleKey>
        std::pair<const_iterator, const_iterator> equal_range(CompatibleKey const& x) const
        {
            return impl().equal_range(x, detail::key_from_value_composite<key_from_value_type, key_compare_type>());
        }

        template <typename CompatibleKey, typename CompatibleCompare>
        std::pair<const_iterator, const_iterator> equal_range(
            CompatibleKey const& x, CompatibleCompare const& comp) const
        {
            return impl().equal_range(x, detail::key_from_value_composite<key_from_value_type, CompatibleCompare>(comp));
        }

        template <typename LowerBounder, typename UpperBounder>
        std::pair<const_iterator, const_iterator> range(LowerBounder const& lower, UpperBounder const& upper) const
        {
            return impl().range(lower, upper);
        }

        bool key_equal(value_type const& x, value_type const& y) const
        {
            return impl().key_eq()(x, y);
        }
    };

    /// \brief The index specifier used to add a unique ordered index to a
    ///        <c>multi_index_container\<\></c>.
    ///
    /// \remarks <c>ordered_unique\<\></c> uses a <c>boost::intrusive::set\<\></c> as the underlying
    ///          container type.
    ///
    /// \tparam Arg1 the key-from-value function object. This type must be a function that object that
    ///              extracts the key from a value type.
    /// \tparam Arg2 the comparison predicate. This type must be a binary function object that compares
    ///              two key types.
    template <typename Arg1, typename Arg2>
    struct ordered_unique
    {
        typedef multi_index::detail::ordered_index_args<Arg1, Arg2, mpl::na>    index_args;
        typedef typename index_args::key_from_value_type                        key_from_value_type;
        typedef typename index_args::compare_type                               compare_type;

        typedef boost::intrusive::set_member_hook<>                             hook_type;

        template <typename Value, typename Hook>
        struct impl_index
        {
            typedef boost::intrusive::set<
                Value,
                Hook,
                boost::intrusive::compare<detail::key_from_value_composite<key_from_value_type, compare_type> >
            > type;
        };

        template <typename MultiIndex, int N>
        struct index_class
        {
            typedef ordered_index<MultiIndex, N> type;
        };
    };

    /// \brief The index specifier used to add a non-unique ordered index to a
    ///        <c>multi_index_container\<\></c>.
    ///
    /// \remarks <c>ordered_non_unique\<\></c> uses a <c>boost::intrusive::multiset\<\></c> as the
    ///          underlying container type.
    ///
    /// \tparam Arg1 the key-from-value function object. This type must be a function that object that
    ///              extracts the key from a value type.
    /// \tparam Arg2 the comparison predicate. This type must be a binary function object that compares
    ///              two key types.
    template <typename Arg1, typename Arg2>
    struct ordered_non_unique
    {
        typedef multi_index::detail::ordered_index_args<Arg1, Arg2, mpl::na>    index_args;
        typedef typename index_args::tag_list_type::type                        tag_list_type;
        typedef typename index_args::key_from_value_type                        key_from_value_type;
        typedef typename index_args::compare_type                               compare_type;

        typedef boost::intrusive::set_member_hook<>                             hook_type;

        template <typename Value, typename Hook>
        struct impl_index
        {
            typedef boost::intrusive::multiset<
                Value,
                Hook,
                boost::intrusive::compare<detail::key_from_value_composite<key_from_value_type, compare_type> >
            > type;
        };

        template <typename MultiIndex, int N>
        struct index_class
        {
            typedef ordered_index<MultiIndex, N> type;
        };
    };
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_ORDERED_INDEX_HPP )
