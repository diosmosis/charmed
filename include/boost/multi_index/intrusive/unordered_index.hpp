///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_UNORDERED_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_UNORDERED_INDEX_HPP

#include <boost/multi_index/intrusive/index_fwd.hpp>
#include <boost/multi_index/intrusive/detail/set_index.hpp>
#include <boost/multi_index/intrusive/detail/key_from_value_composite.hpp>
#include <boost/multi_index/intrusive/detail/hashed_index_args.hpp>
#include <boost/multi_index/intrusive/detail/bucketed_unordered_set.hpp>

#include <boost/intrusive/unordered_set_hook.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    template <typename MultiIndexTypes, int N>
    struct unordered_index : detail::set_index<MultiIndexTypes, N>
    {
        typedef typename impl_type::local_iterator          local_iterator;
        typedef typename impl_type::const_local_iterator    const_local_iterator;

        unordered_index(multi_index_type & x, impl_type & i)
            : set_index(x, i)
        {}

        template <typename CompatibleKey>
        iterator find(CompatibleKey const& x)
        {
            return impl().find(x);
        }

        template <typename CompatibleKey>
        const_iterator find(CompatibleKey const& x) const
        {
            return impl().find(x);
        }

        template <typename CompatibleKey, typename CompatibleHash, typename CompatiblePred>
        iterator find(CompatibleKey const& x, CompatibleHash const& hash, CompatiblePred const& eq) const
        {
            return impl().find(x, hash, eq);
        }

        template <typename CompatibleKey, typename CompatibleHash, typename CompatiblePred>
        size_type count(CompatibleKey const& x, CompatibleHash const& hash, CompatiblePred const& eq) const
        {
            return impl().count(x, hash, eq);
        }

        template <typename CompatibleKey, typename CompatibleHash, typename CompatiblePred>
        std::pair<iterator, iterator> equal_range(
            CompatibleKey const& x, CompatibleHash const& hash, CompatiblePred const& eq) const
        {
            return impl().equal_range(x, hash, eq);
        }

        size_type bucket_count() const
        {
            return impl().bucket_count();
        }

        size_type max_bucket_count() const
        {
            return impl().max_bucket_count();
        }

        size_type bucket_size(size_type n) const
        {
            return impl().bucket_size(n);
        }

        size_type bucket(key_type const& k) const
        {
            return impl().bucket(k);
        }

        local_iterator local_iterator_to(value_type const& x)
        {
            return impl().local_iterator_to(x);
        }

        const_local_iterator local_iterator_to(value_type const& x) const
        {
            return impl().local_iterator_to(x);
        }
    };

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct unordered_unique
    {
        typedef typename detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>                  index_args;
        typedef typename index_args::key_from_value_type                                    key_from_value_type;
        typedef typename index_args::hash_type                                              hash_type;
        typedef typename index_args::pred_type                                              pred_type;
        typedef typename index_args::bucket_count                                           bucket_count;

        typedef boost::intrusive::unordered_set_member_hook<>                               hook_type;

        template <typename Value, typename Hook, int N>
        struct impl_index
        {
            typedef detail::bucketed_unordered_set<
                Value,
                typename Hook::template apply<N>::type,
                boost::intrusive::hash<detail::key_from_value_composite<key_from_value_type, hash_type> >,
                boost::intrusive::equal<detail::key_from_value_composite<key_from_value_type, pred_type> >,
                bucket_count
            > type;
        };

        template <typename MultiIndex, int N>
        struct index_class
        {
            typedef unordered_index<MultiIndex, N> type;
        };
    };

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct unordered_non_unique
    {
        typedef typename detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>                  index_args;
        typedef typename index_args::key_from_value_type                                    key_from_value_type;
        typedef typename index_args::hash_type                                              hash_type;
        typedef typename index_args::pred_type                                              pred_type;
        typedef typename index_args::bucket_count                                           bucket_count;

        typedef boost::intrusive::unordered_set_member_hook<>                               hook_type;

        template <typename Value, typename Hook, int N>
        struct impl_index
        {
            typedef detail::bucketed_unordered_multiset<
                Value,
                typename Hook::template apply<N>::type,
                boost::intrusive::hash<detail::key_from_value_composite<key_from_value_type, hash_type> >,
                boost::intrusive::equal<detail::key_from_value_composite<key_from_value_type, pred_type> >,
                bucket_count
            > type;
        };

        template <typename MultiIndex, int N>
        struct index_class
        {
            typedef unordered_index<MultiIndex, N> type;
        };
    };
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_UNORDERED_INDEX_HPP )
