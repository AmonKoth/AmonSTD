#ifndef AMON_TUPLE_H
#define AMON_TUPLE_H

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




#endif // !AMON_TUPLE_H
