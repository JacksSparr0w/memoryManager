#include "pch.h"
#include "CppUnitTest.h"

#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include "..//ttest/mmemory.h"


namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			int err;
			err = _init(-1, 31);
			err = _init(1, 10);
			Assert::AreEqual(err, -1);
		}
	};
}
