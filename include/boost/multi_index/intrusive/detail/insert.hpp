#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP

#include <boost/multi_index/intrusive/detail/make_pointer_tuple.hpp>
#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>

#include <boost/mpl/at.hpp>

#include <boost/fusion/include/at.hpp>

#include <boost/utility/enable_if.hpp>

#include <utility>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Index, typename IndexTuple>
    struct insert
    {
        typedef typename Index::value_type value_type;
        typedef typename make_pointer_tuple<IndexTuple>::type index_ptr_tuple;
        typedef std::pair<typename Index::iterator, bool> result_type;

        insert(value_type & v, Index & i, result_type & r, IndexTuple & indtup, index_ptr_tuple & ins)
            : value(v)
            , ind(i)
            , result(r)
            , indices(indtup)
            , inserted(ins)
        {
            result.second = true;
        }

        // TODO this can be optimized further by putting an operator() for 
        template <typename N>
        typename enable_if<
            is_associative_container<typename mpl::at<IndexTuple, N>::type::impl_type>, void
        >::type operator()(N) const
        {
            typedef typename mpl::at<IndexTuple, N>::type other_type;
            typedef other_type::iterator other_iterator;

            other_type & other = fusion::at<N>(indices);

            if (result.second && static_cast<void *>(&ind.impl()) != static_cast<void *>(&other.impl()))
            {
                std::pair<other_iterator, bool> result_ = other.impl().insert(value);
                result = result_type(ind.impl().iterator_to(*result_.first), result_.second);

                if (result.second)
                {
                    fusion::at<N>(inserted) = &other;
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

            if (result.second && static_cast<void *>(&ind.impl()) != static_cast<void *>(&other.impl()))
            {
                other.impl().push_back(value);
                fusion::at<N>(inserted) = &other;
            }
        }

        Index & ind;
        value_type & value;
        result_type & result;
        IndexTuple & indices;
        index_ptr_tuple & inserted;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_INSERT_HPP )