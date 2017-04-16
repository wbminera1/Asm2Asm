#include "../stdafx.h"
#include "../prepro/Preprocessor.h"
#include "../../Catch/single_include/catch.hpp"

TEST_CASE("PreprocessorTest", "[Preprocessor]")
{
	SECTION("String 1")
	{
		bool passed = true;
		Preprocessor::Processor prepro;
		std::string src1("#if (ASM) && (__x86_64__)");
		std::string src2("hello");
		std::string src3("#endif");
		std::string dst;
		prepro.m_Macro.push_back(Preprocessor::Macro("ASM", 1));
		prepro.m_Macro.push_back(Preprocessor::Macro("__x86_64__", 1));
		passed &= prepro.Process(src1, dst); passed &= (dst == "");
		passed &= prepro.Process(src2, dst); passed &= (dst == src2);
		passed &= prepro.Process(src3, dst); passed &= (dst == "");
		REQUIRE(passed);
	}
	SECTION("String 2")
	{
		bool passed = true;
		Preprocessor::Processor prepro;
		std::string src1("#if (ASM) && (__x86_64__)");
		std::string src2("hello");
		std::string src3("#endif");
		std::string dst;
		prepro.m_Macro.push_back(Preprocessor::Macro("ASM", 1));
		//prepro.m_Macro.push_back(Preprocessor::Macro("__x86_64__", 1));
		passed &= prepro.Process(src1, dst); passed &= (dst == "");
		passed &= prepro.Process(src2, dst); passed &= (dst == "");
		passed &= prepro.Process(src3, dst); passed &= (dst == "");
		REQUIRE(passed);
	}
}
