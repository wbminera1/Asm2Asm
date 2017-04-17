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
		if (IsValid()) {
			Close();
		}
		m_FI = fopen(fileName, mode);
		return IsValid();
	}

	void Close()
	{
		if (IsValid()) {
			fclose(m_FI);
			m_FI = nullptr;
		}
	}

	bool IsValid() const {
		return (m_FI != nullptr);
	}

protected:
	FILE* m_FI;

};


