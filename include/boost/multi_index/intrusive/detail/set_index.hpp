#if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SET_INDEX_HPP )
#define BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SET_INDEX_HPP

#include <boost/multi_index/intrusive/detail/common_index.hpp>
#include <boost/multi_index/intrusive/detail/key_from_value_composite.hpp>

#include <iterator>

namespace boost { namespace multi_index { namespace intrusive { namespace detail
{
    // TODO need rollback modify_key?
    template <typename MultiIndex, typename KeyFromValue, typename Impl, typename Specifier>
    struct set_index : common_index<MultiIndex, Impl, Specifier>
    {
        typedef common_index<MultiIndex, Impl, Specifier> base_type;

        typedef typename KeyFromValue::result_type key_type;

        set_index(multi_index_type & x, Impl & i)
            : base_type(x, i)
        {}

        template <typename CompatibleKey>
        size_type count(CompatibleKey const& x) const
        {
            return impl().count(x);
        }

        template <typename CompatibleKey>
        std::pair<iterator, iterator> equal_range(CompatibleKey const& x) const
        {
            return impl().equal_range(x);
        }

        template <typename Modifier>
        bool modify_key(iterator position, Modifier m)
        {
            return detail::modify(container, position, key_from_value_composite<KeyFromValue, Modifier>(m));
        }

        std::pair<iterator, bool> insert(value_type & x)
        {
            return detail::insert_associative(container, *this, x);
        }

        iterator insert(iterator position, value_type & x)
        {
            return detail::insert_associative(container, *this, position, x);
        }

        size_type erase(key_type const& x)
        {
            // get all items w/ the key 'x'
            std::pair<iterator, iterator> rng = impl().equal_range(x);

            // get the count of all items w/ the key 'x'
            size_type result = std::distance(rng.first, rng.second);

            // erase the items found
            erase(rng.first, rng.second);

            // return the count of items erased
            return result;
        }

        iterator erase(iterator position)
        {
            return common_index::erase(position);
        }

        iterator erase(iterator first, iterator last)
        {
            return common_index::erase(first, last);
        }
    };
}}}}

#endif // #if !defined( BOOST_MULTI_INDEX_INTRUSIVE_DETAIL_SET_INDEX_HPP )