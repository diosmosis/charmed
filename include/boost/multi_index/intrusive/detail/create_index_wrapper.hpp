#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CREATE_INDEX_WRAPPER_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CREATE_INDEX_WRAPPER_HPP

#include <boost/mpl/at.hpp>

#include <boost/fusion/include/at.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename MultiIndex>
    struct create_index_wrapper
    {
        typedef typename MultiIndex::index_specifier_list index_specifier_list;
        typedef typename MultiIndex::value_type value_type;
        typedef typename MultiIndex::hook_specifier hook_specifier;

        template <typename N>
        struct result;

        template <typename N>
        struct result<create_index_wrapper(N)>
        {
            typedef typename mpl::at<index_specifier_list, N>::type specifier;
            typedef typename specifier::template index_class<
                MultiIndex, value_type, hook_specifier, N::value>::type type;
        };

        create_index_wrapper(MultiIndex & mi_)
            : mi(mi_)
        {}

        template <typename N>
        typename result<create_index_wrapper(N)>::type operator()(N) const
        {
            typedef typename result<create_index_wrapper(N)>::type index_type;
            return index_type(mi, fusion::at<N>(mi.index_storage));
        }

        MultiIndex & mi;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_CREATE_INDEX_WRAPPER_HPP )