///////////////////////////////////////////////////////////////////////////////
//
/// \file member_function.hpp
/// Contains the <c>CHARMED_TAG_MEMBER_FUNCTION()</c> attribute tagging macro.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CHARMED_TAG_MEMBER_FUNCTION_HPP )
#define CHARMED_TAG_MEMBER_FUNCTION_HPP

#include <charmed/charmed_fwd.hpp>
#include <charmed/metadata_initializer.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/typeof/typeof.hpp>
#include <algorithm>

#define CHARMED_MFA(f, expr) charmed::member_function_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f)>

/// \brief Associates the attribute that results from <c>expr</c> with the
///        pointer-to-member-function <c>f</c>.
///
/// \param f the pointer-to-member-function to associate an attribute with.
/// \param expr an expression that results in a constructed attribute.
#define CHARMED_TAG_MEMBER_FUNCTION(f, expr)                                                                          \
    charmed::mem_fun_node<BOOST_TYPEOF(expr), BOOST_TYPEOF(f)> CHARMED_MFA(f, expr)::type<f>::                        \
        mf_node(f, CHARMED_MFA(f, expr)::get_mf_set(), expr)

namespace charmed
{
    template <typename M, typename F>
    struct member_function_association;

    template <typename M, typename F>
    struct mem_fun_node
        : boost::intrusive::set_base_hook<>
    {
        typedef mem_fun_node self_type;

        template <typename Set>
        mem_fun_node(F fptr, Set & set, M const& data)
            : mem_fun_ptr(fptr)
            , _(&mem_fun_ptr, data)
        {
            set.insert(*this);
        }

        F mem_fun_ptr;
        metadata_initializer<M> _;
    };

    struct mem_fun_comp
    {
        template <typename M, typename F>
        bool operator()(mem_fun_node<M, F> const& lhs, F const& rhs) const
        {
            return (*this)(lhs.mem_fun_ptr, rhs);
        }

        template <typename M, typename F>
        bool operator()(F const& lhs, mem_fun_node<M, F> const& rhs) const
        {
            return (*this)(lhs, rhs.mem_fun_ptr);
        }

        template <typename M, typename F>
        bool operator()(mem_fun_node<M, F> const& lhs, mem_fun_node<M, F> const& rhs) const
        {
            return (*this)(lhs.mem_fun_ptr, rhs.mem_fun_ptr);
        }

        template <typename F>
        bool operator()(F const& lhs, F const& rhs) const
        {
            BOOST_STATIC_ASSERT(sizeof(F) % sizeof(std::size_t) == 0);

            std::size_t const* lhs_f = reinterpret_cast<std::size_t const*>(&lhs);
            std::size_t const* rhs_f = reinterpret_cast<std::size_t const*>(&rhs);

            bool r = std::lexicographical_compare(
                lhs_f, lhs_f + sizeof(F) / sizeof(std::size_t),
                rhs_f, rhs_f + sizeof(F) / sizeof(std::size_t));

            return r;
        }
    };

    #define CHARMED_n BOOST_PP_ITERATION()
    #define BOOST_PP_FILENAME_1 <charmed/tag/member_function.hpp>
    #define BOOST_PP_ITERATION_LIMITS (0, CHARMED_PARAMETER_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CHARMED_n
}

#endif // #if !defined( CHARMED_TAG_MEMBER_FUNCTION_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename M, typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(CHARMED_n, typename A)>
    struct member_function_association<M, R(T::*)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A))>
    {
        typedef R(T::*function_type)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A));

        typedef boost::intrusive::multiset<
            mem_fun_node<M, function_type>, boost::intrusive::compare<mem_fun_comp>
        > mem_fun_set_type;

        // TODO: Add appropriate options for this set.
        static mem_fun_set_type & get_mf_set()
        {
            static mem_fun_set_type mf_set;
            return mf_set;
        }

        template <R(T::*F)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A))>
        struct type
        {
            static mem_fun_node<M, function_type> mf_node;
        };
    };

    template <typename M, typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(CHARMED_n, typename A)>
    struct member_function_association<M, R(T::*)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A)) const>
    {
        typedef R(T::*function_type)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A)) const;

        typedef boost::intrusive::multiset<
            mem_fun_node<M, function_type>, boost::intrusive::compare<mem_fun_comp>
        > mem_fun_set_type;

        // TODO: Add appropriate options for this set.
        static mem_fun_set_type & get_mf_set()
        {
            static mem_fun_set_type mf_set;
            return mf_set;
        }

        template <R(T::*F)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A)) const>
        struct type
        {
            static mem_fun_node<M, function_type> mf_node;
        };
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
