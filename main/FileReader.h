#pragma once


class FileReader
{
public:
	FileReader() : m_FI(nullptr)
	{
	}

	~FileReader()
	{
		Close();
	}

	bool Open(const char* fileName)
	{
		if (m_FI != nullptr)
		{
			Close();
		}
		m_FI = fopen(fileName, "rt");
		return m_FI != nullptr;
	}

	void Close()
	{
		if (m_FI != nullptr)
		{
			fclose(m_FI);
			m_FI = nullptr;
		}
	}

	bool ReadLine(std::string& line)
	{
		line.clear();
		if (m_FI != nullptr)
		{
			char buff[1024]; buff[1023] = '\0';
			char* str = fgets(buff, 1023, m_FI);
			if (str != nullptr)
			{
				size_t len = strlen(buff);
				line.reserve(len + 1);
				line = buff;
			}
		}
		// Check for continued line
		size_t length = line.length();
		for (size_t i = length; i > 0; --i)
		{
			if (isspace(line[i - 1])) {
				continue;
			}
			if (line[i - 1] == '\\')
			{
				line.resize(i - 1);
				std::string continuedLine;
				if (ReadLine(continuedLine))
				{
					line += continuedLine;
				}
			}
			break;
		}
		return line.length() > 0;
	}

private:
	FILE* m_FI;

};


