///////////////////////////////////////////////////////////////////////////////
//
/// \file multi_index_container.hpp
/// Contains the <c>multi_index_container\<\></c> intrusive container type.
//
//  Copyright (c) 2010 Benaka Moorthi
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_MULTI_INDEX_CONTAINER_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_MULTI_INDEX_CONTAINER_HPP

#include <boost/multi_index/intrusive/index_fwd.hpp>
#include <boost/multi_index/intrusive/detail/lazy_construct_from_tuple.hpp>
#include <boost/multi_index/intrusive/detail/lazy_construct_index.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/transform_view.hpp>

#include <boost/noncopyable.hpp>

#if defined(BOOST_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4355)
#endif

// TODO list all mechanisms non-intrusive multi-index uses to accomplish task, and see if there are better ways to do it
// TODO need to check for unordered_index and store bucket array and use it in construction of multi index
namespace boost { namespace multi_index { namespace intrusive
{
    namespace detail
    {
        template <typename IndexSpecifier, typename Value, typename Hook, typename N>
        struct make_impl_index
        {
            typedef typename IndexSpecifier::template impl_index<
                Value,
                typename Hook::template apply<N::value>::type
            >::type impl_type;
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

        template <typename MultiIndexTypes, typename N>
        struct make_index
        {
            typedef typename mpl::at<typename MultiIndexTypes::index_specifier_list, N>::type specifier;
            typedef typename specifier::template index_class<MultiIndexTypes, N::value>::type type;
        };

        template <typename IndexSpecifierList>
        struct empty_args_type
            : fusion::result_of::as_vector<
                typename mpl::transform<
                    IndexSpecifierList,
                    mpl::always<fusion::vector<> >
                >::type
            >
        {};

        struct get_from_lazy_construct
        {
            template <typename Sig>
            struct result;

            template <typename T>
            struct result<get_from_lazy_construct(T &)>
            {
                typedef typename T::data_type & type;
            };

            template <typename T>
            struct result<get_from_lazy_construct(T const&)>
            {
                typedef typename T::data_type const& type;
            };

            template <typename T>
            typename result<get_from_lazy_construct(T &)>::type operator()(T & x) const
            {
                return x.get();
            }
        };
    }

    template <typename Value, typename IndexSpecifierList, typename HookSpecifier>
    struct multi_index_container_types
    {
        enum { index_count = mpl::size<IndexSpecifierList>::value };

        typedef multi_index_container_types<Value, IndexSpecifierList, HookSpecifier> mi_types;

        typedef multi_index_container<Value, IndexSpecifierList, HookSpecifier> multi_index_type;

        typedef HookSpecifier hook_specifier;
        typedef IndexSpecifierList index_specifier_list;

        typedef typename detail::make_impl_index_vector<
            Value, IndexSpecifierList, HookSpecifier
        >::type index_vector_type;

        typedef typename detail::empty_args_type<IndexSpecifierList>::type empty_args_type;

        // TODO: rename this 'index_list'
        typedef typename mpl::copy<
            mpl::range_c<int, 0, index_count>,
            mpl::inserter<
                mpl::vector<>,
                mpl::push_back<mpl::_1, detail::make_index<mi_types, mpl::_2> >
            >
        >::type index_view_type;

        typedef typename mpl::front<index_view_type>::type first_index;
    };

