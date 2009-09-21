#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_WITH_HINT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_WITH_HINT_HPP

#include <boost/multi_index/intrusive/detail/make_pointer_tuple.hpp>
#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>

#include <boost/mpl/at.hpp>

#include <boost/fusion/include/at.hpp>

#include <boost/utility/enable_if.hpp>

#include <utility>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Index, typename IndexTuple>
    struct insert_with_hint
    {
        typedef typename Index::value_type value_type;
        typedef typename make_pointer_tuple<IndexTuple>::type index_ptr_tuple;
        typedef typename Index::iterator iterator;

        insert_with_hint(value_type & v, Index & i, iterator p, iterator & r, IndexTuple & indtup, index_ptr_tuple & ins)
            : value(v)
            , ind(i)
            , pos(p)
            , result(r)
            , indices(indtup)
            , inserted(ins)
        {
            result.second = true;
        }

        template <typename N>
        typename enable_if<
            is_associative_container<typename mpl::at<IndexTuple, N>::type::impl_type>, void
        >::type operator()(N) const
        {
            typedef typename mpl::at<IndexTuple, N>::type other_type;
            typedef other_type::iterator other_iterator;

            other_type & other = fusion::at<N>(indices);

            if (!failed() && static_cast<void *>(&ind.impl()) != static_cast<void *>(&other.impl()))
            {
                other_iterator result_ = fusion::at<N>(indices).impl().insert(pos, value);
                result = ind.impl().iterator_to(*result_);

                if (!failed())
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
            typedef typename mpl::at<IndexTuple, N>::type other_type;
            typedef other_type::iterator other_iterator;

            other_type & other = fusion::at<N>(indices);

            if (!failed() && static_cast<void *>(&ind.impl()) != static_cast<void *>(&other.impl()))
            {
                other.impl().push_back(value);
                fusion::at<N>(inserted) = &other;
            }
        }

        bool failed() const
        {
            return &value != &*result;
        }

        Index & ind;
        value_type & value;
        iterator pos;
        iterator & result;
        IndexTuple & indices;
        index_ptr_tuple & inserted;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_WITH_HINT_HPP )