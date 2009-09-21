#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP

#include <boost/multi_index/intrusive/detail/erase_single.hpp>
#include <boost/multi_index/intrusive/detail/erase_multiple.hpp>
#include <boost/multi_index/intrusive/detail/erase_assoc.hpp>
#include <boost/multi_index/intrusive/detail/insert_assoc.hpp>
#include <boost/multi_index/intrusive/detail/can_insert.hpp>
#include <boost/multi_index/intrusive/detail/handle_failed_insert.hpp>
#include <boost/multi_index/intrusive/detail/insert.hpp>
#include <boost/multi_index/intrusive/detail/insert_with_hint.hpp>
#include <boost/multi_index/intrusive/detail/make_pointer_tuple.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/assert.hpp>
#include <utility>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename MultiIndex, typename Index>
    inline typename Index::iterator erase(MultiIndex & mi, Index & ind, typename Index::iterator i)
    {
        // the resulting iterator
        typename Index::iterator result;

        // for every index held by mi, remove the value referenced by i
        fusion::for_each(mi.indices, erase_single<Index>(ind, *i, result));

        // return the result
        return result;
    }

    template <typename MultiIndex, typename Index>
    inline typename Index::iterator erase(MultiIndex & mi, Index & ind, typename Index::iterator f, typename Index::iterator l)
    {
        // the resulting iterator
        typename Index::iterator result;

        // for every index except ind, held by mi, remove the values between f & l
        // the values are not removed from ind so the iterator range remains valid until the end of the operation
        fusion::for_each(mi.indices, erase_multiple<Index>(ind, f, l));

        // remove the range from ind and return the result
        return ind.erase(f, l);
    }

    template <typename MultiIndex, typename Iterator>
    inline bool replace(MultiIndex const& mi, Iterator pos, typename MultiIndex::value_type const& x)
    {
        typedef typename MultiIndex::value_type value_type;
        typedef typename MultiIndex::index_view_type index_type_list;
        typedef typename make_pointer_tuple<index_type_list>::type index_ptr_tuple_type;

        // for every associative index, check if x can be inserted, assuming *pos is absent
        bool result;
        fusion::for_each(mi.indices, can_insert<value_type>(x, result));

        // if x cannot be inserted, stop now
        if (!result)
        {
            return false;
        }

        // dereference pos so we have the value after pos is rendered invalid
        value_type & value = *pos;

        // erase value from every non-sequence index
        fusion::for_each(mi.indices, erase_assoc<value_type>(value));

        // modify the value
        value = x;

        // re-insert into associative indices (there's no binary foreach, so we must iterate over a mpl range and
        // access elements using the given mpl int_.)
        bool result;
        index_ptr_tuple_type index_ptr_tuple;
        fusion::for_each(
            mpl::range_c<0, MultiIndex::index_count>(),
            insert_assoc<value_type, index_type_list>(value, result, mi.indices, index_ptr_tuple));

        // since we checked beforehand, the insertion should always work
        BOOST_ASSERT(result);

        return true;
    }

    template <typename MultiIndex, typename Iterator, typename Modifier>
    inline bool modify(MultiIndex & mi, Iterator pos, Modifier & m)
    {
        typedef typename MultiIndex::value_type value_type;
        typedef typename MultiIndex::index_view_type index_type_list;
        typedef typename make_pointer_tuple<index_type_list>::type index_ptr_tuple_type;

        // get the value before detaching
        value_type & value = *pos;

        // erase value from every non-sequence index
        fusion::for_each(mi.indices, erase_assoc<value_type>(value));

        // modify pos's value
        m(value);

        // attempt to re-insert value to every non-sequence index
        bool result;
        index_ptr_tuple_type index_ptr_tuple;
        fusion::for_each(
            mpl::range_c<0, MultiIndex::index_count>(),
            insert_assoc<value_type, index_type_list>(value, result, mi.indices, index_ptr_tuple));

        // if re-insertion failed, remove the modified value from every index that successfully added it
        if (!result)
        {
            fusion::for_each(index_ptr_tuple, handle_failed_insert<value_type>(value));
            return false;
        }

        // modification succeeded
        return true;
    }

    template <typename MultiIndex, typename Index>
    inline std::pair<typename Index::iterator, bool> insert_associative(
        MultiIndex & mi, Index & ind, typename Index::value_type & x)
    {
        typedef typename MultiIndex::value_type value_type;
        typedef typename MultiIndex::index_view_type index_type_list;
        typedef typename make_pointer_tuple<index_type_list>::type index_ptr_tuple_type;

        // insert into ind
        std::pair<typename Index::iterator, bool> result = ind.impl().insert(x);

        // if insertion fails, stop here
        if (!result.second)
        {
            return result;
        }

        // insert into all other indices
        index_ptr_tuple_type index_ptr_tuple;
        fusion::for_each(
            mpl::range_c<int, 0, MultiIndex::index_count>(),
            insert<Index, index_type_list>(x, ind, result, mi.indices, index_ptr_tuple));

        // if insertion failed, remove the value from every index
        if (!result.second)
        {
            fusion::for_each(index_ptr_tuple, handle_failed_insert<value_type>(x));
            ind.erase(ind.impl().iterator_to(x));
        }

        // return the result
        return result;
    }

    template <typename MultiIndex, typename Index>
    inline typename Index::iterator insert_associative(
        MultiIndex & mi, Index & index, typename Index::iterator pos, typename Index::value_type & x)
    {
        typedef typename MultiIndex::value_type value_type;
        typedef typename MultiIndex::index_view_type index_type_list;
        typedef typename make_pointer_tuple<index_type_list>::type index_ptr_tuple_type;

        // insert into ind
        typename Index::iterator result = ind.impl().insert(pos, x);

        // if insertion fails, stop here
        if (&*result != &x)
        {
            return result;
        }

        // insert into all other indices
        index_ptr_tuple_type index_ptr_tuple;
        fusion::for_each(
            mpl::range_c<int, 0, MultiIndex::index_count>(),
            insert_with_hint<Index, index_type_list>(ind, x, pos, result, mi.indices, index_ptr_tuple));

        // if insertion failed, remove the value from every index
        if (&*result != &x)
        {
            fusion::for_each(index_ptr_tuple, handle_failed_insert<value_type>(x));
            ind.impl().erase(ind.impl().iterator_to(x));
        }

        // return the result
        return result;
    }

    template <typename MultiIndex, typename Index>
    inline std::pair<typename Index::iterator, bool> insert_sequenced(
        MultiIndex & mi, Index & ind, typename Index::iterator pos, typename Index::value_type & x)
    {
        typedef typename MultiIndex::value_type value_type;
        typedef typename MultiIndex::index_view_type index_type_list;
        typedef typename make_pointer_tuple<index_type_list>::type index_ptr_tuple_type;

        // insert into ind
        std::pair<typename Index::iterator, bool> result = std::make_pair(ind.impl().insert(pos, x), true);

        // insert into every other index
        index_ptr_tuple_type index_ptr_tuple;
        fusion::for_each(
            mpl::range_c<int, 0, MultiIndex::index_count>(),
            insert<Index, index_type_list>(x, ind, result, mi.indices, index_ptr_tuple));

        // if insertion failed, remove the value from every index
        if (!result.second)
        {
            fusion::for_each(index_ptr_tuple, handle_failed_insert<value_type>(x));
            ind.impl().erase(ind.impl().iterator_to(x));
        }

        // return the result
        return result;
    }
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP )