#ifndef TERMFREQUENCY_H
#define TERMFREQUENCY_H
#include "Sentence.h"
#include <ostream>
#include <locale>
using namespace std;

/** Class for handling the implementation of the term frequency algorithm on the given sentence */
struct TermFrequency
{
public:
	TermFrequency();

	/** Construct using the given fileNames a summary of the given length */
	TermFrequency(vector<string> fileNames, vector<int> summaryLengths);
	~TermFrequency();

	/** Update the matrix of word occurrence and weights using the active sentences*/
	bool updateMatrix();

	/** Update the weights of the sentences using the stored values in the matrix - does not alter the matrix*/
	void updateWeights();

	/** Write the Summaries of the documents of names in fileNames*/
	void writeSummaries();

	//Matrix of word and their corresponding frequencies and weights
	//Form: [word][document_id]<occurrences, weight>
	map<string, map<string,pair<int,float>>> matrix;
	
	//All sentences for each fileName
	map<string, vector<Sentence>> sentences;

	//stop words
	vector<string> stopWords;

	//All file names to be read from
	vector<string> fileNames;

	// Lengths of summaries to be produced
	vector<int> summaryLengths;

	/** Write the documents based on the previously stored weights*/
	void writeDocs();

	/** Add word to the matrix of word weights*/
	void addWord(string word);
	
	/** Read in the stopWord */
	bool readStopWords();

	/** Read in the documents*/
	bool readDocuments();

	/** Calculate the highest similarity between the target sentence and one of the given storedSentences */
	float sentenceSimilarity(Sentence target, vector<Sentence> storedSentences);
};

#endif