///////////////////////////////////////////////////////////////////////////////
//
/// \file insert.hpp
/// Contains the <c>insert\<\></c> function object.
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
    /// \brief A function object used by the free functions in core_operations.hpp
    ///        to insert a value into every index of a <c>multi_index_container</c>.
    /// 
    /// <c>insert\<\></c> accepts a non-const reference to an index. If the index is
    /// sequenced, the value supplied during construction will be <c>push_back</c>ed.
    /// If the index is associative, insertion is attempted. In either case the
    /// iterator result is saved. If an insertion fails, no other insertion/push_back
    /// is attempted. In addition, the expression <c>result.second</c> will evaluate to
    /// false and the member <c>last_index</c> will reference the index of the index
    /// where insertion failed.
    ///
    /// <c>insert\<\></c> also holds a reference to the index for whom insertion was
    /// originally requested. This reference is used to convert iterators that result
    /// from inserts to the appropriate type.
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
            , last_index(-1)
        {
            result.second = true;
        }

        template <typename Index>
        typename enable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index & other) const
        {
            if (result.second)
            {
                std::pair<typename Index::iterator, bool> r = insert_associative_impl(other.impl(), value);
                result = std::make_pair(ind.iterator_to(*r.first), r.second);

                ++last_index;
            }
        }

        template <typename Index>
        typename disable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index & other) const
        {
            if (result.second)
            {
                other.impl().push_back(value);

                ++last_index;
            }
        }

        IndexImpl & ind;
        value_type & value;
        pair_result & result;
        mutable unsigned int last_index;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP )