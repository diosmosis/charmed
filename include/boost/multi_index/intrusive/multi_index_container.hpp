///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_MULTI_INDEX_CONTAINER_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_MULTI_INDEX_CONTAINER_HPP

#include <boost/multi_index/intrusive/detail/lazy_construct_from_tuple.hpp>
#include <boost/multi_index/intrusive/detail/lazy_construct_index.hpp>
#include <boost/multi_index/intrusive/detail/create_index_wrapper.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/transform_view.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/noncopyable.hpp>

#if defined(BOOST_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4355)
#endif

// TODO list all mechanisms non-intrusive multi-index uses to accomplish task, and see if there are better ways to do it
// TODO get rid of range_c use if possible
// TODO need to check for unordered_index and store bucket array and use it in construction of multi index
namespace boost { namespace multi_index { namespace intrusive
{
    namespace detail
    {
        template <typename IndexSpecifier, typename Value, typename Hook, typename N>
        struct make_impl_index
        {
            typedef typename IndexSpecifier::template impl_index<Value, Hook, N::value>::type impl_type;
            typedef lazy_construct_from_tuple<impl_type> type;
        };

        template <typename Value, typename IndexSpecifierList, typename HookSpecifier>
        struct make_impl_index_vector
            : fusion::result_of::as_vector<
                typename mpl::transform<
                    IndexSpecifierList,
                    mpl::range_c<int, 0, mpl::size<IndexSpecifierList>::value>,
                    make_impl_index<mpl::_1, Value, HookSpecifier, mpl::_2>
                >::type
            >
        {};

        template <typename IndexSpecifierList>
        struct empty_args_type
            : fusion::result_of::as_vector<
                typename mpl::transform<
                    IndexSpecifierList,
                    mpl::always<fusion::vector<> >
                >::type
            >
        {};
    }

    template <typename Value, typename IndexSpecifierList, typename HookSpecifier>
    struct multi_index_container
        : boost::noncopyable,
          boost::mpl::front<IndexSpecifierList>::type::template index_class<
              multi_index_container<Value, IndexSpecifierList, HookSpecifier>, Value, HookSpecifier, 0
          >::type
    {
        enum { index_count = mpl::size<IndexSpecifierList>::value };

        typedef multi_index_container<Value, IndexSpecifierList, HookSpecifier> self_type;

        typedef typename detail::make_impl_index_vector<Value, IndexSpecifierList, HookSpecifier>::type local_index_tuple_type;
        typedef mpl::range_c<int, 0, index_count> index_range;

        typedef fusion::transform_view<index_range, detail::create_index_wrapper<self_type> > index_view_type;

        typedef typename mpl::front<IndexSpecifierList>::type::template index_class<
            self_type, Value, HookSpecifier, 0
        >::type base_type;

        typedef typename detail::empty_args_type<IndexSpecifierList>::type empty_args_type;

        typedef HookSpecifier hook_specifier;
        typedef IndexSpecifierList index_specifier_list;

        template <int N>
        struct nth_index
            : fusion::result_of::at_c<index_view_type, N>
        {};

        // TODO need tag support
        //template <typename Tag>
        //struct index
        //    : boost::mpl::at<local_index_tuple_type, Tag>
        //{};

        multi_index_container()
            : base_type(*this, fusion::front(index_storage))
            , indices(index_range(), detail::create_index_wrapper<self_type>(*this))
        {
            lazy_construct(empty_args_type());
        }

        template <typename CtorArgsList>
        multi_index_container(CtorArgsList const& args_list)
            : base_type(*this, fusion::front(index_storage))
            , indices(index_range(), detail::create_index_wrapper<self_type>(*this))
        {
            lazy_construct(args_list);
        }

        template <typename InputIterator, typename CtorArgsList>
        multi_index_container(InputIterator first, InputIterator last, CtorArgsList const& args_list)
            : base_type(*this, fusion::front(index_storage))
            , indices(index_range(), detail::create_index_wrapper<self_type>(*this))
        {
            lazy_construct(args_list);
        }

        ~multi_index_container() {}

        template <int N>
        typename nth_index<N>::type get()
        {
            return fusion::at_c<N>(indices);
        }

        template <int N>
        typename nth_index<N>::type const get() const
        {
            return fusion::at_c<N>(indices);
        }

        //template <typename Tag>
        //typename index<Tag>::type & get()
        //{
        //    TODO
        //}

        //template <typename Tag>
        //typename index<Tag>::type const& get() const
        //{
        //    TODO
        //}

        template <int N, typename IteratorType>
        typename nth_index<N>::type::iterator project(IteratorType it)
        {
            return get<N>().iterator_to(*it);
        }

        template <int N, typename IteratorType>
        typename nth_index<N>::type::const_iterator project(IteratorType it) const
        {
            return get<N>().iterator_to(*it);
        }

        //template <typename Tag, typename IteratorType>
        //typename index<Tag>::type::iterator project(IteratorType it)
        //{
        //    TODO
        //}

        //template <typename Tag, typename IteratorType>
        //typename index<Tag>::type::const_iterator project(IteratorType it) const
        //{
        //    TODO
        //}

        void swap(self_type & y)
        {
            fusion::for_each(
                mpl::range_c<int, 0, index_count>(),
                detail::swap_index<local_index_tuple_type>(indices, y.indices));
        }

        template <typename CtorArgsList>
        void lazy_construct(CtorArgsList & args_list)
        {
            fusion::for_each(
                mpl::range_c<int, 0, index_count>(),
                detail::lazy_construct_index<local_index_tuple_type, CtorArgsList>(index_storage, args_list));
        }

        local_index_tuple_type index_storage;
        index_view_type indices;
    };

    template <typename V, typename I, typename H>
    inline void swap(multi_index_container<V, I, H> & x, multi_index_container<V, I, H> & y)
    {
        x.swap(y);
    }
}}}

#if defined(BOOST_MSVC)
#   pragma warning(pop)
#endif

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_MULTI_INDEX_CONTAINER_HPP )
