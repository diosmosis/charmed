#if !defined( CHARMED_METADATA_RANGE_TYPE_HPP )
#define CHARMED_METADATA_RANGE_TYPE_HPP

#include <charmed/metadata_iterator.hpp>

#include <boost/range.hpp>

namespace charmed
{
    template <typename M>
    struct metadata_range
    {
        typedef boost::iterator_range<typename metadata_iterator<M>::type> type;
    };
}

#endif // #if !defined( CHARMED_METADATA_RANGE_TYPE_HPP )