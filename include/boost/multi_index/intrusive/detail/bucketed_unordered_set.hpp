///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKETED_UNORDERED_SET_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKETED_UNORDERED_SET_HPP

#include <boost/intrusive/unordered_set.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
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
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_BUCKETED_UNORDERED_SET_HPP )
