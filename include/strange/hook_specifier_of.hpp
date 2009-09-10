#if !defined( CHARMED_HOOK_SPECIFIER_OF_HPP )
#define CHARMED_HOOK_SPECIFIER_OF_HPP

#include <boost/multi_index/intrusive/fusion_hook.hpp>

namespace charmed
{
    template <typename T>
    struct hook_specifier_of
    {
        typedef boost::multi_index::intrusive::fusion_hook_specifier<T, typename T::hook_type, &T::hook> type;
    };
}

#endif // #if !defined( CHARMED_HOOK_SPECIFIER_OF_HPP )