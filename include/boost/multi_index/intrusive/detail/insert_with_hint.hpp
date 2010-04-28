///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_WITH_HINT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_WITH_HINT_HPP

#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename IndexImpl>
    struct insert_with_hint
    {
        typedef typename IndexImpl::value_type value_type;
        typedef typename IndexImpl::iterator iterator;

        template <typename Index>
        insert_with_hint(value_type & v, Index & i, iterator p, iterator & r)
            : value(v)
            , ind(i.impl())
            , pos(p)
            , result(r)
        {}

        template <typename Index>
        typename enable_if<
            is_associative_container<Index>, void
        >::type operator()(Index & other) const
        {
            typedef typename Index::iterator other_iterator;

            if (!failed())
            {
                other_iterator result_ = other.insert(pos, value);
                result = ind.iterator_to(*result_);

                ++last_failed;
            }
        }

        template <typename Index>
        typename disable_if<
            is_associative_container<Index>, void
        >::type operator()(Index & other) const
        {
            typedef typename Index::iterator other_iterator;

            if (!failed())
            {
                other.push_back(value);

                ++last_failed;
            }
        }

        bool failed() const
        {
            return &value != &*result;
        }

        IndexImpl & ind;
        value_type & value;
        iterator pos;
        iterator & result;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_WITH_HINT_HPP )