#ifndef AMON_TUPLE_H
#define AMON_TUPLE_H

#include <iostream>

template<typename... Types>
struct amonTuple;

template<typename Head, typename... Rest>
struct amonTuple <Head, Rest...> 
{
private:
	Head head;
	amonTuple<Rest...> rest;
public:

	amonTuple() {}
	amonTuple(Head const& head, amonTuple<Rest...> const& rest) : head(head), rest(rest) {}
	
	template<typename VHead, typename... VRest,
			 typename = std::enable_if_t<sizeof...(VRest) == sizeof...(Rest)>>
	amonTuple(VHead&& vhead, VRest&&... vrest) : head(std::forward<VHead>(vhead)), rest(std::forward<VRest>(vrest)...) {}
	
	// to prevent incorrect conversion when constructing one tuple from another.
	template<typename VHead, typename... VRest,
			 typename = std::enable_if_t<sizeof...(VRest) == sizeof...(Rest)>>
	amonTuple(amonTuple<VHead, VRest...> const& other) : head(other.getHead()), rest(other.getRest()) {}

	Head& getHead() { return head; }
	Head const& getHead() const { return head; }
	amonTuple<Rest...>& getRest() { return rest; }
	amonTuple<Rest...> const& getRest() const { return rest; }

};

template<>
struct amonTuple<> {};

template<typename... Types>
auto makeTuple(Types&&... elems)
{
	return amonTuple<std::decay_t<Types>...>(std::forward<Types>(elems)...);
}

template<unsigned N>
struct tupleGet
{
	template<typename Head, typename... Rest>
	static auto apply(amonTuple<Head, Rest...> const& t)
	{
		return tupleGet<N - 1>::apply(t.getRest());
	}
};

template<>
struct tupleGet<0>
{
	template<typename Head, typename... Rest>
	static Head const& apply(amonTuple<Head, Rest...> const& t)
	{
		return t.getHead();
	}
};

template<unsigned N, typename... Types>
auto get(amonTuple<Types...> const& t)
{
	return tupleGet<N>::apply(t);
}

bool operator ==(amonTuple<> const&, amonTuple<> const&)
{
	return true;
}

template<typename Head1, typename... Rest1,
		 typename Head2, typename... Rest2,
		 typename = std::enable_if_t<sizeof...(Rest1) == sizeof... (Rest2)>>
bool operator ==(amonTuple<Head1, Rest1...> const& lhs,
					 amonTuple<Head2, Rest2...> const& rhs)
{
	return lhs.getHead() == rhs.getHead() && lhs.getRest() == rhs.getRest();
}

//to print the tuple using << operator

void printTuple(std::ostream& stream, amonTuple<> const&, bool isFirst = true)
{
	stream << (isFirst ? '(' : ')');
}

template<typename Head, typename... Rest>
void printTuple(std::ostream& stream, amonTuple<Head, Rest...> const& t, bool isFirst = true)
{
	stream << (isFirst ? "(" : ",");
	stream << t.getHead();
	printTuple(stream, t.getRest(), false);
}

template<typename... Types>
std::ostream& operator<<(std::ostream& stream, amonTuple<Types...> const& t)
{
	printTuple(stream, t);
	return stream;
}

//Pushfront
template<typename Tuple, typename NewType>
struct PushFrontT;

template<typename... Types, typename NewType>
struct PushFrontT<amonTuple<Types...>, NewType> {
	using Type = amonTuple<NewType, Types...>;
};

template<typename Tuple, typename NewType>
using PushFront = typename PushFrontT<Tuple, NewType>::Type;

template<typename... Types, typename Value>
PushFront<amonTuple<Types...>, Value>
pushFront(amonTuple<Types...> const& tuple, Value const& value)
{
	return PushFront<amonTuple<Types...>, Value>(value, tuple);
}

//Push Back
template<typename V>
amonTuple<V> pushBack(amonTuple<> const&, V const& value)
{
	return amonTuple<V>(value);
}

template<typename Head, typename... Rest, typename V>
amonTuple<Head, Rest..., V> pushBack(amonTuple<Head, Rest...> const& tuple, V const& value)
{
	return amonTuple<Head, Rest..., V>(tuple.getHead(), pushBack(tuple.getRest(), value));
}

//Pop front
template<typename Tuple>
struct PopFrontT;

template<typename Head, typename... Rest>
struct PopFrontT<amonTuple<Head, Rest...>>
{
	using Type = amonTuple<Rest...>;
};

template<typename Tuple>
using PopFront = typename PopFrontT<Tuple>::Type;

template<typename... Types>
PopFront<amonTuple<Types...>> 
popFront(amonTuple<Types...> const& tuple)
{
	return tuple.getRest();
}


#endif // !AMON_TUPLE_H