    /// \brief Allows one to index one set of values in different ways without allocating on the heap.
    /// 
    /// TODO: finish this
    ///
    /// \tparam Value the value type.
    /// \tparam IndexSpecifierList an MPL sequence of index specifiers.
    /// \tparam HookSpecifier the type that tells <c>multi_index_container\<\></c> how to get the hook(s)
    ///                       for each index.
    template <typename Value, typename IndexSpecifierList, typename HookSpecifier>
    struct multi_index_container
        : boost::noncopyable
        , multi_index_container_types<Value, IndexSpecifierList, HookSpecifier>
        , multi_index_container_types<Value, IndexSpecifierList, HookSpecifier>::first_index
    {
        typedef fusion::transform_view<index_vector_type, detail::get_from_lazy_construct> index_views_type;
        typedef fusion::transform_view<
            index_vector_type const, detail::get_from_lazy_construct> const_index_views_type;

        /// \brief a metafunction that gets the type of the <c>N</c>th index.
        ///
        /// \remarks This metafunction will return a container wrapper type, never a Boost.Intrusive
        ///          container.
        ///
        /// \tparam N the index of the index to get the type of.
        template <int N>
        struct nth_index
            : mpl::at_c<index_view_type, N>
        {};

        /// \brief Default constructor.
        multi_index_container()
            : first_index(*this, fusion::front(indices))
        {
            lazy_construct(empty_args_type());
        }

        /// \brief Constructs each index using the fusion sequences in <c>args_list</c>.
        ///
        /// \param args_list a fusion sequence of fusion sequences. Each element's elements are used as the
        ///                  constructor arguments to its corresponding index.
        template <typename CtorArgsList>
        multi_index_container(CtorArgsList const& args_list)
            : first_index(*this, fusion::front(indices))
        {
            lazy_construct(args_list);
        }

        /// \brief Constructs each index using the fusion sequences in <c>args_list</c> and fills the container
        ///        with the values in [first, last).
        ///
        /// \param first the first iterator in the range of values to initialize the container with.
        /// \param last the last iterator in the range of values to initialize the container with.
        /// \param args_list a fusion sequence of fusion sequences. Each element's elements are used as the
        ///                  constructor arguments to its corresponding index.
        template <typename InputIterator, typename CtorArgsList>
        multi_index_container(InputIterator first, InputIterator last, CtorArgsList const& args_list)
            : first_index(*this, fusion::front(indices))
        {
            lazy_construct(args_list);

            // TODO: What happens if this fails?
            insert(first, last);
        }

        /// \brief Destructor. Clears every index.
        ~multi_index_container() {}

        /// \brief Gets the <c>N</c>th index.
        /// \tparam N the index of the index to get.
        /// \return the <c>N</c>th index. This will be a container wrapper.
        template <int N>
        typename nth_index<N>::type get()
        {
            return typename nth_index<N>::type(*this, fusion::at_c<N>(indices));
        }

        /// \brief Gets the <c>N</c>th index.
        /// \tparam N the index of the index to get.
        /// \return the <c>N</c>th index. This will be a <c>const</c> container wrapper.
        template <int N>
        typename nth_index<N>::type const get() const
        {
            return typename nth_index<N>::type(*this, fusion::at_c<N>(indices));
        }

        /// \brief Converts an iterator of one index type to that of an iterator of <c>N</c>th index type.
        /// \tparam N the index of the index to get.
        /// \param it the iterator to convert.
        /// \return the converted iterator. It will still reference the same value.
        template <int N, typename IteratorType>
        typename nth_index<N>::type::iterator project(IteratorType it)
        {
            return get<N>().iterator_to(*it);
        }

        /// \brief Converts a <c>const</c> iterator of one index type to that of a <c>const</c> iterator of
        ///        <c>N</c>th index type.
        /// \tparam N the index of the index to get.
        /// \param it the iterator to convert.
        /// \return the converted iterator. It will still reference the same value.
        template <int N, typename IteratorType>
        typename nth_index<N>::type::const_iterator project(IteratorType it) const
        {
            return get<N>().iterator_to(*it);
        }

        /// \brief Swaps this <c>multi_index_container\<\></c> with <c>y</c>.
        /// \param y the other <c>multi_index_container\<\></c> to swap with.
        void swap(multi_index_container & y)
        {
            fusion::for_each(
                mpl::range_c<int, 0, index_count>(),
                detail::swap_index<index_vector_type>(indices, y.indices));
        }

        // TODO: This needs to be moved to an friend access type for internal use. Its not for public consumption.
        /// \brief Gets the fusion sequence of indices.
        ///
        /// \return a fusion sequence convertible to non-const references of every index's underlying
        ///         intrusive container.
        index_views_type get_indices()
        {
            return index_views_type(indices, detail::get_from_lazy_construct());
        }

        /// \brief Gets the fusion sequence of indices.
        ///
        /// \return a fusion sequence convertible to const references of every index's underlying
        ///         intrusive container.
        const_index_views_type get_indices() const
        {
            return const_index_views_type(indices, detail::get_from_lazy_construct());
        }

    private:
        template <typename CtorArgsList>
        void lazy_construct(CtorArgsList & args_list)
        {
            fusion::for_each(
                mpl::range_c<int, 0, index_count>(),
                detail::lazy_construct_index<index_vector_type, CtorArgsList>(indices, args_list));
        }

        index_vector_type indices;
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
