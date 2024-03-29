///////////////////////////////////////////////////////////////////////////////
//
/// \file member.hpp
/// Contains the <c>CHARMED_TAG_MEMBER()</c> attribute tagging macro.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CHARMED_TAG_MEMBER_HPP )
#define CHARMED_TAG_MEMBER_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/ref_metadata_initializer.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/static_assert.hpp>

#define CHARMED_MA_SET(member, expr)                                                                                  \
    charmed::member_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(member)>::get_ptm_set()

#define CHARMED_MAT(member, expr)                                                                                     \
    charmed::member_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(member)>::type<member>

/// \brief Associates the attribute that results from <c>expr</c> with the
///        pointer-to-member <c>member</c>.
///
/// \param member the pointer-to-member to associate an attribute with.
/// \param expr an expression that results in a constructed attribute.
#define CHARMED_TAG_MEMBER(member, expr)                                                                              \
    charmed::metadata<BOOST_TYPEOF(expr)> CHARMED_MAT(member, expr)::attribute(                                       \
        &CHARMED_MAT(member, expr)::ptm_node.pointer_to_member, typeid(BOOST_TYPEOF(member)), expr);                  \
    charmed::pointer_to_member_node<BOOST_TYPEOF(expr), BOOST_TYPEOF(member)>                                         \
        CHARMED_MAT(member, expr)::ptm_node(                                                                          \
            member, CHARMED_MA_SET(member, expr), CHARMED_MAT(member, expr)::attribute)

namespace charmed
{
    template <typename M, typename T>
    struct member_association;

    template <typename T, typename C>
    inline std::size_t pointer_to_member_cast(T C::* ptm)
    {
        BOOST_STATIC_ASSERT(sizeof(std::size_t) == sizeof(T C::*));

        return *reinterpret_cast<std::size_t const*>(&ptm);
    }

    template <typename M>
    struct pointer_to_member_node
        : boost::intrusive::set_base_hook<>
    {
        typedef pointer_to_member_node self_type;

        template <typename PointerToMember, typename Set>
        pointer_to_member_node(PointerToMember ptm, Set & set, metadata<M> & data)
            : pointer_to_member(pointer_to_member_cast(ptm))
            , _(data)
        {
            set.insert(*this);
        }

        friend bool operator < (self_type const& x, self_type const& y)
        {
            return x.pointer_to_member < y.pointer_to_member;
        }

        std::size_t pointer_to_member;
        ref_metadata_initializer<M> _;
    };

    struct pointer_to_member_comp
    {
        template <typename M, typename PointerToMember>
        bool operator()(pointer_to_member_node<M> const& x, PointerToMember y) const
        {
            return x.pointer_to_member < pointer_to_member_cast(y);
        }

        template <typename M, typename PointerToMember>
        bool operator()(PointerToMember x, pointer_to_member_node<M> const& y) const
        {
            return pointer_to_member_cast(x) < y.pointer_to_member;
        }
    };

    template <typename M, typename T, typename C>
    struct member_association<M, T C::*>
    {
        typedef boost::intrusive::multiset<pointer_to_member_node<M> > ptm_set_type;

        // TODO: Add appropriate options for this set.
        // TODO: there should be one ptm set per C type, not per T C::* type. This needs to be moved into another type
        // that is templated only by C which will reduce the number of sets created.
        static ptm_set_type & get_ptm_set()
        {
            static ptm_set_type ptm_set;
            return ptm_set;
        }

        template <T C::* P>
        struct type
        {
            static metadata<M> attribute;
            static pointer_to_member_node<M> ptm_node;
        };
    };
}

#endif // #if !defined( CHARMED_TAG_MEMBER_HPP )
