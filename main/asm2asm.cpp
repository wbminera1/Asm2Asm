// asm2asm.cpp : Defines the entry point for the console application.
//

#include "../stdafx.h"
#include "../../getopt/getopt.h"
#include "../prepro/Preprocessor.h"
#include "FileReader.h"
#include "FileWriter.h"

class LineProcessor
{
public:

	enum Arch
	{
		eNone,
		e32,
		e64
	};

	LineProcessor() : m_CommentOut(false), m_Arch(eNone) { }

	bool Process(FileReader& reader, FilePartsWriter& writer, Preprocessor::Processor& preprocessor) const
	{
		WriteHeader(writer);
		std::string line;
		while (reader.ReadLine(line))
		{
			std::string preprocessed;
			preprocessor.Process(line, preprocessed);
			if (preprocessed.length() > 0) {
				writer.Write(preprocessed);
			}
			else {
				if (m_CommentOut) {
					preprocessed = "//";
					preprocessed += line;
					writer.Write(preprocessed);
				}
			}
			std::string token;
			GetToken(line.c_str(), token);
			if (m_SplitString == token) {
				writer.NewPart();
				WriteHeader(writer);
			}
		}
		return true;
	}

	bool WriteHeader(FilePartsWriter& writer) const
	{
		if (m_Arch == e32) {
			return writer.Write(std::string(".code32\n"));
		}
		if (m_Arch == e64) {
			return writer.Write(std::string(".code64\n"));
		}
		return false;
	}

	bool		m_CommentOut;
	Arch		m_Arch;
	std::string m_SplitString;
};

int main(int argc, char **argv)
{
	FileReader reader;
	FilePartsWriter writer;
	Preprocessor::Processor preprocessor;
	LineProcessor processor;

	std::string inputFile;
	std::string outputFile;

	int option;
	std::string optionVal;
	while ((option = getopt(argc, argv, "?a:hcs:D:O:")) != -1) {
		switch (option)
		{
		case 'a':
			optionVal = optarg;
			if (optionVal == "32") {
				processor.m_Arch = LineProcessor::e32;
			}
			else if(optionVal == "64") {
				processor.m_Arch = LineProcessor::e64;
			}
			else {
				printf("-a incorrect\n");
				return 1;
			}
			break;
		case 'c':
			processor.m_CommentOut = true;
			break;
		case 's':
			optionVal = optarg;
			if (optionVal.size() > 0) {
				processor.m_SplitString = optionVal;
			}
			else {
				printf("-s - argument expected\n");
				return 1;
			}
			break;
		case 'D':
			optionVal = optarg;
			if (optionVal.size() > 0) {
				preprocessor.m_Macro.push_back(Preprocessor::Macro(optionVal.c_str(),1));
			}
			else {
				printf("-D - argument expected\n");
				return 1;
			}
			break;
		case 'O':
			optionVal = optarg;
			if (optionVal.size() > 0) {
				outputFile = optionVal;
			}
			else {
				printf("-O - file name expected\n");
				return 1;
			}
			break;
		case '?':
		case 'h':
			printf("Asm2Asm\n");
			printf("-a 32|64 architecture\n");
			printf("-c comment strings\n");
			printf("-D arg define symbol\n");
			printf("-O arg output file\n");
			return 1;
		default:
			;
		}
	}

	if (optind < argc) {
		inputFile = argv[optind];
	}
	else {
		return 1;
	}

	if (inputFile.length() > 0 && reader.Open(inputFile.c_str(), "rt"))
	{
		if (outputFile.length() > 0) {
			if (!writer.OpenParts(outputFile.c_str(), "wt", processor.m_SplitString.empty() ? -1 : 0)) {
				return 1;
			}
		}
		processor.Process(reader, writer, preprocessor);
	}

    return 0;
}

