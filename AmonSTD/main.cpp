#include <string>
#include <iostream>

#include "include/utils/AmonTuple.h"



int main()
{
	//amonTuple<int, double,std::string> t(2, 3.4,"bonk");
	//auto t = makeTuple(2, 3.4, "bonk");

	//std::cout <<"last element: " << get<2>(t);

	//std::cout << makeTuple(2, 3.4, "bonk") << "\n";

	auto t = makeTuple(2, 3.4, "bonk");

	auto t2 = pushFront(t, 22);

	std::cout << t2 ;
	return 0;
}