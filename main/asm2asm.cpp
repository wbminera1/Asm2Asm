// asm2asm.cpp : Defines the entry point for the console application.
//

#include "../stdafx.h"
#include "../../getopt/getopt.h"
#include "../prepro/Preprocessor.h"
#include "FileReader.h"

class FileWriter
{
public:
	bool Write(const std::string& str)
	{
		printf("%s", str.c_str());
		return true;
	}
};



class LineProcessor
{
public:
	bool Process(FileReader& reader, FileWriter& writer, Preprocessor::Processor& preprocessor)
	{
		std::string line;
		while (reader.ReadLine(line))
		{
			std::string preprocessed;
			preprocessor.Process(line, preprocessed);
			if (preprocessed.length() > 0)
			{
				writer.Write(preprocessed);
			}
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	FileReader reader;
	FileWriter writer;
	Preprocessor::Processor preprocessor;
	std::string inputFile;

	int option;
	while ((option = getopt(argc, argv, "abc:")) != -1) {
		switch (option)
		{
		case 'a':
			break;
		case 'b':
			break;
		case 'c':
			//cvalue = optarg;
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

	if (inputFile.length() > 0 && reader.Open(inputFile.c_str()))
	{
		LineProcessor processor;
		processor.Process(reader, writer, preprocessor);
	}

    return 0;
}

