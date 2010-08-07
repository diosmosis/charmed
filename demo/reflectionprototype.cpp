
#include <iostream>

struct bad_any_cast
	: std::runtime_error
{
	static std::string make_error_str(std::type_info const& actual, std::type_info const& requested)
	{
		std::stringstream ss;
		ss << "bad_any_cast: tried casting '" << actual.type_name() << "' to '" << requested.type_name() << "'";
		return ss.str();
	}

	bad_any_cast(std::type_info const& actual_, std::type_info const& requested_)
		: std::runtime_error(make_error_str(actual_, requested_))
		, actual(actual_)
		, requested(requested_)
	{}

	std::type_info const& actual;
	std::type_info const& requested;
};

struct bad_any_call
	: std::runtime_error
{
	static std::string make_error_str(std::type_info const& actual, std::type_info const& requested)
	{
		std::stringstream ss;
		ss << "bad_any_call: tried calling '" << actual.type_name() << "' as '" << requested.type_name() << "'";
		return ss.str();
	}

	bad_any_call(std::type_info const& actual_, std::type_info const& requested_)
		: std::runtime_error(make_error_str(actual_, requested_))
		, actual(actual_)
		, requested(requested_)
	{}

	std::type_info const& actual;
	std::type_info const& requested;
};

struct no_such_constructor
	: std::runtime_error
{
	static std::string make_error_str(type_info const& type, boost::mpl::identity<F> sig)
	{
		std::stringstream ss;
		ss << "no_such_constructor: type '" << type.name << "' has no constructor '" << typeid(F).type_name() << "'";
		return ss.str();
	}

	template <typename F>
	no_such_container(type_info const& type_, boost::mpl::identity<F> sig)
		: std::runtime_error(make_error_str(type, sig)
		, type(type_)
	{}

	type_info const* type;
};

struct any
{
    typedef boost::aligned_storage<sizeof(void *)> data_type;
    typedef data_type any::* unspecified_bool_t;

    any()
        : destruct(0)
        , type(0)
        , data()
    {}

    template <typename T>
    any(T const& x)
    {
        assign(x);
    }

    template <typename T>
    any(boost::reference_wrapper<T> x)
    {
        assign_ref(x.get());
    }

    any(any const& x)
    {
        assign(x);
    }

    any(boost::reference_wrapper<T> x)
    {
        assign_ref(x.get());
    }

    ~any()
    {
        if (destruct)
        {
            destruct(data.address());
        }
    }

    template <typename T>
    any & operator = (T const& x)
    {
        any(x).swap(*this);
        return *this;
    }

    template <typename T>
    any & operator = (boost::reference_wrapper<T> x)
    {
        any(x).swap(*this);
        return *this;
    }

    any & operator = (any const& x)
    {
        any(x).swap(*this);
        return *this;
    }

    any & operator = (boost::reference_wrapper<any> x)
    {
        any(x).swap(*this);
        return *this;
    }

    // TODO: Need swap overloads for any & callable_any.
    void swap(any & x)
    {
        using namespace std;

        swap(destruct, x.destruct);
        swap(type, x.type);
        swap(data, x.data);
    }

    template <typename T>
    T & cast()
    {
        check_type<T>();

        return cast_impl<T>();
    }

    template <typename T>
    T const& cast() const
    {
        check_type<T>();

        return cast_impl<T>();
    }

    operator unspecified_bool_type() const
    {
        return type ? &any::data : 0;
    }

private:
    template <typename T>
    typename boost::enable_if_c<sizeof(T) <= sizeof(data_type), T &>::type cast_impl()
    {
        return by_ref ? **static_cast<T **>(data.address()) : *static_cast<T *>(data.address());
    }

    template <typename T>
    typename boost::disable_if_c<sizeof(T) <= sizeof(data_type), T &>::type cast_impl()
    {
        return **static_cast<T **>(data.address());
    }

    template <typename T>
    void check_type() const
    {
        if (*type != typeid(T))
        {
            throw bad_any_cast(*type, typeid(T));
        }
    }

    template <typename T>
    typename boost::enable_if_c<sizeof(T) <= sizeof(data_type), void>::type assign(T const& x)
    {
        destruct = &destroy_no_dealloc<T>();
        type = &typeid(T);
        by_ref = 0;
        *static_cast<T *>(data.address()) = x;
    }

    template <typename T>
    typename boost::disable_if_c<sizeof(T) <= sizeof(data_type), void>::type assign(T const& x)
    {
        destruct = &destroy<T>;
        type = &typeid(T);
        by_ref = 0;
        *static_cast<T **>(data.address()) = new T(x);
    }

    template <typename T>
    void assign_ref(boost::reference_wrapper<T> x)
    {
        destruct = 0;
        type = &typeid(T);
        by_ref = 1;
        *static_cast<T *>(data.address()) = x.get_pointer();
    }

    template <typename T>
    static void destroy(void * p)
    {
        delete *static_cast<T **>(p);
    }

    template <typename T>
    static void destroy_no_dealloc(void * p)
    {
        static_cast<T *>(p)->~T();
    }

    // TODO: Can replace 3 of these w/ one pointer to a abstract base. It would point to static function data, but
    // this concrete data would have no members just definitions for pure virtual functions in the abstract base.
    // Would work, might be a bit more expensive in calculations. Can probably do the same w/ callable_any.
    void (*destruct)(void *);
    std::type_info const* type;
    unsigned int by_ref;
    data_type data;
};

namespace detail
{
    template <typename F>
    struct wrap_result;

    template <typename R>
    struct wrap_result_base
    {
        static any wrap(R & x)
        {
            return any(x);
        }
    };

    template <typename R>
    struct wrap_result_base<R &>
    {
        static any wrap(R & x)
        {
            return any(boost::ref(x));
        }
    };

    // TODO: use preprocessor iteration
    template <typename R>
    struct wrap_result<R ()>
        : wrap_result_base<R>
    {
        typedef R (*function_pointer_type)();

        static any call(void * f, void ** args)
        {
            return wrap(static_cast<function_pointer_type *>(f)());
        }
    };

    template <>
    struct wrap_result<void ()>
    {
        typedef void (*function_pointer_type)();

        static any call(void * f, void ** args)
        {
            static_cast<function_pointer_type *>(f)();
            return any();
        }
    };

    template <typename R, typename A0>
    struct wrap_result<R (A0)>
        : wrap_result_base<R>
    {
        typedef R (*function_pointer_type)(A0);
        typedef typename boost::remove_reference<A0>::type arg0;

        static any call(void * f, void ** args)
        {
            return wrap(
                static_cast<function_pointer_type *>(f)(
                    *static_cast<arg0 *>(args[0])
                )
            );
        }
    };

    template <typename A0>
    struct wrap_result<void (A0)>
    {
        typedef void (*function_pointer_type)(A0);
        typedef typename boost::remove_reference<A0>::type arg0;

        static any call(void * f, void ** args)
        {
            static_cast<function_pointer_type *>(f)(
                *static_cast<arg0 *>(args[0])
            );
            return any();
        }
    };

    template <typename R, typename A0, typename A1>
    struct wrap_result<R (A0, A1)>
        : wrap_result_base<R>
    {
        typedef R (*function_pointer_type)(A0, A1);
        typedef typename boost::remove_reference<A0>::type arg0;
        typedef typename boost::remove_reference<A1>::type arg1;

        static any call(void * f, void ** args)
        {
            return wrap(
                static_cast<function_pointer_type *>(f)(
                    *static_cast<arg0 *>(args[0]),
                    *static_cast<arg1 *>(args[1])
                )
            );
        }
    };

    template <typename A0, typename A1>
    struct wrap_result<void (A0, A1)>
        : wrap_result_base<R>
    {
        typedef void (*function_pointer_type)(A0, A1);
        typedef typename boost::remove_reference<A0>::type arg0;
        typedef typename boost::remove_reference<A1>::type arg1;

        static any call(void * f, void ** args)
        {
            static_cast<function_pointer_type *>(f)(
                *static_cast<arg0 *>(args[0]),
                *static_cast<arg1 *>(args[1])
            );
            return any();
        }
    };

	template <typename X, typename R>
	struct wrap_result<R (X::)()>
		: wrap_result_base<R>
	{
		typedef R (X::*function_pointer_type)();

		static any call(void * f, void ** args)
		{
			return wrap(
                (static_cast<X *>(args[0])->*static_cast<function_pointer_type *>(f))()
			);
		}
	};

	template <typename X>
	struct wrap_result<void (X::)()>
	{
		typedef void (X::*function_pointer_type)();

		static any call(void * f, void ** args)
		{
            (static_cast<X *>(args[0])->*static_cast<function_pointer_type *>(f))();
            return any();
		}
	};

	template <typename X, typename R, typename A0>
	struct wrap_result<R (X::)(A0)>
		: wrap_result_base<R>
	{
		typedef R (X::*function_pointer_type)(A0);
        typedef typename boost::remove_reference<A0>::type arg0;

		static any call(void * f, void ** args)
		{
			return wrap(
                (static_cast<X *>(args[0])->*static_cast<function_pointer_type *>(f))(
                    *static_cast<arg0 *>(args[1])
                )
			);
		}
	};

	template <typename X, typename A0>
	struct wrap_result<void (X::)(A0)>
	{
		typedef void (X::*function_pointer_type)(A0);
        typedef typename boost::remove_reference<A0>::type arg0;

		static any call(void * f, void ** args)
		{
            (static_cast<X *>(args[0])->*static_cast<function_pointer_type *>(f))(
                *static_cast<arg0 *>(args[1])
            );
            return any();
		}
	};

	template <typename X, typename R, typename A0, typename A1>
	struct wrap_result<R (X::)(A0, A1)>
		: wrap_result_base<R>
	{
		typedef R (X::*function_pointer_type)(A0, A1);
        typedef typename boost::remove_reference<A0>::type arg0;
        typedef typename boost::remove_reference<A1>::type arg1;

		static any call(void * f, void ** args)
		{
			return wrap(
                (static_cast<X *>(args[0])->*static_cast<function_pointer_type *>(f))(
                    *static_cast<arg0 *>(args[1]),
                    *static_cast<arg1 *>(args[2])
                )
			);
		}
	};

	template <typename X, typename A0, typename A1>
	struct wrap_result<void (X::)(A0, A1)>
	{
		typedef void (X::*function_pointer_type)(A0, A1);
        typedef typename boost::remove_reference<A0>::type arg0;
        typedef typename boost::remove_reference<A1>::type arg1;

		static any call(void * f, void ** args)
		{
            (static_cast<X *>(args[0])->*static_cast<function_pointer_type *>(f))(
                *static_cast<arg0 *>(args[1]),
                *static_cast<arg1 *>(args[2])
            );
            return any();
		}
	};

	template <typename F, typename True, typename False>
	struct signature_transform
		: boost::function_types::function_type<
			typename boost::mpl::push_front<
				any, // NOTE: Assumes 'any' is the return type to improve compile time.
				typename boost::mpl::transform<
					typename boost::function_types::parameter_types<F>::types,
					boost::mpl::if_<
						boost::is_reference<boost::mpl::_1>,
						True,
						False
					>
				>::type
			>::type
		>
	{};

	template <typename F>
	struct assign_reduce
		: signature_transform<
			F,
			boost::mpl::_1,
			boost::mpl::add_reference<
				boost::mpl::add_const<boost::mpl::_1>
			>
		>
	{};

	template <typename F>
	struct check_upgrade
		: signature_transform<
			F,
			boost::mpl::_1,
			boost::add_reference<boost::mpl::_1>
		>
	{};

	template <typename F>
	struct check_upgrade_total
		: signature_transform<
			F,
			boost::add_reference<
				boost::add_const<
					boost::remove_reference<boost::mpl::_1>
				>
			>,
			boost::mpl::_1
		>
	{};
}

// TODO: allow functors to be used w/ callable_any
struct callable_any
    : any
{
    typedef boost::aligned_storage<sizeof(void *) * 2> data_type;
    typedef data_type callable_any::* unspecified_bool_type;

    callable_any()
        : delegate_caller(0)
        , call_type(0)
        , function_type(0)
        , data()
    {}

    template <typename F>
    callable_any(F * f)
    {
        assign(f);
    }

    callable_any(callable_any const& x)
    {
        assign(x);
    }

    ~callable_any() {}

    callable_any & operator = (callable_any const& x)
    {
        callable_any(x).swap(*this);
        return *this;
    }

    template <typename F>
    callable_any & operator = (F * f)
    {
        callable_any(f).swap(*this);
        return *this;
    }

    void swap(callable_any & x)
    {
        using namespace std;

        swap(delegate_caller, x.delegate_caller);
        swap(call_type, x.call_type);
        swap(function_type, x.function_type);
        swap(data, x.data);
    }

    template <typename T>
    T * cast() const
    {
        // TODO: deal w/ equivalent function types. ie, int (X::*)(int) == int (X *, int, int)
        if (*function_type != typeid(T))
        {
            throw bad_any_cast(*function_type, typeid(T));
        }

        return *static_cast<T **>(data.address());
    }
    
    operator unspecified_bool_type() const
    {
        return delegate_caller ? &callable_any::data : 0;
    }

    any operator()() const
    {
        check_arg_compatibility<any ()>();

        return delegate_caller(data.address(), 0);
    }

    // TODO: use preprocessor iteration
    template <typename A0>
    any operator()(A0 & a0) const
    {
        check_arg_compatibility<any (A0)>();

        void * args[] = {&a0};
        return delegate_caller(data.address(), args);
    }

    template <typename A0, typename A1>
    any operator()(A0 & a0, A1 & a1) const
    {
        check_arg_compatibility<any (A0, A1)>();

        void * args[] = {&a0, &a1};
        return delegate_caller(data.address(), args);
    }

	template <typename F>
	void assign(F * f)
	{
		BOOST_STATIC_ASSERT(sizeof(F) == sizeof(data_type));

		delegate_caller = &detail::wrap_result<F>::call;
		call_type = &typeid(typename detail::assign_reduce<F>::type);
		function_type = &typeid(F);
		*static_cast<F *>(data.address()) = f;
	}

	// TODO: Need a local_callable_any that only checks pointer addresses instead of type_info instances.
	// this type would not be able to be used across assemblies.
	template <typename F>
	void check_arg_compatibility() const
	{
		typedef typename detail::check_upgrade<F>::type upgrade;

		// NOTE the check_upgrade_total metafunction needs the partially upgraded type, not F
		typedef typename detail::check_upgrade_total<upgrade>::type total_upgrade;

		if (!(typeid(upgrade) == *call_type || typeid(total_upgrade) == *call_type))
		{
			throw bad_any_call(*function_type, typeid(F));
		}
	}

	any (*delegate_caller)(void *, void **);
    std::type_info * call_type;
    std::type_info * function_type;
    data_type data;
};

template <typename T>
struct type_reflector;

struct member_function_info
{
    template <typename F>
    member_function_info(char const* name_, F * fptr_)
        : name(name_)
        , fptr(fptr_)
    {}

    char const* name;
    callable_any fptr;
};

namespace detail
{
	template <typename F, typename R>
	struct replace_return_type
		: boost::function_types::function_type<
			typename boost::mpl::push_front<
				R,
				typename boost::function_types::parameter_types<F>::type
			>::type
		>
	{};
}

struct constructor_info
    : member_function_info
{
    template <typename Signature>
    constructor_info(char const* name_, boost::mpl::identity<Signature>)
        : member_function_info(name_, &construct_function<Signature>::construct)
		, type_agnostic_sig(&typeid(typename detail::replace_return_type<Signature, any>::type))
    {}

	std::type_info * type_agnostic_sig;
	boost::intrusive::set_member_hook<> mapping_hook;
};

struct constructor_info_compare
{
	bool operator()(constructor_info const& lhs, constructor_info const& rhs) const
	{
		return lhs.type_agnostic_sig < rhs.type_agnostic_sig;
	}

	bool operator()(constructor_info const& lhs, std::type_info const* rhs) const
	{
		return lhs.type_agnostic_sig < rhs;
	}

	bool operator()(std::type_info const* lhs, constructor_info const& rhs) const
	{
		return lhs < rhs.type_agnostic_sig;
	}
};

struct member_info
{
    template <typename F>
    constructor_info(char const* name_, F * getter_)
        : name(name_)
        , getter(getter_)
    {}

    char const* name;
    callable_any getter;
};

struct type_info
{
    type_info(
        char const* name_
      , constructor_info const* ctr_f
      , constructor_info const* ctr_l
      , member_function_info const* mf_f
      , member_function_info const* mf_l
      , member_info const* m_f
      , member_info const* m_l
    ) : name(name_)
      , constructors(ctr_f, ctr_l)
      , member_functions(mf_f, mf_l)
      , members(m_f, m_l)
    {}

    char const* name;
    boost::iterator_range<constructor_info const*> constructors;
    boost::iterator_range<member_function_info const*> member_functions;
    boost::iterator_range<member_info const*> members;
};

struct c_str_compare
{
    typedef bool result_type;

    bool operator()(char const* lhs, char const* rhs) const
    {
        // TODO: I need a const_string.
        return strcmp(lhs, rhs) < 0;
    }
};

namespace charmed
{
    template <>
    struct index_specifier_of<type_info>
    {
        // TODO: Need tree-like indices. Specify indexed_by w/i an indexed_by to have tree-like container or
        // something like this.
        typedef boost::multi_index::indexed_by<
            boost::multi_index::intrusive::sequenced,
            boost::multi_index::intrusive::ordered_unique<
                boost::multi_index::member<type_info, char const*, &type_info::name>,
                c_str_compare
            >
        > type;
    };
}

struct parameter_info
{
    parameter_info(char const* name_)
        : name(name_)
    {}

    char const* name;
};

struct function_info
{
    template <typename F>
    function_info(char const* name_, F * fptr_, parameter_info const* p_f, parameter_info const* p_l)
        : name(name_)
        , fptr(fptr_)
        , parameters(p_f, p_l)
    {}

    char const* name;
    callable_any fptr;
    boost::iterator_range<parameter_info const*> parameters;
};

namespace charmed
{
    template <>
    struct index_specifier_of<function_info>
    {
        typedef boost::multi_index::indexed_by<
            boost::multi_index::intrusive::sequenced,
            boost::multi_index::intrusive::ordered_unique<
                boost::multi_index::member<function_info, char const*, &function_info::name>,
                c_str_compare
            >
        > type;
    };
}

template <unsigned int N>
struct parameter_storage
{
    enum { n = N };
    metadata<parameter_info> params;
};

template <unsigned int C, unsigned int MF, unsigned int MB>
struct whole_type_info
{
    enum { c = C, mf = MF, mb = MB };

    metadata<constructor_info> ctrs[C];
    metadata<member_function_info> memfuns[MF];
    metadata<member_info> mbrs[MB];
};

#define CHARMED_REFLECT_CTR_T(r, data, i, elem)                                                                       \
    BOOST_PP_COMMA_IF(i) boost::add_pointer<elem>::type

#define CHARMED_REFLECT_MBR_T(r, data, i, elem)                                                                       \
    BOOST_PP_COMMA_IF(i) BOOST_TYPEOF(&data::elem)

#define CHARMED_REFLECT_CTR_I(r, data, i, elem)                                                                       \
    BOOST_PP_COMMA_IF(i) &construct_function<elem>::construct

#define CHARMED_REFLECT_MBR_I(r, data, i, elem)                                                                       \
    BOOST_PP_COMMA_IF(i) &data::elem

#define CHARMED_REFLECT_CTR_A_I(r, data, i, elem)                                                                     \
    BOOST_PP_COMMA_IF(i) metadata<constructor_info>(                                                                  \
        &construct_function<Signature>::construct, constructor_info(#elem, boost::mpl::identity<elem>())

#define CHARMED_REFLECT_MBR_A_I(r, data, i, elem)                                                                     \
    BOOST_PP_COMMA_IF(i) metadata<member_info>(                                                                       \
        &data::elem, member_info(#elem, member<BOOST_TYPEOF(&data::elem)>::getter<&data::elem>))

#define CHARMED_REFLECT_MF_A_I(r, data, i, elem)                                                                      \
    BOOST_PP_COMMA_IF(i) metadata<member_function_info>(&data::elem, member_function_info(#elem, &data::elem))

#define CHARMED_REFLECT_MFND_T(r, data, i, elem)                                                                      \
    charmed::memfun_node<member_function_info, BOOST_TYPEOF(&data::elem)> memfun_init_ ## i;

#define CHARMED_REFLECT_PTMND_T(r, data, i, elem)                                                                     \
    charmed::pointer_to_member_node<member_info> mbr_init_ ## i;

#define CHARMED_REFLECT_MFND_T(r, data, i, elem)                                                                      \
    , memfun_init_ ## i(&data::elem,                                                                                  \
        charmed::member_function_association<member_function_info, BOOST_TYPEOF(&data::elem)>::get_mf_set(),          \
        other_attrs.memfuns[i])

#define CHARMED_REFLECT_PTMND_T(r, data, i, elem)                                                                     \
    , mbr_init_ ## i(&data::elem,                                                                                     \
        charmed::member_association<member_info, BOOST_TYPEOF(&data::elem)>::get_ptm_set(), other_attrs.mbrs[i])

#define CHARMED_REFLECT_CTR_INIT(r, data, i, elem)                                                                    \
    ref_metadata_initializer<constructor_info>(other_attrs.ctrs[i]);

#define CHARMED_REFLECT_TYPE(type, ctrs, memfuns, mbrs)                                                                  \
    template <>                                                                                                       \
    struct type_reflector<type> : boost::noncopyable                                                                  \
    {                                                                                                                 \
		typedef boost::intrusive::set<																				  \
			constructor_info,																						  \
			boost::intrusive::member_hook<																			  \
				constructor_info, boost::intrusive::set_member_hook<>, &constructor_info::mapping_hook				  \
			>,																										  \
			boost::intrusive::compare<constructor_info_compare>														  \
		> ctr_set_type;																							      \
																													  \
        /* for compile time reflection: */                                                                            \
        typedef boost::fusion::vector<                                                                                \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_CTR_T, _, ctrs)                                                   \
            BOOST_PP_COMMA_IF(BOOST_PP_SEQ_SIZE(ctrs))                                                                \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MBR_T, type, memfuns)                                             \
            BOOST_PP-COMMA_IF(BOOST_PP_SEQ_SIZE(memfuns))                                                             \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MBR_T, type, mbrs)                                                \
        > metadata_tuple_type;                                                                                        \
                                                                                                                      \
        static metadata_tuple_type make_metatuple()                                                                   \
        {                                                                                                             \
            return metadata_tuple_type(                                                                               \
                BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_CTR_I, _, ctrs)                                               \
                BOOST_PP_COMMA_IF(BOOST_PP_SEQ_SIZE(ctrs))                                                            \
                BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MBR_I, _, memfuns)                                            \
                BOOST_PP_COMMA_IF(BOOST_PP_SEQ_SIZE(memfuns))                                                         \
                BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MBR_I, type, mbrs)                                            \
            );                                                                                                        \
        }                                                                                                             \
                                                                                                                      \
        /* for run time reflection: */                                                                                \
        typedef whole_type_info<                                                                                      \
            BOOST_PP_SEQ_SIZE(ctrs), BOOST_PP_SEQ_SIZE(memfuns), BOOST_PP_SEQ_SIZE(mbrs)                              \
        > whole_type_info_type;                                                                                       \
                                                                                                                      \
        static whole_type_info_type make_whole_type_info_type()                                                       \
        {                                                                                                             \
            whole_type_info_type result = {                                                                           \
                { BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_CTR_A_I, _, ctrs) },                                        \
                { BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MF_A_I, type, memfuns) },                                   \
                { BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MBR_A_I, type, mbrs) }                                      \
            };                                                                                                        \
            return result;                                                                                            \
        }                                                                                                             \
                                                                                                                      \
        static type_info make_type_attribute(whole_type_info_type & ti)                                               \
        {                                                                                                             \
            return type_info(                                                                                         \
                #type,                                                                                                \
                ti.ctrs, ti.ctrs + whole_type_info_type::c,                                                           \
                ti.memfuns, ti.memfuns + whole_type_info_type::mf,                                                    \
                ti.mbrs, ti.mbrs + whole_type_info_type::mb                                                           \
            );                                                                                                        \
        }                                                                                                             \
                                                                                                                      \
        /* attributes: */                                                                                             \
        whole_type_info_type other_attrs;                                                                             \
        metadata<type_info> type_info_attr;                                                                           \
                                                                                                                      \
        /* initializers for attributes: */                                                                            \
        ref_metadata_initializer<type_info> type_info_init;                                                           \
        BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MFND_T, type, memfuns)                                                \
        BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_PTMND_T, type, mbrs)                                                  \
																													  \
		/* extra mapping data: */																					  \
		ctr_set_type ctrs;																							  \
                                                                                                                      \
        /* reflector constructor (creates attributes & executes initializers): */                                     \
        type_reflector()                                                                                              \
            : other_attrs(make_whole_type_info_type())                                                                \
            , type_info_attr(make_type_attribute(other_attrs))                                                        \
            , type_info_init(type_info_attr)                                                                          \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_MFND_I, type, memfuns)                                            \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_PTMND_I, type, mbrs)                                              \
        {                                                                                                             \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_CTR_INIT, _, ctrs)                                                \
																													  \
			for (constructor_info * c = other_attrs.ctrs; c != other_attrs.ctrs + whole_type_info_type::c; ++c)		  \
			{																										  \
				bool success = ctrs.insert(*c).second;																  \
				BOOST_ASSERT(success);																				  \
			}																										  \
        }                                                                                                             \
    }

// TOdo: deal w/ multiple dlls/assemblies
template <typename F>
struct function_reflector_outer;

template <typename R>
struct function_reflector_outer<R()>
{
    template <R (*F)()>
    struct inner;
};

template <typename R, typename A0>
struct function_reflector_outer<R(A0)>
{
    template <R (*F)(A0)>
    struct inner;
};

// TODO: Need to combine all of these compile time shennanigans into one set of types.
template <typename R, typename A0, typename A1>
struct function_reflector_outer<R(A0, A1)>
{
    template <R (*F)(A0, A1)>
    struct inner;
};

template <typename UniqueType>
struct function_reflector;

#define CHARMED_REFLECT_PARAM_I(r, data, i, elem)                                                                     \
    BOOST_PP_COMMA_IF(i) metadata<parameter_info>(#elem)

#define CHARMED_REFLECT_PARAM_INIT(r, data, i, elem)                                                                  \
    ref_metadata_initializer<parameter_info>(param_attrs.params[i]);

// TODO: deal w/ multiple dlls using the same reflection info.
#define CHARMED_REFLECT_FUNCTION(func, params)                                                                        \
    template <>                                                                                                       \
    struct function_reflector<                                                                                        \
        function_reflector_outer<BOOST_TYPEOF(&func)>::inner<&func>                                                   \
    >                                                                                                                 \
    {                                                                                                                 \
        typedef parameter_storage<BOOST_PP_SEQ_SIZE(params)> parameter_storage_type;                                  \
                                                                                                                      \
        static parameter_storage_type make_params()                                                                   \
        {                                                                                                             \
            parameter_storage_type result = {                                                                         \
                {BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_PARAM_I, _, params)}                                         \
            };                                                                                                        \
            return result;                                                                                            \
        }                                                                                                             \
                                                                                                                      \
        /* attributes: */                                                                                             \
        parameter_storage_type param_attrs;                                                                           \
        metadata<function_info> function_attr;                                                                        \
                                                                                                                      \
        /* function_reflector constructor: */                                                                         \
        function_reflector()                                                                                          \
            : param_attrs(make_params())                                                                              \
            , function_attr(#func, &func, param_attrs.params, param_attrs.params + parameter_storage_type::n)         \
        {                                                                                                             \
            BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_PARAM_INIT, _, params)                                            \
            ref_metadata_initializer<function_info>(function_attr);                                                   \
        }                                                                                                             \
    }

template <typename T, typename Enable = void>
struct info_attribute_of
{
    typedef type_info type;
};

template <typename T, typename C>
struct info_attribute_of<T C::*>
{
    typedef member_info type;
};

template <typename T>
struct info_attribute_of<
    T,
    typename boost::enable_if<
        boost::is_member_function_pointer<T>
    >::type
>
{
    typedef member_function_info type;
};

template <typename T>
struct info_attribute_of<
    T,
    typename boost::enable_if<
        is_function<typename boost::remove_pointer<T>::type>
    >::type
>
{
    typedef function_info type;
};

template <typename T>
inline char const* name_of(T const& x)
{
    typedef typename info_attribute_of<T>::type attribute_type;

    attribute_type const* a = query::metadata_of<attribute_type>(x);
    return a ? a->name : 0;
}

inline callable_any function_from_name(std::string const& x)
{
    function_info const* fi = query::metadata_of<function_info, 1>(x.c_str());
    return fi ? fi->fptr : callable_any();
}

inline std::type_info const* type_from_name(std::string const& x)
{
	type_info const* ti = query::metadata_of<type_info, 1>(x.c_str());
	return ti ? query::tagged_type_of<std::type_info>(*ti) : 0;
}

namespace result_of
{
	template <typename T>
	struct members_of
		: boost::fusion::result_of::as_vector<
			typename boost::fusion::result_of::filter_if<
				typename type_reflector<T>::metadata_tuple_type,
				boost::is_member_object_pointer<boost::mpl::_1>
			>::type
		>
	{};
}

template <typename T>
typename result_of::members_of<T>::type members_of()
{
	return boost::fusion::as_vector(
		fusion::filter_if<boost::is_member_object_pointer<boost::mpl::_1> >(
			type_reflector<T>::make_metatuple()
		)
	);
}

namespace detail
{
	template <typename F>
	inline F * find_constructor(std::string const& type_name)
	{
		type_info const* ti = query::metadata_of<type_info, 1>(type_name.c_str());

		// TODO: Allow option for assert.
		if (!ti)
		{
			throw no_such_type(type_name);
		}

		type_info::constructor_mapping_t::const_iterator i = ti->constructor_mapping.find(
			&typeid(F), constructor_info_compare());

		// TODO: assert option here too.
		if (i == ti->constructor_mapping.end())
		{
			throw no_such_constructor(*ti, boost::mpl::identity<F>());
		}

		return i->fptr.cast<F>();
	}
}

inline any make(std::string const& name)
{
	return detail::find_constructor<any ()>(name)();
}

template <typename A0>
inline any make(std::string const& name, A0 & a0)
{
	return detail::find_constructor<any (A0 &)>(name)(a0);
}

template <typename A0, typename A1>
inline any make(std::string const& name, A0 & a0, A1 & a1)
{
	return detail::find_constructor<any (A0 &, A1 &)>(name)(a0, a1);
}

template <typename A0, typename A1, typename A2>
inline any make(std::string const& name, A0 & a0, A1 & a1, A2 & a2)
{
	return detail::find_constructor<any (A0 &, A1 &, A2 &)>(name)(a0, a1, a2);
}

namespace result_of
{
	template <typename T>
	struct member_functions_of
		: boost::fusion::result_of:::as_vector<
			typename boost::fusion::result_of::filter_if<
				typename type_reflector<T>::metadata_tuple_type,
				boost::is_member_function_pointer<boost::mpl::_1>
			>::type
		>
	{};
}

template <typename T>
typename result_of::member_functions_of<T>::type member_functions_of()
{
	return boost::fusion::as_vector(
		fusion::filter_if<boost::is_member_function_pointer<boost::mpl::_1> >(
			type_reflector<T>::make_metatuple()
		)
	);
}

template <typename TypeReflectors, typename FunctionReflectors>
struct assembly_reflector
{
    static TypeReflectors type_reflectors;
    static FunctionReflectors function_reflectors;
};

template <typename TypeReflectors, typename FunctionReflectors>
TypeReflectors assembly_reflector<TypeReflectors, FunctionReflectors>::type_reflectors;

template <typename TypeReflectors, typename FunctionReflectors>
FunctionReflectors assembly_reflector<TypeReflectors, FunctionReflectors>::function_reflectors;

#define CHARMED_REFLECT_ASM_TYPE(r, data, i, elem)                                                                    \
    BOOST_PP_COMMA_IF(i) type_reflector<elem>

#define CHARMED_REFLECT_ASM_FN(r, data, i, elem)                                                                      \
    BOOST_PP_COMMA_IF(i) function_reflector<function_reflector_outer<BOOST_TYPEOF(&elem)>::inner<&elem> >

#define CHARMED_REFLECT_ASSEMBLY(types, functions)                                                                    \
    template assembly_reflector<                                                                                      \
        boost::fusion::vector<BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_ASM_TYPE, _, types)>,                           \
        boost::fusion::vector<BOOST_PP_SEQ_FOR_EACH_I(CHARMED_REFLECT_ASM_FN, _, functions)>                          \
    >

/////////////////////////

struct my_type
{
    my_type()
    {
        a = 1;
        b = 2;
        doodle = "123";
    }

    my_type(int a_, int b_, std::string const& d)
        : a(a_)
        , b(b_)
        , doodle(d)
    {}

    void my_func(int ab, int cd)
    {
    }

    int do_something()
    {
        return -1;
    }

    void print()
    {
        std::cout
            << "{\n"
            << "\ta: " << a << ",\n"
            << "\tb: " << b << ",\n'
            << "\tdoodle: " << doodle << "\n"
            << "}" << std::endl;
    }

    int a;
    int b;
    std::string doodle;
};

CHARMED_REFLECT_TYPE(
    my_type
    ,
    (my_type())
    (my_type(int, int, std::string))
    ,
    (my_func)
    (do_something)
    (print)
    ,
    (a)
    (b)
    (doodle)
);

struct print_name
{
    template <typename T>
    void operator()(T const& x) const
    {
        std::cout << name_of(x) << std::endl;
    }
};

void do_one_thing(int i, int j)
{
    std::cout << "do_one_thing(" << i << ", " << j << ")" << std::endl;
}

void do_another_thing(std::string v)
{
    std::cout << "do_another_thing('" << v << "')" << std::endl;
}

CHARMED_REFLECT_FUNCTION(do_one_thing, (i)(j));
CHARMED_REFLECT_FUNCTION(do_another_thing, (v));

CHARMED_REFLECT_ASSEMBLY(
    (my_type)
    ,
    (do_one_thing)
    (do_another_thing)
);

int main(int argc, char ** argv)
{
    // function from name
    std::cout << std::endl;
    {
        typedef void (*do_one_thing_t)(int, int);
        typedef void (*do_ano_thing_t)(std::string);

        do_one_thing_t f0 = function_from_name<do_one_thing_t>("do_one_thing");
        f0(12,24);

        do_another_thing_t f1 = function_from_name<do_another_thing_t>("do_another_thing");
        f1("dsfd");

        any_callable f0ac = function_from_name("do_one_thing");
        f0ac(50, 100);
        f0ac.cast<do_one_thing_t>()(55, 110);

        any_callable f1ac = function_from_name("do_another_thing");
        f1ac("here we go");
        f1ac.cast<do_another_thing_t>()("whatever now");
    }

    // iterate over members
    std::cout << std::endl;
    {
        std::cout << "members of my_type:" << std::endl;
        fusion::for_each(members_of<my_type>(), print_name());

        std::cout << std::endl << "member functions of my_type:" << std::endl;
        fusion::for_each(member_functions_of<my_type>(), print_name());
    }

    // get type_info from name
    std::cout << std::endl;
    {
        charmed::type const& t = type_from_name("my_type");
        std::type_info const& ti = t;

        std::cout << ti.type_name() << std::endl;
    }

    // create type from name
    std::cout << std::endl;
    {
        any v0 = make(type_from_name("my_type"));
        any_cast<my_type>(v0).print();

        any v1 = make(type_from_name("my_type"), 10, 20, "guerilla planting");
        any_cast<my_type>(v1).print();
    }

    // get stuff from assembly
    std::cout << std::endl;
    {
        assembly asm(argv[0]);

        std::cout << "assembly types: " << std::endl;
        std::for_each(asm.types(), print_name());

        std::cout << "\nassembly functions: " << std::endl;
        std::for_each(asm.functions(), print_name());
    }

    return EXIT_SUCCESS;
}
// TODO: test callable_any w/ result