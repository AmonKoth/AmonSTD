#include <string>
#include <iostream>

#include "include/utils/AmonTuple.h"



int main()
{
	amonTuple<int, double,std::string> t(2, 3.4,"bonk");

	std::cout <<"last element: " << get<2>(t);

	return 0;
}