///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP

#include <boost/multi_index/intrusive/detail/erase_single.hpp>
#include <boost/multi_index/intrusive/detail/erase_assoc.hpp>
#include <boost/multi_index/intrusive/detail/insert_assoc.hpp>
#include <boost/multi_index/intrusive/detail/can_insert.hpp>
#include <boost/multi_index/intrusive/detail/handle_failed_insert.hpp>
#include <boost/multi_index/intrusive/detail/insert.hpp>
#include <boost/multi_index/intrusive/detail/insert_with_hint.hpp>
#include <boost/multi_index/intrusive/detail/insert_associative_impl.hpp>
#include <boost/multi_index/intrusive/detail/clear_index.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/erase.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/advance.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/assert.hpp>
#include <utility>

// TODO: There seems to be a bit of code repetition here. Should get rid of it.
namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename MultiIndex, typename Index>
    inline typename fusion::result_of::erase<
        typename MultiIndex::index_view_type,
        typename fusion::result_of::advance<
            typename fusion::result_of::begin<typename MultiIndex::index_view_type>::type,
            typename Index::index_n
        >::type
    >::type all_indices_but(MultiIndex & mi, Index & ind)
    {
        return fusion::erase(mi.indices, fusion::advance<Index::index_n>(fusion::begin(mi.indices)));
    }

    template <typename MultiIndex, typename Index>
    inline typename Index::iterator erase(MultiIndex & mi, Index & ind, typename Index::iterator i)
    {
        // the resulting iterator
        typename Index::iterator result = i;
        ++result;

        // for every index held by mi, remove the value referenced by i
        fusion::for_each(mi.indices, erase_single<typename Index::value_type>(*i));

        // return the result
        return result;
    }

    template <typename MultiIndex, typename Index>
    inline typename Index::iterator erase(MultiIndex & mi, Index & ind, typename Index::iterator f, typename Index::iterator l)
    {
        // remove every value between [f,l) from every index in mi, except ind
        for (typename Index::iterator i = f; i != l; ++i)
        {
            fusion::for_each(all_indices_but(mi, ind), erase_single<typename Index::value_type>(*i));
        }

        // remove the range from ind and return the result
        return ind.impl().erase(f, l);
    }

    // TODO: Is there a point to this function? Will 'replace' ever be used?
    template <typename MultiIndex, typename Iterator>
    inline bool replace(MultiIndex const& mi, Iterator pos, typename MultiIndex::value_type const& x)
    {
        typedef typename MultiIndex::value_type value_type;

        bool result = true;

        // for every associative index, check if x can be inserted, assuming *pos is absent
        fusion::for_each(mi.indices, can_insert<value_type>(x, *pos, result));

        // if x cannot be inserted, stop now
        if (!result)
        {
            return false;
        }

        // dereference pos so we have the value after pos is rendered invalid
        value_type & old_value = *pos;

        // erase value from every associative index
        fusion::for_each(mi.indices, erase_assoc<value_type>(old_value));

        // modify the value
        old_value = x;

        // re-insert into associative indices
        insert_assoc<value_type> ia(old_value);
        fusion::for_each(mi.indices, ia);

        // since we checked beforehand, the insertion should always work
        BOOST_ASSERT(ia.result);

        return true;
    }

    template <typename MultiIndex, typename Iterator, typename Modifier>
    inline bool modify(MultiIndex & mi, Iterator pos, Modifier & m)
    {
        typedef typename MultiIndex::value_type value_type;

        // get the value before detaching
        value_type & value = *pos;

        // copy the value in case modification fails
        value_type copy = value;

        // erase value from every non-sequence index
        fusion::for_each(mi.indices, erase_assoc<value_type>(value));

        // modify pos's value
        m(value);

        // attempt to re-insert value to every non-sequence index
        insert_assoc<value_type> ia(value);
        fusion::for_each(mi.indices, ia);

        // if re-insertion failed, remove the modified value from every index that successfully added it,
        // and insert the old one
        if (!ia.result)
        {
            fusion::for_each(mi.indices, handle_failed_insert<value_type>(value, ia.last_index));

            value = copy;

            ia = insert_assoc<value_type>(value);
            fusion::for_each(mi.indices, ia);

            BOOST_ASSERT(ia.result);

            return false;
        }

        // modification succeeded
        return true;
    }

    // TODO: exception safety (for all of intrusive multi_index). Oy.
    template <typename MultiIndex, typename Index>
    inline std::pair<typename Index::iterator, bool> insert_associative(
        MultiIndex & mi, Index & ind, typename Index::value_type & x)
    {
        typedef typename MultiIndex::value_type value_type;

        // insert into ind
        std::pair<typename Index::iterator, bool> result = insert_associative_impl(ind.impl(), x);

        // if insertion fails, stop here
        if (!result.second)
        {
            return result;
        }

        // insert into all other indices
        insert<typename Index::impl_type> ins(x, ind, result);
        fusion::for_each(all_indices_but(mi, ind), ins);

        // if insertion failed, remove the value from every index
        if (!result.second)
        {
            fusion::for_each(all_indices_but(mi, ind), handle_failed_insert<value_type>(x, ins.last_index));
            ind.impl().erase(ind.impl().iterator_to(x));
        }

        // return the result
        return result;
    }

    template <typename MultiIndex, typename Index>
    inline typename Index::iterator insert_associative(
        MultiIndex & mi, Index & index, typename Index::iterator pos, typename Index::value_type & x)
    {
        typedef typename MultiIndex::value_type value_type;

        // insert into ind
        typename Index::iterator result = ind.impl().insert(pos, x);

        // if insertion fails, stop here
        if (&*result != &x)
        {
            return result;
        }

        // insert into all other indices
        insert_with_hint<typename Index::impl_type> iwh(ind, x, pos, result);
        fusion::for_each(all_indices_but(mi, ind), iwh);

        // if insertion failed, remove the value from every index
        if (&*result != &x)
        {
            fusion::for_each(all_indices_but(mi, ind), handle_failed_insert<value_type>(x, iwh.last_index));
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

        // insert into ind
        std::pair<typename Index::iterator, bool> result = std::make_pair(ind.impl().insert(pos, x), true);

        // insert into every other index
        insert<typename Index::impl_type> ins(x, ind, result);
        fusion::for_each(all_indices_but(mi, ind), ins);

        // if insertion failed, remove the value from every index
        if (!result.second)
        {
            fusion::for_each(all_indices_but(mi, ind), handle_failed_insert<value_type>(x, ins.last_index));
            ind.impl().erase(ind.impl().iterator_to(x));
        }

        // return the result
        return result;
    }

    template <typename MultiIndex>
    inline void clear(MultiIndex & mi)
    {
        fusion::for_each(mi.indices, clear_index());
    }
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP )