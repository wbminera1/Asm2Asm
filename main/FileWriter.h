#pragma once
#include "File.h"

class FileWriter : public File
{
public:
	FileWriter() {}

	bool Write(const std::string& str)
	{
		if (m_FI != nullptr) {
			int res = fputs(str.c_str(), m_FI);
			if (res < 0) {
				return false;
			}
		}
		else {
			printf("%s", str.c_str());
		}
		return true;
	}
};


