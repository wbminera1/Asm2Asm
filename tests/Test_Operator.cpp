#include "../stdafx.h"
#include "../prepro/Preprocessor.h"
#include "../../Catch/single_include/catch.hpp"

TEST_CASE("PreprocessorOperatorSet", "[Operator]")
{
	SECTION("Set Op +")
	{
		bool passed = false;
		Preprocessor::Operator op;
		bool set = op.Parse("+");
		passed = set && (op.m_Type == op.eUnaryPlus);
		REQUIRE(passed);
	}
	SECTION("Set Op !")
	{
		bool passed = false;
		Preprocessor::Operator op;
		bool set = op.Parse("!");
		passed = set && (op.m_Type == op.eLogicalNOT);
		REQUIRE(passed);
	}
	SECTION("Set Op &&")
	{
		bool passed = false;
		Preprocessor::Operator op;
		bool set = op.Parse("&&");
		passed = set && (op.m_Type == op.eLogicalAND);
		REQUIRE(passed);
	}
	SECTION("Set Op ||")
	{
		bool passed = false;
		Preprocessor::Operator op;
		bool set = op.Parse("||");
		passed = set && (op.m_Type == op.eLogicalOR);
		REQUIRE(passed);
	}
}

TEST_CASE("PreprocessorOperatorEvaluate", "[Operator]")
{
	SECTION("Evaluate Op !")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eLogicalNOT);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value value(false);
		values.push_back(value);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == true);
		REQUIRE(passed);
	}
	SECTION("Evaluate Op && 1")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eLogicalAND);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value valueA(true);
		Preprocessor::Value valueB(true);
		values.push_back(valueA);
		values.push_back(valueB);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == true);
		REQUIRE(passed);
	}
	SECTION("Evaluate Op && 2")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eLogicalAND);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value valueA(false);
		Preprocessor::Value valueB(true);
		values.push_back(valueA);
		values.push_back(valueB);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == false);
		REQUIRE(passed);
	}
	SECTION("Evaluate Op || 1")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eLogicalOR);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value valueA(false);
		Preprocessor::Value valueB(true);
		values.push_back(valueA);
		values.push_back(valueB);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == true);
		REQUIRE(passed);
	}
	SECTION("Evaluate Op || 2")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eLogicalOR);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value valueA(false);
		Preprocessor::Value valueB(false);
		values.push_back(valueA);
		values.push_back(valueB);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == false);
		REQUIRE(passed);
	}
	SECTION("Evaluate Op == 1")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eEqualTo);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value valueA(1);
		Preprocessor::Value valueB(1);
		values.push_back(valueA);
		values.push_back(valueB);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == true);
		REQUIRE(passed);
	}

	SECTION("Evaluate Op == 2")
	{
		bool passed = false;
		Preprocessor::Operator op(Preprocessor::Operator::eEqualTo);
		std::vector<Preprocessor::Value> values;
		Preprocessor::Value valueA(1);
		Preprocessor::Value valueB(0);
		values.push_back(valueA);
		values.push_back(valueB);
		Preprocessor::Value res = op.Evaluate(values);
		passed = (res.m_Type == res.eBoolean) && (res.m_BoolValue == false);
		REQUIRE(passed);
	}
}
