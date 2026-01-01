#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;

void test_castx() {
	printf("- castx...\n");

	int i=castx<uint32_t>(std::string("123"));
	assert(i==123);

	int j=castx<uint32_t, const char *>("123");
	assert(j==123);

	int k=castx<uint32_t>("123");
	assert(k==123);

	int l=castx<int>((int)1234);
	assert(l==1234);

	std::string s=castx<std::string>(123);
	assert(s=="123");

	std::string t=castx<std::string,std::string>(std::string("bla"));
	assert(t=="bla");
}
