#pragma once
#include <stdio.h>

class File
{
public:
	File() : m_FI(nullptr)
	{
	}

	~File()
	{
		Close();
	}

	bool Open(const char* fileName, const char* mode)
	{
		if (m_FI != nullptr) {
			Close();
		}
		m_FI = fopen(fileName, mode);
		return m_FI != nullptr;
	}

	void Close()
	{
		if (m_FI != nullptr) {
			fclose(m_FI);
			m_FI = nullptr;
		}
	}

protected:
	FILE* m_FI;

};


