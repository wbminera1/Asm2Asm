#pragma once


	inline bool isbracket(int chr)
	{
		return (chr == '(') || (chr == ')') || (chr == '{') || (chr == '}') || (chr == '[') || (chr == ']');
	}

	inline bool ispunct_(int chr)
	{
		return ispunct(chr) && chr != '_';
	}

	inline bool isalnum_(int chr)
	{
		return isalnum(chr) || chr == '_';
	}

	inline const char* GetToken(const char* str, std::string& token)
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


