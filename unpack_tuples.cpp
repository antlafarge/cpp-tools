#include <iostream>
#include <tuple>
#include <string>
#include <sstream>

void serialize(const std::stringstream&)
{
}

template<typename T, typename... Ts>
void serialize(std::stringstream& stream, const T& t, const Ts&... ts)
{
	stream << t;
	serialize(stream, ts...);
}

template<typename Tuple, int I>
Tuple& deserializeImpl(const std::stringstream&, Tuple& tuple)
{
	return tuple;
}

template<typename Tuple, int I, typename T, typename... Ts>
Tuple& deserializeImpl(std::stringstream& stream, Tuple& tuple)
{
	stream >> std::get<I>(tuple);
	return deserializeImpl<Tuple, I + 1, Ts...>(stream, tuple);
}

template<typename... Ts>
std::tuple<Ts...> deserialize(std::stringstream& stream)
{
	using Tuple = std::tuple<Ts...>;
	Tuple tuple;
	return deserializeImpl<Tuple, 0, Ts...>(stream, tuple);
}

void echo()
{
	std::cout << std::endl;
}

template<typename T, typename... Ts>
void echo(const T& t, const Ts&... ts)
{
	std::cout << t << ", ";
	echo(ts...);
}

template <class Callable, class Tuple, size_t... Is>
void call(Callable callable, Tuple t, std::index_sequence<Is...>)
{
	callable(std::get<Is>(t)...);
}

template<typename... Ts>
void echo(std::tuple<Ts...> tuple)
{
	//echo(std::get<Ts>(tuple)...);
	call(echo, );
}

int main()
{
	std::stringstream ss;
	serialize(ss, 'A', 'N', 'T', 'O', 'I', 'N', 'E');
	auto tuple = deserialize<char, char, char>(ss);
	echo(tuple);

	return 0;
}
