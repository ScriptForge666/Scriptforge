#pragma once
#include<CppUnitTest.h>
import Scriptforge.Log;
import std;
namespace sf = Scriptforge;

namespace ScriptforgeTest::LogTest {
	TEST_CLASS(LogTest) {
public:
	TEST_METHOD_INITIALIZE(tSetup);
	TEST_METHOD_CLEANUP(tCleanup);
	TEST_METHOD(LogWriteAndFilename);
private:
	/*
	std::string logFilePath = "test_log.log";
	sf::Logger logger{ logFilePath };
	*/
	};
}