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
    template <typename Tuple, int N>
    struct vector_member_ptr;

#define BOOST_PP_FILENAME_1 <boost/multi_index/intrusive/detail/vector_from_element.hpp>
#include BOOST_PP_ITERATE()

    template <typename Tuple, int N>
    Tuple * vector_from_element(typename mpl::at_c<Tuple, N>::type * item)
    {
        typedef typename mpl::at_c<Tuple, N>::type type;
        return boost::intrusive::detail::parent_from_member<Tuple, type>(item, vector_member_ptr<Tuple, N>::get());
    }

    template <typename Tuple, int N>
    Tuple const* vector_from_element(typename mpl::at_c<Tuple, N>::type const* item)
    {
        typedef typename mpl::at_c<Tuple, N>::type type;
        return boost::intrusive::detail::parent_from_member<Tuple, type>(item, vector_member_ptr<Tuple, N>::get());
    }
}}}}

#undef BOOST_MULTI_INDEX_INTRUSIVE_n

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_VECTOR_FROM_ELEMENT_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Tuple>
    struct vector_member_ptr<Tuple, BOOST_MULTI_INDEX_INTRUSIVE_n>
    {
        typedef typename mpl::at_c<Tuple, BOOST_MULTI_INDEX_INTRUSIVE_n>::type result_type;

        static result_type Tuple::* get()
        {
            return &Tuple::BOOST_PP_CAT(m, BOOST_MULTI_INDEX_INTRUSIVE_n);
        }
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
