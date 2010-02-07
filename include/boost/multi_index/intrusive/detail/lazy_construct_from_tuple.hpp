///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_FROM_TUPLE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_FROM_TUPLE_HPP

#include <boost/config.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>

#include <boost/fusion/include/at.hpp>

#include <boost/aligned_storage.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    // TODO use preprocessor include iteration
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

        template <typename Tuple>
        void construct_impl(Tuple const& x, mpl::int_<0>)
        {
            new (data.address()) T();
        }

        template <typename Tuple>
        void construct_impl(Tuple const& x, mpl::int_<1>)
        {
            new (data.address()) T(fusion::at_c<0>(x));
        }

        template <typename Tuple>
        void construct_impl(Tuple const& x, mpl::int_<2>)
        {
            new (data.address()) T(fusion::at_c<0>(x), fusion::at_c<1>(x));
        }

        template <typename Tuple>
        void construct_impl(Tuple const& x, mpl::int_<3>)
        {
            new (data.address()) T(fusion::at_c<0>(x), fusion::at_c<1>(x), fusion::at_c<2>(x));
        }

        template <typename Tuple>
        void construct_impl(Tuple const& x, mpl::int_<4>)
        {
            new (data.address()) T(fusion::at_c<0>(x), fusion::at_c<1>(x), fusion::at_c<2>(x), fusion::at_c<3>(x));
        }

        operator T & () { return get(); }
        operator T const& () const { return get(); }

        T & get() { return *static_cast<T *>(data.address()); }
        T const& get() const { return *static_cast<T const*>(data.address()); }

        aligned_storage<sizeof(T), alignment_of<T>::value> data;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_LAZY_CONSTRUCT_FROM_TUPLE_HPP )