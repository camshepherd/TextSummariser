#ifndef DOCUMENT_H
#define DOCUMENT_H
#include "Sentence.h"

class Document
{
public:
	Document();
	Document(string fileName, weak_ptr<vector<string>> stopWords, weak_ptr<map<string, map<string,int>>> matrix);
	~Document();

	
	void readDoc();
	string fileName;
	vector<Sentence> sentences;
	float weight;
	map<string, int> countOccurrences();
	weak_ptr<vector<string>> stopWords;
	weak_ptr<map<string, map<string,int>>> matrix;
	void updateWeights();
};

#endif