#if !defined( CHARMED_METADATA_ITERATOR_HPP )
#define CHARMED_METADATA_ITERATOR_HPP

#include <charmed/metadata_storage.hpp>

namespace charmed
{
    template <typename M>
    struct metadata_iterator
    {
        typedef typename metadata_storage<M>::container_type::const_iterator type;
    };
}

#endif // #if !defined( CHARMED_METADATA_ITERATOR_HPP )