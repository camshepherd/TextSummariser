#include "stdafx.h"
#include "Document.h"


Document::Document()
{
	fileName = "";
	weight = -1;
	sentences = vector<Sentence>();
}

Document::Document(string fileName, weak_ptr<vector<string>> stopWords_, weak_ptr<map<string, map<string,int>>> matrix) : fileName(fileName), stopWords(stopWords),matrix(matrix){
	weight = -1;
	sentences = vector<Sentence>();
	readDoc();
}

map<string, int> Document::countOccurrences() {
	map<string, int> occurrences();
	for (Sentence sentence : sentences) {
		for (string word : sentence.sentenceSplit) {

		}
	}
}

void Document::readDoc() {
	ifstream doc;
	sentences = vector<Sentence>();
	string sentence = "";
	string line = "";
	doc.open(fileName);
	while (getline(doc, line)) {
		for (char character : line) {
			sentence += character;
			if (character == '.' || character == '?' || character == '!') {
				sentences.push_back(Sentence(sentence, fileName, stopWords, matrix));
				sentence = "";
			}
		}
	}
	if (sentence.length() > 0) {
		sentences.push_back(Sentence(sentence, fileName, stopWords, matrix));
	}
	doc.close();
	weight = -1;
}

void Document::updateWeights() {
	for (auto sentence : sentences) {
		sentence.getWeight();
	}
}

Document::~Document()
{
}
