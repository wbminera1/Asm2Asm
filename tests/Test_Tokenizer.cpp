#include "../stdafx.h"
#include "../prepro/Preprocessor.h"
#include "../../Catch/single_include/catch.hpp"

TEST_CASE("PreprocessorTokenizer", "[Tokenizer]")
{
	SECTION("Token1")
	{
		bool passed = true;
		const char* sample1 = "#if (ASM) && (__x86_64__)";
		std::string token;
		Preprocessor prepro;
		sample1 = GetToken(sample1, token); passed &= (token == "#");
		sample1 = GetToken(sample1, token); passed &= (token == "if");
		sample1 = GetToken(sample1, token); passed &= (token == "(");
		sample1 = GetToken(sample1, token); passed &= (token == "ASM");
		sample1 = GetToken(sample1, token); passed &= (token == ")");
		sample1 = GetToken(sample1, token); passed &= (token == "&&");
		sample1 = GetToken(sample1, token); passed &= (token == "(");
		sample1 = GetToken(sample1, token); passed &= (token == "__x86_64__");
		sample1 = GetToken(sample1, token); passed &= (token == ")");
		REQUIRE(passed);
	}
}
