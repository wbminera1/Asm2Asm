#pragma once
#include <string>
#include <vector>
#include "Tokenizer.h"

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
		dst.clear();
		std::string token;
		const char* strPtr = GetToken(src.c_str(), token);
		if (token == "#") {
			strPtr = GetToken(strPtr, token);
			if (token.length() > 0) {
				Directive directive = GetDirective(token);
				if (directive != eNone) {
					if (!ProcessDirective(directive, strPtr)) {
						return false;
					}
				}
			}
		}
		else {
			if (m_DefineState.back()) {
				dst = src;
			}
		}
		return true;
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

	bool ProcessDirective(Directive directrive, const char* str)
	{
	//	printf("#%s %s", GetDirectiveDescription(directrive).m_Name.c_str(), str);
		switch (directrive)
		{
		case Preprocessor::eNone:
			return ProcessNotImplemented();
		case Preprocessor::eDefine:
			return ProcessNotImplemented();
		case Preprocessor::eUndef:
			return ProcessNotImplemented();
		case Preprocessor::eIf:
			return ProcessIf(str);
		case Preprocessor::eElif:
			return ProcessNotImplemented();
		case Preprocessor::eElse:
			return ProcessNotImplemented();
		case Preprocessor::eIfdef:
			return ProcessNotImplemented();
		case Preprocessor::eIfndef:
			return ProcessNotImplemented();
		case Preprocessor::eEndif:
			return ProcessEndIf();
		case Preprocessor::eInclude:
			return ProcessNotImplemented();
		case Preprocessor::eUsing:
			return ProcessNotImplemented();
		case Preprocessor::eImport:
			return ProcessNotImplemented();
		case Preprocessor::eError:
			return ProcessNotImplemented();
		case Preprocessor::eLine:
			return ProcessNotImplemented();
		case Preprocessor::ePragma:
			return ProcessNotImplemented();
		case Preprocessor::eLast:
			return ProcessNotImplemented();
		default:
			break;
		}
		return false;
	}

	struct Macro
	{
		std::string Name;
		int Value;
		Macro() : Value(0) {}
		Macro(const char* name, int value) : Name(name), Value(value) {}
	};

#include "Value.h"
#include "Operator.h"
#include "Expression.h"

	bool ProcessIf(const char* expression)
	{
		Expression expr;
		expression = expr.Evaluate(expression, m_Macro);
		if (expr.m_Value.IsValid()) {
			m_DefineState.push_back(expr.m_Value.m_BoolValue);
			return true;
		}
		return false;
	}

	bool ProcessEndIf()
	{
		if (m_DefineState.size() > 0) {
			m_DefineState.pop_back();
			return true;
		}
		return false;
	}

	bool ProcessNotImplemented()
	{
		return false;
	}


	std::vector<bool>  m_DefineState;
	std::vector<Macro> m_Macro;
	std::vector<Expression> m_Expressions;
};


