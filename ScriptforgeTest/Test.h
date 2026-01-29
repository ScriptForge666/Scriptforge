#pragma once
#include<CppUnitTest.h>

namespace Test {
	TEST_CLASS(Test) {
public:
	TEST_METHOD_INITIALIZE(tSetup);
	TEST_METHOD_CLEANUP(tCleanup);
	TEST_METHOD(tTestSimple);
	};
}