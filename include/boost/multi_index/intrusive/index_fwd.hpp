#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP

#include <boost/multi_index/tag.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    // specifier forward decls
    // Arg1 -> !TagList
    // Arg2 -> KeyFromValue
    // Arg3 -> Compare
    template <typename Arg1, typename Arg2 = mpl::na, typename Arg3 = mpl::na>
    struct ordered_unique;

    template <typename Arg1, typename Arg2 = mpl::na, typename Arg3 = mpl::na>
    struct ordered_non_unique;

    // Arg1 -> !TagList
    // Arg2 -> KeyFromValue
    // Arg3 -> Hash
    // Arg4 -> Pred
    template <typename Arg1 = mpl::na, typename Arg2 = mpl::na, typename Arg3 = mpl::na, typename Arg4 = mpl::na>
    struct hashed_unique;

    template <typename Arg1 = mpl::na, typename Arg2 = mpl::na, typename Arg3 = mpl::na, typename Arg4 = mpl::na>
    struct hashed_non_unique;

    template <typename TagList = tag<> >
    struct sequenced;
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_INDEX_FWD_HPP )
