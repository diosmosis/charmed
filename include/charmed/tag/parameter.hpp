///////////////////////////////////////////////////////////////////////////////
//
/// \file parameter.hpp
/// Contains the <c>CHARMED_TAG_PARAMETER()</c> attribute tagging macro.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CHARMED_TAG_PARAMETER_HPP )
#define CHARMED_TAG_PARAMETER_HPP

#include <charmed/metadata_initializer.hpp>
#include <charmed/nonunique_initializer.hpp>

#include <boost/function_types/function_arity.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/static_assert.hpp>

#include <utility>

#if defined(BOOST_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4355)
#endif

#define CHARMED_PA(f, i, expr) charmed::parameter_association<BOOST_TYPEOF(expr), BOOST_TYPEOF(f)>::type<f, i>
#define CHARMED_PRA(f, expr) charmed::param_range_attribute<                                                          \
    BOOST_TYPEOF(expr), boost::function_types::function_arity<BOOST_TYPEOF(f)>::value>

/// \brief Associates the <c>i</c>th parameter of <c>f</c> with the attribute
///        that results from <c>expr</c>.
///
/// \param f the address of the function whose parameter is to be tagged.
/// \param i the index of the parameter that is to be tagged. Use -1 to tag the
///          return type.
/// \param expr an expression that results in a constructed attribute.
#define CHARMED_TAG_PARAMETER(f, i, expr)                                                                             \
    charmed::nonunique_initializer<CHARMED_PRA(f, expr)> CHARMED_PA(f, i, expr)::fa(f, CHARMED_PRA(f, expr)());       \
                                                                                                                      \
    CHARMED_PA(f, i, expr)::unique_t CHARMED_PA(f, i, expr)::_(expr);

namespace charmed
{
    template <typename M, typename F>
    struct parameter_association;

    template <typename M, int N_>
    struct param_range_attribute
    {
        enum { N = N_ };

        param_range_attribute()
        {
            std::fill(pdata, pdata + N + 1, (metadata<M> *)0);
        }

        metadata<M> const* const* begin() const
        {
            return pdata;
        }

        metadata<M> const* const* end() const
        {
            return pdata + N + 1;
        }

        metadata<M> * pdata[N + 1];
    };

#define CHARMED_n BOOST_PP_ITERATION()
#define BOOST_PP_FILENAME_1 <charmed/tag/parameter.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, CHARMED_PARAMETER_LIMIT)
#include BOOST_PP_ITERATE()
#undef CHARMED_n
}

#if defined(BOOST_MSVC)
#   pragma warning(pop)
#endif

#endif // #if !defined( CHARMED_TAG_PARAMETER_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename M, typename R BOOST_PP_ENUM_TRAILING_PARAMS(CHARMED_n, typename A)>
    struct parameter_association<M, R(*)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A))>
    {
        template <R(*F)(BOOST_PP_ENUM_PARAMS(CHARMED_n, A)), int N>
        struct type
        {
            BOOST_STATIC_ASSERT(N >= -1 && N < CHARMED_n);

            struct unique_t
            {
                unique_t(M const& m)
                    : initializer(this, m)
                {
                    type::fa.actual->pdata[N + 1] = &initializer.data;
                }

                metadata_initializer<M> initializer;
            };

            static unique_t _;
            static nonunique_initializer<
                param_range_attribute<M, CHARMED_n>
            > fa;
        };
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
