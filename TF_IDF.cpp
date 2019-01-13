#include "stdafx.h"
#include "TF_IDF.h"
#include <algorithm>

TF_IDF::TF_IDF()
{
}


/** Construct the object and set the summary generation going*/
TF_IDF::TF_IDF(vector<string> fileNames, int summaryLength) : fileNames(fileNames){
	cout << "**Started up analysing via Term Frequency-Inverse Document Frequency method" << endl;
	summaryLengths = vector<int>();
	summaryLengths.push_back(summaryLength);
	std::cout.setstate(std::ios_base::failbit);
	matrix = map<string, map<string, pair<int, float>>>();
	sentences = map<string, vector<Sentence>>();
	writeSummaries();
}

/** Perform the text summarisation process from start to finish*/
void TF_IDF::writeSummaries() {
	readDocuments();
	updateMatrix();
	updateWeights();
	writeDocs();
}

/** Get the idf value for the particular word in the particular document*/
float TF_IDF::getIDF(string word, string fileName) {
	int docCount = sentences.size();
	int occurrenceCount = 0;
	for (string fileName2 : fileNames) {
		if (matrix[word][fileName2].first > 0) {
			occurrenceCount++;
		}
	}
	float idf = log(docCount/occurrenceCount);
	return idf;
}


/** Read the documents into the object's storage, ready for further use elsewhere.*/
bool TF_IDF::readDocuments() {
	ifstream reader;
	string line;
	string sentence;
	unsigned char prev1, prev2;

	//Whether it is possible that an initialism is being dealt with
	bool possIN = false;
	//Whether it is definite that an initialism is being dealt with
	bool isIN = false;
	for (string fileName : fileNames) {
		sentences.emplace(fileName, vector<Sentence>());
		sentence = "";
		reader.open(fileName);
		//reader.imbue(std::locale("en_US.UTF8"));
		if (!reader.is_open()) {
			cout << "*******FAILED TO OPEN: " << fileName << endl;
		}
		prev1 = u8' ';
		prev2 = u8' ';
		possIN = false;
		isIN = false;
		// for every line
		while (getline(reader, line)) {
			// 
			for (int position = 0; position < line.length(); position++) {
				sentence += line[position];
				// special cases for splitting the sentences
				if (line[position] == u8'?' || line[position] == u8'!' || (line[position] == u8'.' && !((isalpha(prev1) && isupper(prev1)) || (isdigit(prev1))))) {
					cout << "Sentence here: " << sentence << endl;
					sentences[fileName].push_back(Sentence(sentence));
					sentence = u8"";
					possIN = false;
				}
				else if (line[position] == u8' ' && prev1 == u8'.' && !((isalpha(prev2) && isupper(prev2)) || isdigit(prev2))) {
					sentences[fileName].push_back(Sentence(sentence));
					sentence = u8"";
					possIN = false;
				}
				prev2 = prev1;
				prev1 = line[position];
			}
		}
		reader.close();
	}
	return true;
}

// Update the matrix of word frequency and weights based on the current set of sentences
bool TF_IDF::updateMatrix() {
	matrix.clear();
	float fileSize = 0;
	//find the word count of each document for weighting purposes
	for (string fileName : fileNames) {
		fileSize = 0;
		for (Sentence sentence : sentences[fileName]) {
			fileSize += sentence.sentenceSplit.size();
		}


		//generate tf values
		for (Sentence sentence : sentences[fileName]) {
			//only update the matrix if the sentence has not been removed
			if (!sentence.removed) {
				for (string word : sentence.sentenceSplit) {
					if (matrix.find(word) == matrix.end()) {
						addWord(word);
					}
					matrix[word][fileName].first++;
				}
			}
		}
		//generate tf-idf values
		for (Sentence sentence : sentences[fileName]) {
			//only update the matrix if the sentence has not been removed
			if (!sentence.removed) {
				for (string word : sentence.sentenceSplit) {
					matrix[word][fileName].second = getIDF(word, fileName);
					matrix[word][fileName].second = matrix[word][fileName].first * matrix[word][fileName].second;
				}
			}
		}
	}
	return true;
}

// Add new word to the Matrix
void TF_IDF::addWord(string word) {
	matrix.emplace(word, map<string, pair<int, float>>());
	for (string fileName : fileNames) {
		matrix[word].emplace(fileName, pair<int, float>(0, 0));
	}
}

