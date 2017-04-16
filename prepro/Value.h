#pragma once

namespace Preprocessor {
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

}  // namespace Preprocessor
