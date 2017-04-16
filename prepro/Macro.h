#pragma once
#include <string>

namespace Preprocessor {

	struct Macro
	{
		std::string Name;
		int Value;
		Macro() : Value(0) {}
		Macro(const char* name, int value) : Name(name), Value(value) {}
	};

} // namespace Preprocessor

