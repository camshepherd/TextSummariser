#ifndef TF_IDF_H
#define TF_IDF_H
#include "Sentence.h"
#include <ostream>
#include <locale>
#include <tuple>

using namespace std;

struct TF_IDF
{
public:
	TF_IDF();

	/** Perform the tf*idf weighting to generate summary document, of length summaryLength, from the given set of file names */
	TF_IDF(vector<string> fileNames, int summaryLength);
	~TF_IDF();

	/** Update the matrix of word occurrence and weights using the active sentences*/
	bool updateMatrix();

	/** Update the weights of the sentences using the stored values in the matrix - does not alter the matrix*/
	void updateWeights();

	/** Write the Summaries of the documents of names in fileNames*/
	void writeSummaries();

	//Matrix of word and their corresponding frequencies and weights
	//Form: [word][document_id]<term frequency, tf*idf>
	map<string, map<string, pair<int, float>>> matrix;

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

	/** Get the idf value for the given word in the given fileName*/
	float getIDF(string word, string fileName);

	/** Read in the documents*/
	bool readDocuments();

	/** Calculate the highest similarity between the target sentence and one of the given storedSentences */
	float sentenceSimilarity(Sentence target, vector<Sentence> storedSentences);
};

#endif