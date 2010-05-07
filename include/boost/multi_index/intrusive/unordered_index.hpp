///////////////////////////////////////////////////////////////////////////////
//
/// \file unordered_index.hpp
/// Contains the <c>unordered_index\<\></c> index type and the <c>unordered_unique\<\></c>
/// and <c>unordered_non_unique\<\></c> index specifier types.
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
#include <boost/intrusive/unordered_set_hook.hpp>
#include <boost/intrusive/unordered_set.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    namespace detail
    {
        template <typename Set, int N>
        struct bucket_array
        {
            typedef typename Set::bucket_type bucket_type;
            typedef typename Set::bucket_traits bucket_traits;

            bucket_array()
                : buckets()
            {}

            bucket_traits make_traits()
            {
                return bucket_traits(buckets, N);
            }

            bucket_type buckets[N];
        };

        template <typename Value, typename Hook, typename Hash, typename Equal, typename BucketCount>
        struct bucketed_unordered_set
            : bucket_array<boost::intrusive::unordered_set<Value, Hook, Hash, Equal>, BucketCount::value>
            , boost::intrusive::unordered_set<Value, Hook, Hash, Equal>
        {
            typedef boost::intrusive::unordered_set<Value, Hook, Hash, Equal> Base;
            typedef bucket_array<Base, BucketCount::value> bucket_array_type;

            typedef typename Base::value_traits       value_traits;
            typedef typename Base::bucket_traits      bucket_traits;
            typedef typename Base::iterator           iterator;
            typedef typename Base::const_iterator     const_iterator;
            typedef typename Base::bucket_ptr         bucket_ptr;
            typedef typename Base::size_type          size_type;
            typedef typename Base::hasher             hasher;
            typedef typename Base::key_equal          key_equal;

            bucketed_unordered_set(
                hasher const& hash_func = hasher()
              , key_equal const& equal_func = key_equal()
              , value_traits const& v_traits = value_traits()
            ) : Base(bucket_array_type::make_traits(), hash_func, equal_func, v_traits)
            {}

            template<class Iterator>
            bucketed_unordered_set(
                Iterator b
              , Iterator e
              , hasher const& hash_func = hasher()
              , key_equal const& equal_func = key_equal()
              , value_traits const& v_traits = value_traits()
            ) :  Base(b, e, bucket_array_type::make_traits(), hash_func, equal_func, v_traits)
            {}
        };

        template <typename Value, typename Hook, typename Hash, typename Equal, typename BucketCount>
        struct bucketed_unordered_multiset
            : bucket_array<boost::intrusive::unordered_multiset<Value, Hook, Hash, Equal>, BucketCount::value>
            , boost::intrusive::unordered_multiset<Value, Hook, Hash, Equal>
        {
            typedef boost::intrusive::unordered_multiset<Value, Hook, Hash, Equal> Base;
            typedef bucket_array<Base, BucketCount::value> bucket_array_type;

            typedef typename Base::value_traits       value_traits;
            typedef typename Base::bucket_traits      bucket_traits;
            typedef typename Base::iterator           iterator;
            typedef typename Base::const_iterator     const_iterator;
            typedef typename Base::bucket_ptr         bucket_ptr;
            typedef typename Base::size_type          size_type;
            typedef typename Base::hasher             hasher;
            typedef typename Base::key_equal          key_equal;

            bucketed_unordered_multiset(
                hasher const& hash_func = hasher()
              , key_equal const& equal_func = key_equal()
              , value_traits const& v_traits = value_traits()
            ) : Base(bucket_array_type::make_traits(), hash_func, equal_func, v_traits)
            {}

            template<class Iterator>
            bucketed_unordered_multiset(
                Iterator b
              , Iterator e
              , hasher const& hash_func = hasher()
              , key_equal const& equal_func = key_equal()
              , value_traits const& v_traits = value_traits()
            ) :  Base(b, e, bucket_array_type::make_traits(), hash_func, equal_func, v_traits)
            {}
        };
    }

    /// \brief The container wrapper type for hashed associative containers.
    ///
    /// <c>unordered_index\<\></c> wraps an intrusive container and defines methods that are only
    /// provided by hashed associative containers. Methods that modify the wrapped container
    /// are routed to the free functions in \ref core_operations.hpp, so other indices can be
    /// modified appropriately.
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

    /// \brief The index specifier used to add a unique hashed index to a
    ///        <c>multi_index_container\<\></c>.
    ///
    /// \remarks <c>unordered_unique\<\></c> uses a modified <c>boost::intrusive::unordered_set\<\></c>
    ///          as the underlying container type. This type initially holds an array of buckets which
    ///          are allocated on the stack.
    ///
    /// \tparam Arg1 the key-from-value function object. This type must be a function that object that
    ///              extracts the key from a value type.
    /// \tparam Arg2 the hashing function object. This type must be a unary function object that
    ///              accepts a key and returns a <c>std::size_t</c> hash value.
    /// \tparam Arg3 the key equality predicate. This type must be a binary function object that
    ///              tests two key instances for equality.
    /// \tparam Arg4 the bucket count. This type must be <c>bucket_count\<N\></c> where N is the
    ///              desired amount of buckets to be stored by the index.
    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct unordered_unique
    {
        typedef typename detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>                  index_args;
        typedef typename index_args::key_from_value_type                                    key_from_value_type;
        typedef typename index_args::hash_type                                              hash_type;
        typedef typename index_args::pred_type                                              pred_type;
        typedef typename index_args::bucket_count                                           bucket_count;

        typedef boost::intrusive::unordered_set_member_hook<>                               hook_type;

        template <typename Value, typename Hook>
        struct impl_index
        {
            typedef detail::bucketed_unordered_set<
                Value,
                Hook,
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

    /// \brief The index specifier used to add a non-unique hashed index to a
    ///        <c>multi_index_container\<\></c>.
    ///
    /// \remarks <c>unordered_unique\<\></c> uses a modified <c>boost::intrusive::unordered_set\<\></c>
    ///          as the underlying container type. This type initially holds an array of buckets which
    ///          are allocated on the stack.
    ///
    /// \tparam Arg1 the key-from-value function object. This type must be a function that object that
    ///              extracts the key from a value type.
    /// \tparam Arg2 the hashing function object. This type must be a unary function object that
    ///              accepts a key and returns a <c>std::size_t</c> hash value.
    /// \tparam Arg3 the key equality predicate. This type must be a binary function object that
    ///              tests two key instances for equality.
    /// \tparam Arg4 the bucket count. This type must be <c>bucket_count\<N\></c> where N is the
    ///              desired amount of buckets to be stored by the index.
    template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    struct unordered_non_unique
    {
        typedef typename detail::hashed_index_args<Arg1, Arg2, Arg3, Arg4>                  index_args;
        typedef typename index_args::key_from_value_type                                    key_from_value_type;
        typedef typename index_args::hash_type                                              hash_type;
        typedef typename index_args::pred_type                                              pred_type;
        typedef typename index_args::bucket_count                                           bucket_count;

        typedef boost::intrusive::unordered_set_member_hook<>                               hook_type;

        template <typename Value, typename Hook>
        struct impl_index
        {
            typedef detail::bucketed_unordered_multiset<
                Value,
                Hook,
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
