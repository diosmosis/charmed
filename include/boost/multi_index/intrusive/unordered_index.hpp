#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_UNORDERED_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_UNORDERED_INDEX_HPP

#include <boost/multi_index/intrusive/detail/set_index.hpp>
#include <boost/multi_index/intrusive/index_fwd.hpp>

#include <boost/multi_index/detail/hash_index_args.hpp>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/unordered_set_hook.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    template <typename MultiIndex, typename KeyFromValue, typename Impl, typename Specifier>
    struct unordered_index : detail::set_index<MultiIndex, KeyFromValue, Impl, Specifier>
    {
        typedef detail::set_index<MultiIndex, KeyFromValue, Impl, Specifier> base_type;

        typedef typename impl_type::local_iterator          local_iterator;
        typedef typename impl_type::const_local_iterator    const_local_iterator;

        unordered_index(multi_index_type & x, Impl & i)
            : base_type(x, i)
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
        typedef typename multi_index::detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>     index_args;
        typedef typename index_args::tag_list_type::type                                    tag_list_type;
        typedef typename index_args::key_from_value_type                                    key_from_value_type;
        typedef typename index_args::hash_type                                              hash_type;
        typedef typename index_args::pred_type                                              pred_type;

        typedef intrusive::unordered_set_member_hook<>                                      hook_type;

        template <typename Value, typename Hook, int N>
        struct impl_index
        {
            typedef intrusive::unordered_set<
                Value,
                typename Hook::template apply<N>::type,
                intrusive::hash<member_functor_adapter<key_from_value_type, hash_type> >,
                intrusive::equal<member_functor_adapter<key_from_value_type, pred_type> >
            > type;
        };

        template <typename MultiIndex, typename Value, typename Hook, int N>
        struct index_class
        {
            typedef unordered_index<
                MultiIndex,
                key_from_value_type,
                typename impl_index<Value, Hook, N>::type,
                unordered_unique<Arg1, Arg2, Arg3, Arg4>
            > type;
        };
    };

    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct unordered_non_unique
    {
        typedef typename multi_index::detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>     index_args;
        typedef typename index_args::tag_list_type::type                                    tag_list_type;
        typedef typename index_args::key_from_value_type                                    key_from_value_type;
        typedef typename index_args::hash_type                                              hash_type;
        typedef typename index_args::pred_type                                              pred_type;

        typedef boost::intrusive::unordered_set_member_hook<>                               hook_type;

        template <typename Value, typename Hook, int N>
        struct impl_index
        {
            typedef boost::intrusive::unordered_multiset<
                Value,
                typename Hook::template apply<N>::type,
                boost::intrusive::hash<member_functor_adapter<key_from_value_type, hash_type> >,
                boost::intrusive::equal<member_functor_adapter<key_from_value_type, pred_type> >
            > type;
        };

        template <typename MultiIndex, typename Value, typename Hook, int N>
        struct index_class
        {
            typedef unordered_index<
                MultiIndex,
                key_from_value_type,
                typename impl_index<Value, Hook, N>::type,
                unordered_non_unique<Arg1, Arg2, Arg3, Arg4>
            > type;
        };
    };
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_UNORDERED_INDEX_HPP )