#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP

#include <boost/multi_index/intrusive/detail/hook_type_of.hpp>

#include <boost/mpl/transform.hpp>

#include <boost/fusion/include/as_vector.hpp>

namespace boost { namespace multi_index { namespace intrusive
{
    // TODO should derive from a fusion vector of hook's.  custom hook accessor may be needed.
    // will need to modify multi index container and index specifiers to specify specific hook type.
    template <typename IndexSpecifierList>
    struct composite_hook
        : fusion::result_of::as_vector<
            typename mpl::transform<
                IndexSpecifierList,
                detail::hook_type_of<mpl::_1>
            >::type
        >::type
    {};
}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_COMPOSITE_HOOK_HPP )