#include"Test.h"
namespace Test{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;
	void Test::tSetup() {}
	void Test::tCleanup() {}
	void Test::tTestSimple() {
		Assert::AreEqual(1, 1);
	}
}