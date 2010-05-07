///////////////////////////////////////////////////////////////////////////////
//
/// \file vector_from_element.hpp
/// Contains the mechanism used to get pointer-to-member pointers for elements
/// in a <c>fusion::vector\<\></c>.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_VECTOR_FROM_ELEMENT_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_VECTOR_FROM_ELEMENT_HPP

#include <boost/mpl/at.hpp>

#include <boost/fusion/include/vector.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#if (FUSION_MAX_VECTOR_SIZE > 40)
#   define BOOST_PP_ITERATION_LIMITS (0, 50)
#elif (FUSION_MAX_VECTOR_SIZE > 30)
#   define BOOST_PP_ITERATION_LIMITS (0, 40)
#elif (FUSION_MAX_VECTOR_SIZE > 20)
#   define BOOST_PP_ITERATION_LIMITS (0, 30)
#elif (FUSION_MAX_VECTOR_SIZE > 10)
#   define BOOST_PP_ITERATION_LIMITS (0, 20)
#else
#   define BOOST_PP_ITERATION_LIMITS (0, 10)
#endif

#define BOOST_MULTI_INDEX_INTRUSIVE_n BOOST_PP_ITERATION()

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Vector, int N>
    struct vector_member_ptr;

#define BOOST_PP_FILENAME_1 <boost/multi_index/intrusive/detail/vector_from_element.hpp>
#include BOOST_PP_ITERATE()

    /// \brief Obtains the address of a <c>fusion::vector\<\></c> based on the address of one of its
    ///        elements.
    ///
    /// \tparam Vector the type of the <c>fusion::vector\<\></c>.
    /// \tparam N the index of <c>item</c> in <c>Vector</c>.
    /// \param item the address of the item held in the <c>fusion::vector\<\></c>.
    /// \result a pointer to the fusion sequence that contains <c>*item</c>.
    template <typename Vector, int N>
    Vector * vector_from_element(typename mpl::at_c<Vector, N>::type * item)
    {
        typedef typename mpl::at_c<Vector, N>::type type;
        return boost::intrusive::detail::parent_from_member<Vector, type>(item, vector_member_ptr<Vector, N>::get());
    }

    template <typename Vector, int N>
    Vector const* vector_from_element(typename mpl::at_c<Vector, N>::type const* item)
    {
        typedef typename mpl::at_c<Vector, N>::type type;
        return boost::intrusive::detail::parent_from_member<Vector, type>(item, vector_member_ptr<Vector, N>::get());
    }
}}}}

#undef BOOST_MULTI_INDEX_INTRUSIVE_n

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_VECTOR_FROM_ELEMENT_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Vector>
    struct vector_member_ptr<Vector, BOOST_MULTI_INDEX_INTRUSIVE_n>
    {
        typedef typename mpl::at_c<Vector, BOOST_MULTI_INDEX_INTRUSIVE_n>::type result_type;

        static result_type Vector::* get()
        {
            return &Vector::BOOST_PP_CAT(m, BOOST_MULTI_INDEX_INTRUSIVE_n);
        }
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
