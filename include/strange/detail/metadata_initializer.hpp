#if !defined( CHARMED_DETAIL_METADATA_INITIALIZER_HPP )
#define CHARMED_DETAIL_METADATA_INITIALIZER_HPP

#include <charmed/metadata_storage.hpp>

namespace charmed { namespace detail
{
    // needed so derived class' values get initialized before put into the non-intrusive multi-index
    template <typename D>
    struct metadata_initializer
    {
        template <typename T>
        metadata_initializer(T const& x, D const& data_)
            : data(x, data_)
        {
            insert(metadata_storage_access<D>::mutable_metadata().get<0>());
        }

        template <typename M, typename I, typename S>
        void insert(boost::multi_index::intrusive::sequenced_index<M, I, S> & idx)
        {
            idx.push_back(data);
        }

        template <typename I>
        void insert(I & idx)
        {
            idx.insert(data);
        }

        metadata<D> data;
    };
}}

#endif //#if !defined( CHARMED_DETAIL_METADATA_INITIALIZER_HPP )
