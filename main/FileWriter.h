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

class FilePartsWriter : public FileWriter
{
	public:
		FilePartsWriter() : m_Idx(-1)
		{ }

		bool OpenParts(const char* fileName, const char* mode, int startIdx = -1, const char* suffix = "_%d")
		{
			m_OriginalName = fileName;
			m_OriginalMode = mode;
			m_Suffix = suffix;
			m_Idx = startIdx;
			if (m_Idx == -1) {
				return Open(fileName, mode);
			}
			return OpenPart();
		}

		bool OpenPart()
		{
			std::string partName = MakePartName();
			if (!partName.empty()) {
				++m_Idx;
				return Open(partName.c_str(), m_OriginalMode.c_str());
			}
			return false;
		}

		std::string MakePartName() const
		{
			std::string name;
			if (m_Suffix.length() > 0 && m_Idx >= 0) {
				char buffer[1024];
				int res = snprintf(buffer, 1023, m_Suffix.c_str(), m_Idx);
				if (res >= 0) {
					name = m_OriginalName + buffer;
				}
			}
			return name;
		}

		bool NewPart()
		{
			if (IsValid()) {
				Close();
				return OpenPart();
			}
			return false;
		}

protected:
		std::string m_OriginalName;
		std::string m_OriginalMode;
		std::string m_Suffix;
		int m_Idx;
};
