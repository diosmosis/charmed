///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_FROM_TUPLE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_FROM_TUPLE_HPP

#include <boost/config.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>

#include <boost/fusion/include/at.hpp>
#include <boost/fusion/container/vector/limits.hpp>

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#include <boost/aligned_storage.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename T>
    struct lazy_construct_from_tuple
    {
        lazy_construct_from_tuple() : data() {}
        ~lazy_construct_from_tuple()
        {
            static_cast<T *>(data.address())->~T();
        }

        template <typename Tuple>
        void construct_(Tuple const& x)
        {
            construct_impl(x, typename mpl::size<Tuple>::type());
        }

#define BOOST_n BOOST_PP_ITERATION()
#define BOOST_fusion_at(z, n, text) fusion::at_c<n>(x)

#define BOOST_PP_FILENAME_1 <boost/multi_index/intrusive/detail/lazy_construct_from_tuple.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, FUSION_MAX_VECTOR_SIZE)
#include BOOST_PP_ITERATE()

#undef BOOST_n
#undef BOOST_fusion_at

        operator T & () { return get(); }
        operator T const& () const { return get(); }

        T & get() { return *static_cast<T *>(data.address()); }
        T const& get() const { return *static_cast<T const*>(data.address()); }

        aligned_storage<sizeof(T), alignment_of<T>::value> data;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_FROM_TUPLE_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Tuple>
    void construct_impl(Tuple const& x, mpl::int_<BOOST_n>)
    {
        new (data.address()) T(BOOST_PP_ENUM(BOOST_n, BOOST_fusion_at, _));
    }

#endif // #if !defined( BOOST_PP_IS_ITERATING )
