#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_MAKE_POINTER_TUPLE_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_MAKE_POINTER_TUPLE_HPP

#include <boost/mpl/transform.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Tuple>
    struct make_pointer_tuple
        : mpl::transform<
            typename fusion::result_of::as_vector<Tuple>::type,
            add_pointer<mpl::_1>
        >
    {};
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_MAKE_POINTER_TUPLE_HPP )