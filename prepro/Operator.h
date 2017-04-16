#pragma once

	struct Operator
	{
		enum Type
		{
			eNone,
			// Arithmetic operators
			eBasicAssignment,			// a = b
			eAddition,					// a + b
			eSubtraction,				// a - b
			eUnaryPlus,					// + a
			eUnaryMinus,				// - a
			eMultiplication,			// a * b
			eDivision,					// a / b
			eModulo,					// a % b
			eIncrementPrefix,			// ++a
			eIncrementPostfix,			// a++
			eDecrementPrefix,			// --a
			eDecrementPostfix,			// a--
			// Comparison operators/relational operators[edit]
			eEqualTo,					// 	a == b
			eNotEqualTto,				// 	a != b
			eGreaterThan,				// 	a > b
			eLessThan,					// 	a < b
			eGreaterThanOrEqualTo,		// 	a >= b
			eLessThanOrEqualTo,			// 	a <= b
			// Logical operators
			eLogicalNOT,				//	!a
			eLogicalAND,				//	a && b
			eLogicalOR,					//	a || b
			// Bitwise operators
			eBitwiseNOT,				//	~a
			eBitwiseAND,				//	a & b
			eBitwiseOR,					//	a | b
			eBitwiseXOR,				//	a ^ b
			eBitwiseLeftShift,			//	a << b
			eBitwiseRightShift,			//	a >> b
			// Compound assignment operators
			eAdditionAssignment,		//	a += b
			eSubtractionAssignment,		//	a -= b
			eMultiplicationAssignment,	//	a *= b
			eDivisionAssignment,		//	a /= b
			eModuloAssignment,			//	a %= b
			eBitwiseANDAssignment,		//	a &= b
			eBitwiseORAssignment,		//	a |= b
			eBitwiseXORAssignment,		//	a ^= b
			eBitwiseLeftShiftAssignment,//	a <<= b
			eBitwiseRightShiftAssignment,//a >>= b
			//
			eLast
		};

		Type m_Type;
		int m_Arity;
		int m_Precedence;
		std::string m_Name;

		static const Operator& Get(size_t idx)
		{
			static Operator Descriptions[eLast] = {
				{ eNone,				0, 9, "" },
				// Arithmetic operators
				{ eBasicAssignment,		2, 16, "=" },		// a = b
				{ eUnaryPlus,			1, 3, "+" },		// + a
				{ eAddition,			2, 6, "+" },		// a + b
				{ eUnaryMinus,			1, 3, "-" },		// - a
				{ eSubtraction,			2, 6, "-" },		// a - b
				{ eMultiplication,		2, 5, "*" },		// a * b
				{ eDivision,			2, 5, "/" },		// a / b
				{ eModulo,				2, 5, "%" },		// a % b
				{ eIncrementPostfix,	-1, 2, "++" },		// a++
				{ eIncrementPrefix,		1, 3, "++" },		// ++a
				{ eDecrementPostfix,	-1, 2, "--" },		// a--
				{ eDecrementPrefix,		1, 3, "--" },		// --a
				// Comparison operators/relational operators[edit]
				{ eEqualTo,				2, 9, "==" },		// a == b
				{ eNotEqualTto,			2, 9, "!=" },		// a != b
				{ eGreaterThan,			2, 8, ">" },		// a > b
				{ eLessThan,			2, 8, "<" },		// a < b
				{ eGreaterThanOrEqualTo, 2, 8, ">=" },		// a >= b
				{ eLessThanOrEqualTo,	2, 8, "<=" },		// a <= b
				// Logical operators
				{ eLogicalNOT,			1, 3, "!" },		// !a
				{ eLogicalAND,			2, 13, "&&" },		// a && b
				{ eLogicalOR,			2, 14, "||" },		// a || b
				// Bitwise operators
				{ eBitwiseNOT,			1, 3, "~" },		// ~a
				{ eBitwiseAND,			2, 10, "&" },		// a & b
				{ eBitwiseOR,			2, 12, "|" },		// a | b
				{ eBitwiseXOR,			2, 11, "^" },		// a ^ b
				{ eBitwiseLeftShift,	2, 7, "<<" },		// a << b
				{ eBitwiseRightShift,	2, 7, ">>" },		// a >> b
				// Compound assignment operators
				{ eAdditionAssignment,			2, 16, "+=" },	// a += b
				{ eSubtractionAssignment,		2, 16, "-=" },	// a -= b
				{ eMultiplicationAssignment,	2, 16, "*=" },	// a *= b
				{ eDivisionAssignment,			2, 16, "/=" },	// a /= b
				{ eModuloAssignment,			2, 16, "%=" },	// a %= b
				{ eBitwiseANDAssignment,		2, 16, "&=" },	// a &= b
				{ eBitwiseORAssignment,			2, 16, "|=" },	// a |= b
				{ eBitwiseXORAssignment,		2, 16, "^=" },	// a ^= b
				{ eBitwiseLeftShiftAssignment,	2, 16, "<<=" },	// a <<= b
				{ eBitwiseRightShiftAssignment, 2, 16, ">>=" }	// a >>= b
			};
			if (idx <= eLast) {
				return Descriptions[idx];
			}
			return Descriptions[eNone];
		}

		Operator() 
			: m_Type(eNone)
			, m_Arity(0)
			, m_Precedence(0)
		{ }

		Operator(Type type)
			: m_Type(type)
		{ 
			const Operator& op = Get(type);
			m_Arity = op.m_Arity;
			m_Precedence = op.m_Precedence;
			m_Name = op.m_Name;
		}

		Operator(Type type, int arity, int precedence, const char* name)
			: m_Type(type)
			, m_Arity(arity)
			, m_Precedence(precedence)
			, m_Name(name)
		{ }

		void Invalidate()
		{
			m_Type = eNone;
			m_Arity = 0;
			m_Precedence = 0;
		}

		bool IsValid() const {
			return (m_Type != eNone) && (m_Type < eLast);
		}

		bool Parse(const char* str)
		{
			m_Type = eNone;
			for (size_t i = 0; i < eLast; ++i) {
				const Operator& op = Get(i);
				if (op.m_Name == str) {
					*this = op;
					break;
				}
			}
			return m_Type != eNone;
		}

		Value Evaluate(Value& left, Value& right) const
		{
			Value result;
			if (m_Arity == 1) {
				if (m_Type >= eLogicalNOT && m_Type <= eLogicalOR) {
					result = EvaluateLogical(right);
					right.Invalidate();
				}
			} else if (m_Arity == 2) {
				if (m_Type >= eLogicalNOT && m_Type <= eLogicalOR) {
					result = EvaluateLogical(left, right);
					left.Invalidate();
					right.Invalidate();
				}
			}
			return result;
		}

		Value Evaluate(std::vector<Value>& expr) const
		{
			if (expr.size() >= m_Arity) {
				return Evaluate(expr[0], expr.size() > 1 ? expr[1] : expr[0]);
			}
			return Value();
		}

		Value EvaluateLogical(const Value& value) const
		{
			Value dst(false);
			if (m_Type == eLogicalNOT) {
				dst.Set(!value.m_BoolValue);
			}
			return dst;
		}

		Value EvaluateLogical(const Value& valueA, const Value& valueB) const
		{
			Value dst(false);
			if (m_Type == eLogicalAND) {
				dst.Set(valueA.m_BoolValue && valueB.m_BoolValue);
			}
			else if (m_Type == eLogicalOR) {
				dst.Set(valueA.m_BoolValue || valueB.m_BoolValue);
			}
			return dst;
		}

	};


