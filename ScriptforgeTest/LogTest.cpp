#include"LogTest.h"
import Scriptforge.Log;
import std;
namespace sf = Scriptforge;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace ScriptforgeTest::LogTest {
	void LogTest::tSetup() {
		
	}
	void LogTest::tCleanup() {}

	void LogTest::LogWriteAndFilename() {
		/*
		std::string testMessage = "Hello, Scriptforge Logger!";
			logger.log(testMessage);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::ifstream logFileStream(logFilePath);
		Assert::IsTrue(logFileStream.is_open(), L"日志文件打开失败");
		std::string loggedMessage;
		std::getline(logFileStream, loggedMessage);
		Assert::AreEqual(testMessage.c_str(), loggedMessage.c_str());
		*/
	}
}