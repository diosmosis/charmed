///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOC_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOC_HPP

#include <boost/multi_index/intrusive/detail/make_pointer_tuple.hpp>
#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>

#include <boost/mpl/at.hpp>

#include <boost/fusion/include/at.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Value, typename IndexTuple>
    struct insert_assoc
    {
        typedef typename make_pointer_tuple<IndexTuple>::type index_ptr_tuple;

        insert_assoc(Value & v, bool & r, IndexTuple & ind, index_ptr_tuple & ins)
            : value(v)
            , result(r)
            , indices(ind)
            , inserted(ins)
        {
            result = true;
        }

        template <typename N>
        typename enable_if<
            is_associative_container<typename mpl::at<IndexTuple, N>::type::impl_type>, void
        >::type operator()(N) const
        {
            if (result)
            {
                result = fusion::at<N>(indices).impl().insert(value).second;

                if (result)
                {
                    fusion::at<N>(inserted) = &fusion::at<N>(indices);
                }
            }
        }

        template <typename N>
        typename disable_if<
            is_associative_container<typename mpl::at<IndexTuple, N>::type::impl_type>, void
        >::type operator()(N) const
        {
            // empty
        }

        Value & value;
        bool & result;
        IndexTuple & indices;
        index_ptr_tuple & inserted;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_ASSOC_HPP )