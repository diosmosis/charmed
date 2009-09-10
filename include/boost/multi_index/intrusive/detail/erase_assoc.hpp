#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_ASSOC_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_ASSOC_HPP

#include <boost/multi_index/intrusive/detail/is_associative_container.hpp>

#include <boost/utility/enable_if.hpp>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    template <typename Value>
    struct erase_assoc
    {
        erase_assoc(Value & v)
            : value(v)
        {}

        template <typename Index>
        typename enable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index & x) const
        {
            x.impl().erase(x.impl().iterator_to(value));
        }

        template <typename Index>
        typename disable_if<
            is_associative_container<typename Index::impl_type>, void
        >::type operator()(Index &) const
        {
            // empty
        }

        Value & value;
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_ERASE_ASSOC_HPP )