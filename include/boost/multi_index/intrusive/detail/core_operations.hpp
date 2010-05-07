///////////////////////////////////////////////////////////////////////////////
//
/// \file core_operations.hpp
/// Contains the container modification logic for <c>multi_index_container\<\></c>.
/// These free functions ensure the guarantees of <c>multi_index_container\<\></c>
/// are held.
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
#include <boost/multi_index/intrusive/detail/can_replace.hpp>
#include <boost/multi_index/intrusive/detail/handle_failed_insert.hpp>
#include <boost/multi_index/intrusive/detail/insert.hpp>
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
        typename MultiIndex::index_views_type,
        typename fusion::result_of::advance<
            typename fusion::result_of::begin<
                typename MultiIndex::index_views_type
            >::type,
            typename Index::index_n
        >::type
    >::type all_indices_but(MultiIndex & mi, Index & ind)
    {
        return fusion::erase(mi.get_indices(), fusion::advance<Index::index_n>(fusion::begin(mi.get_indices())));
    }

    /// \brief Erases one element from every index in an intrusive <c>multi_index_container\<\></c>.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to erase from.
    /// \param ind the index for which an erase was originally requested.
    /// \param i an iterator referencing the value to be erased.
    /// \return <c>++i</c>.
    template <typename MultiIndex, typename Index>
    inline typename Index::iterator erase(MultiIndex & mi, Index & ind, typename Index::iterator i)
    {
        // the resulting iterator
        typename Index::iterator result = i;
        ++result;

        // for every index held by mi, remove the value referenced by i
        fusion::for_each(mi.get_indices(), erase_single<typename Index::value_type>(*i));

        // return the result
        return result;
    }

    /// \brief Erases all values between <b>[f,l)</b> from every index in an intrusive
    ///        <c>multi_index_container\<\></c>.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to erase from.
    /// \param ind the index for which an erase was originally requested.
    /// \param f the start of the range of values that are to be erased.
    /// \param l the end of the range of values that are to be erased.
    /// \return <c>l</c>
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

    /// \brief Changes the value referenced by <c>pos</c> to <c>x</c> while maintaining
    ///        index and <c>multi_index_container\<\></c> guarantees.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to modify.
    /// \param pos an iterator referencing the element in <c>mi</c> to modify.
    /// \param x the value to change <c>*pos</c> to. This value itself is NOT added to <c>mi</c>.
    ///          Instead, <c>*pos</c> is set to <c>x</c> and re-inserted.
    /// \return true if the operation succeeded, false if inserting <c>x</c> in an index is not
    ///         possible.
    template <typename MultiIndex, typename Iterator>
    inline bool replace(MultiIndex const& mi, Iterator pos, typename MultiIndex::value_type const& x)
    {
        typedef typename MultiIndex::value_type value_type;

        bool result = true;

        // for every associative index, check if x can be inserted, assuming *pos is absent
        fusion::for_each(mi.get_indices(), can_replace<value_type>(x, *pos, result));

        // if x cannot be inserted, stop now
        if (!result)
        {
            return false;
        }

        // dereference pos so we have the value after pos is rendered invalid
        value_type & old_value = *pos;

        // erase value from every associative index
        fusion::for_each(mi.get_indices(), erase_assoc<value_type>(old_value));

        // modify the value
        old_value = x;

        // re-insert into associative indices
        insert_assoc<value_type> ia(old_value);
        fusion::for_each(mi.get_indices(), ia);

        // since we checked beforehand, the insertion should always work
        BOOST_ASSERT(ia.result);

        return true;
    }

    // TODO: document complexity EVERYWHERE
    // TODO: Need to re-think 'modify/replace'. Should assignment be required? Since the value type will hold
    // hooks, assignment doesn't make a whole lot of sense...
    // TODO: make sure guarantees provided by intrusive multi_index match the non-intrusive version.
    /// \brief Allows the arbitrary modification of the value referenced by <c>pos</c> while
    ///        maintaining index and <c>multi_index_container\<\></c> guarantees.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to modify.
    /// \param pos an iterator referencing the element in <c>mi</c> to modify.
    /// \param m the unary function object used to modify <c>*pos</c>.
    /// \return true if the operation succeeded, false if inserting the modified value in an
    ///         an index violates the index's constraints.
    template <typename MultiIndex, typename Iterator, typename Modifier>
    inline bool modify(MultiIndex & mi, Iterator pos, Modifier & m)
    {
        typedef typename MultiIndex::value_type value_type;

        // get the value before detaching
        value_type & value = *pos;

        // copy the value in case modification fails
        value_type copy = value;

        // erase value from every non-sequence index
        fusion::for_each(mi.get_indices(), erase_assoc<value_type>(value));

        // modify pos's value
        m(value);

        // attempt to re-insert value to every non-sequence index
        insert_assoc<value_type> ia(value);
        fusion::for_each(mi.get_indices(), ia);

        // if re-insertion failed, remove the modified value from every index that successfully added it,
        // and insert the old one
        if (!ia.result)
        {
            fusion::for_each(mi.get_indices(), handle_failed_insert<value_type>(value, ia.last_index));

            value = copy;

            ia = insert_assoc<value_type>(value);
            fusion::for_each(mi.get_indices(), ia);

            BOOST_ASSERT(ia.result);

            return false;
        }

        // modification succeeded
        return true;
    }

    // TODO: generate docs & review
    /// \brief Inserts or appends <c>x</c> to every index in <c>mi</c> except <c>ind</c>.
    /// 
    /// \remarks If insertion fails, <c>x</c> is removed from all indices where insertion succeeded
    ///          including <c>ind</c>.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to insert into.
    /// \param ind the index <c>x</c> has already been inserted into.
    /// \param result if insertion succeeds, a pair holding an iterator to <c>x</c> and the value
    ///               <c>true</c>. Otherwise, a pair holding an iterator to the value that caused
    ///               insertion to fail and the value <c>false</c>.
    template <typename MultiIndex, typename Index>
    inline void insert_into_other_indices(
        MultiIndex & mi
      , Index & ind
      , typename Index::value_type & x
      , std::pair<typename Index::iterator, bool> & result)
    {
        typedef typename Index::value_type value_type;

        // insert into all other indices
        insert<typename Index::impl_type> ins(x, ind, result);
        fusion::for_each(all_indices_but(mi, ind), ins);

        // if insertion failed, remove the value from every index
        if (!result.second)
        {
            fusion::for_each(all_indices_but(mi, ind), handle_failed_insert<value_type>(x, ins.last_index));
            ind.impl().erase(ind.impl().iterator_to(x));
        }
    }

    // TODO: exception safety (for all of intrusive multi_index). Oy.
    /// \brief Inserts a value into an associative index, as well as every other index in a
    ///        <c>multi_index_container\<\></c>.
    ///
    /// \remarks If insertion fails, <c>x</c> is removed from all indices in <c>mi</c> where
    ///          insertion succeeded.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to insert into.
    /// \param ind the index for which insertion was originally requested. This must be an associative
    ///            index.
    /// \param x the value to insert. This value must be non-const so the hooks it contains can be
    ///        modified.
    /// \return a pair representing either the position of the inserted item and the value
    ///         <c>true</c>, or if insertion failed, the position of the item that caused the
    ///         failure and <c>false</c>.
    template <typename MultiIndex, typename Index>
    inline std::pair<typename Index::iterator, bool> insert_associative(
        MultiIndex & mi, Index & ind, typename Index::value_type & x)
    {
        // insert into ind
        std::pair<typename Index::iterator, bool> result = insert_associative_impl(ind.impl(), x);

        // if insertion fails, stop here
        if (!result.second)
        {
            return result;
        }

        insert_into_other_indices(mi, ind, x, result);

        return result;
    }

    /// \brief Inserts a value into <c>ind</c> using <c>pos</c> as a hint. Inserts/appends to
    ///        every other index normally.
    ///
    /// \remarks If insertion fails, <c>x</c> is removed from all indices in <c>mi</c> where
    ///          insertion succeeded.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to insert into.
    /// \param ind the index for which insertion was orignally requested. This must be an associative
    ///            index.
    /// \param pos an iterator to use as a hint when inserting into <c>ind</c>.
    /// \param x the value to insert. This value must be non-const so the hooks it contains can be
    ///        modified.
    /// \result if insertion succeeds, an iterator referencing <c>x</c>, otherwise, an iterator
    ///         referencing the value that caused insertion to fail.
    template <typename MultiIndex, typename Index>
    inline typename Index::iterator insert_associative(
        MultiIndex & mi, Index & ind, typename Index::iterator pos, typename Index::value_type & x)
    {
        // insert into ind
        typename Index::iterator result_i = ind.impl().insert(pos, x);
        std::pair<typename Index::iterator, bool> result = std::make_pair(result_i, &*result == &x);

        // if insertion fails, stop here
        if (result.second)
        {
            return result;
        }

        insert_into_other_indices(mi, ind, x, result);

        return result.first;
    }

    /// \brief Inserts <c>x</c> into <c>ind</c> before <c>pos</c>. Inserts/appends to all other
    ///        indices normally.
    ///
    /// \remarks If insertion fails, <c>x</c> is removed from all indices in <c>mi</c> where
    ///          insertion succeeded.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to insert into.
    /// \param ind the index for which insertion was originally requested. This must be a sequenced
    ///            index.
    /// \param pos the location in <c>ind</c> to insert before.
    /// \param x the value to insert. This value must be non-const so the hooks it contains can be
    ///        modified.
    /// \return a pair representing either the position of the inserted item and the value
    ///         <c>true</c>, or if insertion failed, the position of the item that caused the
    ///         failure and <c>false</c>.
    template <typename MultiIndex, typename Index>
    inline std::pair<typename Index::iterator, bool> insert_sequenced(
        MultiIndex & mi, Index & ind, typename Index::iterator pos, typename Index::value_type & x)
    {
        // insert into ind
        std::pair<typename Index::iterator, bool> result = std::make_pair(ind.impl().insert(pos, x), true);

        insert_into_other_indices(mi, ind, x, result);

        // return the result
        return result;
    }

    /// \brief Removes every value from every index in <c>mi</c>.
    ///
    /// \param mi the <c>multi_index_container\<\></c> to clear.
    template <typename MultiIndex>
    inline void clear(MultiIndex & mi)
    {
        fusion::for_each(mi.get_indices(), clear_index());
    }
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CORE_OPERATIONS_HPP )