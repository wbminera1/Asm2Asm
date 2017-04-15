#include "../stdafx.h"
#include "../prepro/Preprocessor.h"
#include "../../Catch/single_include/catch.hpp"

TEST_CASE("PreprocessorTest", "[Preprocessor]")
{
	SECTION("String 1")
	{
		bool passed = true;
		Preprocessor prepro;
		std::string src("#if (ASM) && (__x86_64__)");
		std::string dst;
		prepro.Process(src, dst);
		REQUIRE(passed);
	}
}
