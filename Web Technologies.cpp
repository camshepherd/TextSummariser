// Web Technologies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TF_IDF.h"
#include "TermFrequency.h"
int main(int argc, char* argv[])
{
	vector<string> args;
	string arg;
	for (int x = 1; x < argc; x++) {
		arg = "";
		for (int y = 0; argv[x][y] != '\0'; y++) {
			arg += argv[x][y];
		}
		args.push_back(arg);
	}

	vector<string> fileNames = vector<string>();
	vector<int> summaryLengths = vector<int>();
	bool readingDocNames = true;
	bool readingLength = false;
	bool idf = false;
	for (string arg : args) {
		if (readingDocNames) {
			if (arg[0] != '-' || arg[1] != '-') {
				fileNames.push_back(arg);
			}
			else if (arg == "--length") {
				readingLength = true;
				readingDocNames = false;
			}
			else if (arg == "--tf") {
				idf = false;
			}
			else if (arg == "--tf-idf") {
				idf = true;
			}
		}
		else if (readingLength) {
			summaryLengths.push_back(stoi(arg));
			readingDocNames = true;
			readingLength = false;
		}
	}
	
	if (idf) {
		TF_IDF thingy(fileNames, summaryLengths[0]);
	}
	else {
		TermFrequency thingy(fileNames, summaryLengths);
	}
	getchar();
    return 0;
}
