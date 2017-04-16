#pragma once
#include "Value.h"
#include "Operator.h"
#include "Macro.h"

namespace Preprocessor {

	struct Expression
	{
		struct ExpressionPair
		{
			Value m_Value;
			Operator m_Operator;

			ExpressionPair(const Value& value) : m_Value(value) {}
			ExpressionPair(const Operator& op) : m_Operator(op) {}
		};

		Value m_Value;
		std::vector<ExpressionPair> m_Expressions;

		void Substitute(const std::vector<Macro>& defines)
		{
			for (size_t v = 0; v < m_Expressions.size(); ++v) {
				if (m_Expressions[v].m_Value.m_Type == Value::eString) {
					for (size_t m = 0; m < defines.size(); ++m) {
						if (m_Expressions[v].m_Value.m_StringValue == defines[m].Name) {
							m_Expressions[v].m_Value.Set(defines[m].Value);
							break;
						}
					}
				}
			}
		}

		int GetOperatorIdxByPrecedence() const
		{
			int bestIdx = -1;
			int bestPrecedence = INT_MAX;
			for (int i = 0; i < m_Expressions.size(); ++i) {
				if (m_Expressions[i].m_Operator.m_Type != Operator::eNone) {
					if (m_Expressions[i].m_Operator.m_Precedence < bestPrecedence) {
						bestPrecedence = m_Expressions[i].m_Operator.m_Precedence;
						bestIdx = i;
					}
				}
			}
			return bestIdx;
		}

		bool GetOperandsIdx(int idx, int& atLeft, int& atRight)
		{
			atLeft = atRight = -1;
			if (idx >= 0 && idx < m_Expressions.size()) {
				idx -= 1;
				if (idx >= 0 && m_Expressions[idx].m_Value.IsValid()) {
					atLeft = idx;
				}
				idx += 2;
				if (idx < m_Expressions.size() && m_Expressions[idx].m_Value.IsValid()) {
					atRight = idx;
				}
				return true;
			}
			return false;
		}

		void Clear(int fromIdx, int amount)
		{
			if (fromIdx < 0) {
				fromIdx = 0;
			}
			for (int i = 0; i < amount; ++i) {
				if (fromIdx >= m_Expressions.size()) {
					break;
				}
				if ((!m_Expressions[fromIdx].m_Value.IsValid()) && (!m_Expressions[fromIdx].m_Operator.IsValid())) {
					m_Expressions.erase(m_Expressions.begin() + fromIdx);
				}
				else {
					++fromIdx;
				}
			}
		}

		bool Evaluate()
		{
			while (m_Expressions.size() > 1) {
				int opIdx = GetOperatorIdxByPrecedence();
				if (opIdx != -1) {
					int atLeft, atRight;
					if (GetOperandsIdx(opIdx, atLeft, atRight)) {
						Operator& op = m_Expressions[opIdx].m_Operator;
						if (op.m_Arity == -1) {
							if (atLeft == -1) {
								return false;
							}
							atRight = atLeft;
						}
						else if (op.m_Arity == 1) {
							if (atRight == -1) {
								return false;
							}
							atLeft = atRight;
						}
						else if (op.m_Arity == 2) {
							if (atLeft == -1 || atRight == -1) {
								return false;
							}
						}
						m_Expressions[opIdx].m_Value = op.Evaluate(m_Expressions[atLeft].m_Value, m_Expressions[atRight].m_Value);
						if (!m_Expressions[opIdx].m_Value.IsValid()) {
							return false;
						}
						m_Expressions[opIdx].m_Operator.Invalidate();
						Clear(opIdx - 1, 3);
					}
				}
			}
			if (m_Expressions.size() > 0 && m_Expressions[0].m_Value.IsValid())
			{
				m_Value = m_Expressions[0].m_Value;
				return true;
			}
			return false;
		}

		const char* Evaluate(const char* expressionStr, const std::vector<Macro>& defines)
		{
			std::string token;
			do {
				expressionStr = GetToken(expressionStr, token);
				if (token.length() == 0) {
					break;
				}
				else if (token == ")") {
					--expressionStr;
					break;
				}
				else if (token == "(") {
					Expression expr;
					expressionStr = expr.Evaluate(expressionStr, defines);
					expressionStr = GetToken(expressionStr, token);
					if (token != ")" || expressionStr == nullptr) {
						return expressionStr;
					}
					m_Expressions.push_back(expr.m_Value);
				}
				else {
					Operator op;
					if (op.Parse(token.c_str())) {
						m_Expressions.push_back(op);
					}
					else {
						Value value;
						if (value.Parse(token.c_str())) {
							m_Expressions.push_back(value);
						}
						else {
							printf("Error parsing: %s\n", token.c_str());
							return nullptr;
						}
					}
				}
			} while (token.length() > 0);
			Substitute(defines);
			if (!Evaluate()) {
				return nullptr;
			}
			return expressionStr;
		}
	};

}  // namespace Preprocessor
