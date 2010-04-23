///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP

#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>
#include <boost/multi_index/intrusive/detail/insert_associative_impl.hpp>
#include <boost/mpl/at.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename IndexImpl>
    struct insert
    {
        typedef typename IndexImpl::value_type value_type;
        typedef typename IndexImpl::iterator iterator;
        typedef std::pair<iterator, bool> pair_result;

        template <typename Index>
        insert(value_type & v, Index & i, pair_result & r)
            : value(v)
            , ind(i.impl())
            , result(r)
            , last_failed(-1)
        {
            result.second = true;
        }

        template <typename Index>
        typename enable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index & other) const
        {
            if (result.second && static_cast<void *>(&ind) != static_cast<void *>(&other.impl()))
            {
                std::pair<typename Index::iterator, bool> r = insert_associative_impl(other.impl(), value);
                result = std::make_pair(ind.iterator_to(*r.first), r.second);

                ++last_failed;
            }
        }

        template <typename Index>
        typename disable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index & other) const
        {
            if (result.second && static_cast<void *>(&ind) != static_cast<void *>(&other.impl()))
            {
                other.impl().push_back(value);

                ++last_failed;
            }
        }

        IndexImpl & ind;
        value_type & value;
        pair_result & result;
        mutable unsigned int last_failed;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP )