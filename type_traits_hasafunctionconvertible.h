// Type traits (conversion to function)
template<typename Tto, typename... TRest>
struct HasAFunctionConvertible;

template<typename Tto>
struct HasAFunctionConvertible<Tto>
{
	static const bool value = false;
};

template<typename Tto, typename Tfrom, typename... TRest>
struct HasAFunctionConvertible<Tto, Tfrom, TRest...>
{
	static const bool value = std::is_convertible<Tfrom, Tto>::value || HasAFunctionConvertible<Tto, TRest...>::value;
};
