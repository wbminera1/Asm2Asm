#pragma once
#include <string>
#include <vector>

class Preprocessor
{
public:
	enum Directive
	{
		eNone,
		eDefine,
		eUndef,
		eIf,
		eElif,
		eElse,
		eIfdef,
		eIfndef,
		eEndif,
		eInclude,
		eUsing,
		eImport,
		eError,
		eLine,
		ePragma,
		eLast
	};

	typedef bool (Preprocessor::*ProcessDirectiveFn)(const char* str);

	struct DirectiveDescription
	{
		Directive m_Directive;
		std::string m_Name;
		ProcessDirectiveFn m_Fn;

		static const DirectiveDescription& Get(size_t idx)
		{
			static DirectiveDescription Descriptions[eLast] = {
				{ eNone,   "" },
				{ eDefine, "define" },
				{ eUndef,  "undef" },
				{ eIf,     "if" },
				{ eElif,   "elif" },
				{ eElse,   "else" },
				{ eIfdef,  "ifdef" },
				{ eIfndef, "ifndef" },
				{ eEndif,  "endif" },
				{ eInclude,"include" },
				{ eUsing,  "using" },
				{ eImport, "import" },
				{ eError,  "error" },
				{ eLine,   "line" },
				{ ePragma, "pragma" }
			};
			if (idx <= eLast) {
				return Descriptions[idx];
			}
			return Descriptions[eNone];
		}
	};

	bool Process(const std::string& src, std::string& dst)
	{
		std::string token;
		const char* strPtr = GetToken(src.c_str(), token);
		if (token == "#") {
			strPtr = GetToken(strPtr, token);
			if (token.length() > 0) {
				Directive directive = GetDirective(token);
				if (directive != eNone) {
					ProcessDirective(directive, strPtr);
					return true;
				}
			}
		}
		dst = src;
		return false;
	}

	static bool isbracket(int chr)
	{
		return (chr == '(') || (chr == ')') || (chr == '{') || (chr == '}') || (chr == '[') || (chr == ']');
	}

	static bool ispunct_(int chr)
	{
		return ispunct(chr) && chr != '_';
	}

	static bool isalnum_(int chr)
	{
		return isalnum(chr) || chr == '_';
	}

	static const char* GetToken(const char* str, std::string& token)
	{
		token.clear();
		for (; *str != '\0'; ++str)
		{
			if (isspace(*str)) {
				if (token.size() > 0) {
					break;
				}
			}
			else if (isbracket(*str)) {
				if (token.size() == 0) {
					token.push_back(*str);
					++str;
				}
				break;
			}
			else if (isalnum_(*str)) {
				if (token.size() > 0) {
					if (!isalnum_(token[0])) {
						break;
					}
				}
				token.push_back(*str);
			}
			else if (ispunct_(*str)) {
				if (token.size() > 0) {
					if(!ispunct_(token[0])) { 
						break;
					}
				}
				token.push_back(*str);
			}
			else {
				token.push_back(*str);
			}
		}
		return str;
	}

	Directive GetDirective(const std::string& token)
	{
		if (token.length() > 0) {
			for (size_t i = 0; i < eLast; ++i) {
				const DirectiveDescription& description = DirectiveDescription::Get(i);
				if (token == description.m_Name) {
					return description.m_Directive;
				}
			}
		}
		return eNone;
	}

	void ProcessDirective(Directive directrive, const char* str)
	{
	//	printf("#%s %s", GetDirectiveDescription(directrive).m_Name.c_str(), str);
		switch (directrive)
		{
		case Preprocessor::eNone:
			break;
		case Preprocessor::eDefine:
			break;
		case Preprocessor::eUndef:
			break;
		case Preprocessor::eIf:
			break;
		case Preprocessor::eElif:
			break;
		case Preprocessor::eElse:
			break;
		case Preprocessor::eIfdef:
			break;
		case Preprocessor::eIfndef:
			break;
		case Preprocessor::eEndif:
			break;
		case Preprocessor::eInclude:
			break;
		case Preprocessor::eUsing:
			break;
		case Preprocessor::eImport:
			break;
		case Preprocessor::eError:
			break;
		case Preprocessor::eLine:
			break;
		case Preprocessor::ePragma:
			break;
		case Preprocessor::eLast:
			break;
		default:
			break;
		}
	}
/*
	void ProcessIf(const char* expression)
	{
		Expression expr;
		expr.Evaluate(expression);
//		if (expr.m_Value.)
		{
		}
	}
*/
	struct Value
	{
		enum Type {
			eNone,
			eBoolean,
			eNumeric,
			eString,
			eLast
		};

		Type		m_Type;
		bool		m_BoolValue;
		int			m_IntegerValue;
		std::string m_StringValue;

		Value() : m_Type(eNone)
			, m_BoolValue(false)
			, m_IntegerValue(0)
		{ }

		Value(bool val) { 
			Set(val);
		}

		Value(int val) { 
			Set(val);
		}

		void Invalidate() {
			m_Type = eNone;
			m_BoolValue = false;
			m_IntegerValue = 0;
			m_StringValue.clear();
		}

		bool IsValid() const {
			return (m_Type != eNone) && (m_Type < eLast);
		}

		void Set(bool val) {
			m_Type = eBoolean;
			m_BoolValue = val;
			m_IntegerValue = val ? 1 : 0;
			m_StringValue = std::to_string(m_IntegerValue);
		}

		void Set(int val) {
			m_Type = eNumeric;
			m_BoolValue = (val != 0);
			m_IntegerValue = val;
			m_StringValue = std::to_string(m_IntegerValue);
		}

		bool Parse(const char* str)
		{
			m_Type = eNone;
			for (size_t i = 0; str[i] != '\0'; ++i) {
				if (isspace(str[i])) {
					continue;
				}
				if (!isalnum(str[i]) && str[i] != '_') {
					m_Type = eNone;
					return false;
				}
				if (str[i] >= '0' && str[i] <= '9') {
					if (m_Type != eString) {
						m_Type = eNumeric;
					}
				}
				else {
					m_Type = eString;
				}
			}
			m_StringValue = str;
			if (m_Type == eNumeric) {
				m_IntegerValue = atoi(str);
			}
			return m_Type != eNone;
		}

		bool Cast(const Value& value)
		{
			if (m_Type == value.m_Type) {
				*this = value;
				return true;
			}
			if (m_Type == eNumeric) {
				if (value.m_Type == eBoolean) {
					m_IntegerValue = value.m_BoolValue;
					return true;
				}
			} else if (m_Type == eBoolean) {
				if (value.m_Type == eNumeric) {
					m_BoolValue = (value.m_IntegerValue != 0);
					return true;
				}
			}
			else if (m_Type == eString) {
				m_StringValue = value.m_StringValue;
				return true;
			}
			return false;
		}
	};

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

	struct Macro
	{
		std::string Name;
		int Value;
		Macro() : Value(0) {}
		Macro(const char* name, int value) : Name(name), Value(value) {}
	};

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
					int atLeft,atRight;
					if (GetOperandsIdx(opIdx, atLeft, atRight)) {
						Operator& op = m_Expressions[opIdx].m_Operator;
						if (op.m_Arity == -1) {
							if (atLeft == -1) {
								return false;
							}
							atRight = atLeft;
						} else if (op.m_Arity == 1) {
							if (atRight == -1) {
								return false;
							}
							atLeft = atRight;
						} else if (op.m_Arity == 2) {
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
						if(value.Parse(token.c_str())) {
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

	std::vector<Macro> m_Macro;
	std::vector<Expression> m_Expressions;
};


