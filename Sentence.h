#ifndef SENTENCE_H
#define SENTENCE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <locale>
using namespace std;

/** Represents a sentence, along with its corresponding weights, and the different representations of the raw sentence */
struct Sentence
{
	Sentence();

	/** Initialise using the sentence and don't use stopwords */
	Sentence(string sentence);

	/** Initialise using the sentence and remove all words in sentence that are in stopWords*/
	Sentence(string sentence, vector<string> stopWords);
	~Sentence();

	/** Count the number of times the given word occurs in the sentence*/
	int wordCount(string targetWord);

	/** Split the sentence into its constituent words*/
	void splitSentence(vector<string> stopWords);
	
	
	/** Whether the sentence has been removed or not*/
	bool removed = false;

	/**The original sentence*/
	string sentenceOriginal;

	/** The words making up the sentence - in their original order*/
	vector<string> sentenceSplit;

	/** The weight of the sentence*/
	float sentenceWeight;
	 
};

#endif