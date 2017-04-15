#include "../stdafx.h"
#include "../prepro/Preprocessor.h"
#include "../../Catch/single_include/catch.hpp"

TEST_CASE("PreprocessorValue", "[Value]")
{
	SECTION("Set 0")
	{
		bool passed = false;
		Preprocessor::Value value;
		bool set = value.Parse("0");
		passed = set && (value.m_Type == value.eNumeric) && (value.m_IntegerValue == 0);
		REQUIRE(passed);
	}
	SECTION("Set 1")
	{
		bool passed = false;
		Preprocessor::Value value;
		bool set = value.Parse("1");
		passed = set && (value.m_Type == value.eNumeric) && (value.m_IntegerValue == 1);
		REQUIRE(passed);
	}
	SECTION("Set 1234567")
	{
		bool passed = false;
		Preprocessor::Value value;
		bool set = value.Parse("1234567");
		passed = set && (value.m_Type == value.eNumeric) && (value.m_IntegerValue == 1234567);
		REQUIRE(passed);
	}
	SECTION("Set abcdef")
	{
		bool passed = false;
		Preprocessor::Value value;
		bool set = value.Parse("abcdef");
		passed = set && (value.m_Type == value.eString) && (value.m_StringValue == "abcdef");
		REQUIRE(passed);
	}
	SECTION("Set _abc_def_")
	{
		bool passed = false;
		Preprocessor::Value value;
		bool set = value.Parse("_abc_def_");
		passed = set && (value.m_Type == value.eString) && (value.m_StringValue == "_abc_def_");
		REQUIRE(passed);
	}
	SECTION("Set +")
	{
		bool passed = false;
		Preprocessor::Value value;
		bool set = value.Parse("+");
		passed = !set && (value.m_Type == value.eNone);
		REQUIRE(passed);
	}
}
