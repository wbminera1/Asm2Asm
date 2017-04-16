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
	LineProcessor() : m_CommentOut(false) { }

	bool Process(FileReader& reader, FileWriter& writer, Preprocessor::Processor& preprocessor)
	{
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
		}
		return true;
	}

	bool m_CommentOut;
};

int main(int argc, char **argv)
{
	FileReader reader;
	FileWriter writer;
	Preprocessor::Processor preprocessor;
	LineProcessor processor;

	std::string inputFile;
	std::string outputFile;

	int option;
	std::string optionVal;
	while ((option = getopt(argc, argv, "cD:O:")) != -1) {
		switch (option)
		{
		case 'c':
			processor.m_CommentOut = true;
			break;
		case 'D':
			optionVal = optarg;
			if (optionVal.size() > 0) {
				preprocessor.m_Macro.push_back(Preprocessor::Macro(optionVal.c_str(),1));
			}
			else {
				return 1;
			}
			break;
		case 'O':
			optionVal = optarg;
			if (optionVal.size() > 0) {
				outputFile = optionVal;
			}
			else {
				return 1;
			}
			break;
		case '?':
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
			if (!writer.Open(outputFile.c_str(), "wt")) {
				return 1;
			}
		}
		processor.Process(reader, writer, preprocessor);
	}

    return 0;
}

