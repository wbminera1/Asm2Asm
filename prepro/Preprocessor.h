#pragma once
#include <string>
#include <vector>
#include "Tokenizer.h"
#include "Value.h"
#include "Operator.h"
#include "Expression.h"
#include "Macro.h"

namespace Preprocessor {

	class Processor
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

		typedef bool (Processor::*ProcessDirectiveFn)(const char* str);

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
				if (GetDefineState()) {
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
			case eNone:
				return ProcessNotImplemented();
			case eDefine:
				return ProcessNotImplemented();
			case eUndef:
				return ProcessNotImplemented();
			case eIf:
				return ProcessIf(str);
			case eElif:
				return ProcessNotImplemented();
			case eElse:
				return ProcessElse();
			case eIfdef:
				return ProcessNotImplemented();
			case eIfndef:
				return ProcessNotImplemented();
			case eEndif:
				return ProcessEndIf();
			case eInclude:
				return ProcessNotImplemented();
			case eUsing:
				return ProcessNotImplemented();
			case eImport:
				return ProcessNotImplemented();
			case eError:
				return ProcessNotImplemented();
			case eLine:
				return ProcessNotImplemented();
			case ePragma:
				return ProcessNotImplemented();
			case eLast:
				return ProcessNotImplemented();
			default:
				break;
			}
			return false;
		}

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

		bool ProcessElse()
		{
			if (m_DefineState.size() > 0) {
				m_DefineState[m_DefineState.size() - 1] = !m_DefineState[m_DefineState.size() - 1];
				return true;
			}
			return false;
		}

		bool ProcessNotImplemented()
		{
			return false;
		}

		bool GetDefineState() const {
			for (size_t i = m_DefineState.size(); i > 0; --i) {
				if (!m_DefineState[i - 1]) {
					return false;
				}
			}
			return true;
		}

		std::vector<bool>  m_DefineState;
		std::vector<Macro> m_Macro;
		std::vector<Expression> m_Expressions;
	};

} // namespace Preprocessor