/** Update the sentences' weights based on the matrix values. Does not alter the matrix */
void TF_IDF::updateWeights() {
	cout << "*************UPDATING WEIGHTS************" << endl;

	for (string fileName : fileNames) {
		cout << "Filename: " << fileName << endl;
		//ref = sentence address
		int ref = 0;
		//for every Sentence associated with the fileName
		for (Sentence sentence : sentences[fileName]) {
			sentence.sentenceWeight = 0;
			for (string word : sentence.sentenceSplit) {
				// sentenceWeight += previously calculated tf*idf value
				sentence.sentenceWeight += matrix[word][fileName].second;
				cout << "matrix[" << word << "][" << fileName << "]: " << matrix[word][fileName].second << endl;
			}
			cout << "intermedate sentenceWeight: " << sentence.sentenceWeight << endl;
			sentences[fileName][ref].sentenceWeight = sentence.sentenceWeight / sentence.sentenceSplit.size();
			cout << "final normalised sentence weight : " << sentence.sentenceWeight << endl;
			ref++;
		}
	}
}

// Write out all summaries, updating the matrices and weights after each sentence selection in each summary document
void TF_IDF::writeDocs() {
	int totalWeight = 0;
	for (int length : summaryLengths){
		// name of the file to be saved to
		string saveFile = "summary_" + to_string(length) + ".txt";
		ofstream output(saveFile);
		// current number of word in summary document
		int currLength = 0;
		// current best sentence weight
		float bestVal = 0;
		// current best sentence ref
		int bestRef = 0;
		// current best filename
		string bestFileName = "";
		while (currLength < length) {
			bestVal = 0;
			bestRef = 0;
			bestFileName = "";
			// find the best sentence
			for (string fileName : fileNames) {
				cout << "Evaluating: " << fileName << endl;
				for (int ref = 0; ref < sentences[fileName].size(); ref++) {
					// if the sentence has a better weight, has not been removed and can fit in the document:
					if (!(sentences[fileName][ref].removed) && sentences[fileName][ref].sentenceWeight > bestRef && sentences[fileName][ref].sentenceSplit.size() <= (length - currLength)) {
						// make it the new best sentence
						bestVal = sentences[fileName][ref].sentenceWeight;
						bestRef = ref;
						bestFileName = fileName;
					}
				}
			}
			//As long as a real sentence has been found correctly:
			if (bestFileName != u8"" && currLength + sentences[bestFileName][bestRef].sentenceSplit.size() <= length) {
				// take out the sentence and add it to the summary document
				// not worrying about the location of the sentence in the document, only the weight
				currLength += sentences[bestFileName][bestRef].sentenceSplit.size();
				output << sentences[bestFileName][bestRef].sentenceOriginal << " " << endl;
				cout << "Length = " << sentences[bestFileName].size() << endl;
				cout << "Erasing element: " << bestRef << endl;
				sentences[bestFileName][bestRef].removed = true;
				cout << "Length = " << sentences[bestFileName].size() << endl;
				totalWeight += bestVal;
			}
			else {
				// no sentence can fit, so end with what we have.
				currLength = length;
			}
			cout << "currLength: " << currLength << endl;
			cout << "length : " << length << endl;
			// with the sentence removed, recalculate weights and pick the next sentence
			updateMatrix();
			updateWeights();
		}
		output.close();
		
		
		cout.clear();
		cout << "Written ./" << saveFile << endl;
		std::cout.setstate(std::ios_base::failbit);
		//reset removed states for each new summary document
		for (string fileName : fileNames) {
			for (Sentence sentence : sentences[fileName]) {
				sentence.removed = false;
			}
		}
	}
	cout.clear();
	cout << "Finished writing summaries" << endl;
}

//Calculate similarity between target and set of sentences. Similarity = (Word Matches/words in all sentences combined)
// Note: not actually being used
float TF_IDF::sentenceSimilarity(Sentence target, vector<Sentence> storedSentences) {
	float similarity = 0;
	//int totalWords = target.sentenceSplit.size();
	int matches = 0;
	for (string word : target.sentenceSplit) {
		for (Sentence sentence : storedSentences) {
			// get number of matches for the word in the given sentence
			matches = count(sentence.sentenceSplit.begin(), sentence.sentenceSplit.end(), word);
			if (matches / (target.sentenceSplit.size() * sentence.sentenceSplit.size())) {
				similarity = matches / (target.sentenceSplit.size() * sentence.sentenceSplit.size());
			}
		}
	}
	return similarity;
}

TF_IDF::~TF_IDF()
{
}
