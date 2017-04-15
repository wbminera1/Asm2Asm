#include "../stdafx.h"
#include "../prepro/Preprocessor.h"
#include "../../Catch/single_include/catch.hpp"

TEST_CASE("PreprocessorExpression", "[Expression]")
{
	SECTION("String 1")
	{
		bool passed = true;
		Preprocessor::Expression expr;
		std::vector<Preprocessor::Macro> defines;
		defines.push_back(Preprocessor::Macro("ASM", 1));
		defines.push_back(Preprocessor::Macro("__x86_64__", 1));
		passed = (expr.Evaluate(" (ASM) && (__x86_64__)", defines) != nullptr);
		passed &= expr.m_Value.IsValid();
		passed &= (expr.m_Value.m_Type == Preprocessor::Value::eBoolean);
		passed &= (expr.m_Value.m_BoolValue == true);
		REQUIRE(passed);
	}
	SECTION("String 2")
	{
		bool passed = true;
		Preprocessor::Expression expr;
		std::vector<Preprocessor::Macro> defines;
		defines.push_back(Preprocessor::Macro("ASM", 1));
		defines.push_back(Preprocessor::Macro("__x86_64__", 1));
		passed = (expr.Evaluate(" (ASM) || (__x86_64__)", defines) != nullptr);
		passed &= expr.m_Value.IsValid();
		passed &= (expr.m_Value.m_Type == Preprocessor::Value::eBoolean);
		passed &= (expr.m_Value.m_BoolValue == true);
		REQUIRE(passed);
	}
	SECTION("String 3")
	{
		bool passed = true;
		Preprocessor::Expression expr;
		std::vector<Preprocessor::Macro> defines;
		defines.push_back(Preprocessor::Macro("ASM", 1));
		defines.push_back(Preprocessor::Macro("__x86_64__", 1));
		passed = (expr.Evaluate("!ASM", defines) != nullptr);
		passed &= expr.m_Value.IsValid();
		passed &= (expr.m_Value.m_Type == Preprocessor::Value::eBoolean);
		passed &= (expr.m_Value.m_BoolValue == false);
		REQUIRE(passed);
	}
}
