#include "stdafx.h"
#include "TermFrequency.h"
#include <algorithm>

TermFrequency::TermFrequency()
{
}


/** Construct the object and set the summary generation going*/
TermFrequency::TermFrequency(vector<string> fileNames, vector<int> summaryLengths) : fileNames(fileNames), summaryLengths(summaryLengths){
	cout << "**Started up analysing via Term Frequency method" << endl;
	std::cout.setstate(std::ios_base::failbit);
	matrix = map<string, map<string, pair<int,float>>>();
	sentences = map<string, vector<Sentence>>();
	writeSummaries();
}

/** Performt the text summarisation process from start to finish*/
void TermFrequency::writeSummaries() {
	readStopWords();
	readDocuments();
	updateMatrix();
	updateWeights();
	writeDocs();
}

/** Read in the stop words, ready for use elsewhere*/
bool TermFrequency::readStopWords() {
	stopWords = vector<string>();
	string fileName = "stopwords_en.txt";
	string line = "";
	string word = "";
	ifstream stopWordFile(fileName);
	while (getline(stopWordFile, line)) {
		word = line;
		if (word.length() > 0) {
			stopWords.push_back(string(word));
		}
	}
	stopWordFile.close();
	return true;
}

/** Read the documents into the object's storage, ready for further use elsewhere.*/
bool TermFrequency::readDocuments() {
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
				cout << "Character = " << endl;
				// special cases for splitting the sentences
				if (line[position] == u8'?' || line[position] == u8'!' || (line[position] == u8'.' && !((isalpha(prev1) && isupper(prev1)) || (isdigit(prev1))))) {
					sentences[fileName].push_back(Sentence(sentence, stopWords));
					sentence = u8"";
					possIN = false;
				}
				else if (line[position] == u8' ' && prev1 == u8'.' && !((isalpha(prev2) && isupper(prev2)) || isdigit(prev2))) {
					sentences[fileName].push_back(Sentence(sentence, stopWords));
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
bool TermFrequency::updateMatrix() {
	matrix.clear();
	float fileSize = 0;
	//find the word count of each document for weighting purposes
	for (string fileName : fileNames) {
		fileSize = 0;
		for (Sentence sentence : sentences[fileName]) {
			fileSize += sentence.sentenceSplit.size();
		}

		for (Sentence sentence : sentences[fileName]) {
			//only update the matrix if the sentence has not been removed
			if (!sentence.removed) {
				for (string word : sentence.sentenceSplit) {
					if (matrix.find(word) == matrix.end()) {
						addWord(word);
					}
					matrix[word][fileName].first++;
					matrix[word][fileName].second += float(1 / fileSize);
				}
			}
		}
	}
	return true;
}

// Add new word to the Matrix
void TermFrequency::addWord(string word) {
	matrix.emplace(word, map<string, pair<int,float>>());
	for (string fileName : fileNames) {
		matrix[word].emplace(fileName, pair<int,float>(0,0));
	}
}

// Update the sentences' weights based on the matrix values
void TermFrequency::updateWeights() {
	cout << "*************UPDATING WEIGHTS************" << endl;
	
	for (string fileName : fileNames) {
		cout << "Filename: " << fileName << endl;
		int ref = 0;
		for (Sentence sentence : sentences[fileName]) {
			sentence.sentenceWeight = 0;
			for (string word : sentence.sentenceSplit) {
				sentence.sentenceWeight += matrix[word][fileName].second;
				cout << "matrix[" << word << "][" << fileName << "]: " << matrix[word][fileName].first << endl;
			}
			cout << "intermedate sentenceWeight: " << sentence.sentenceWeight << endl;
			sentences[fileName][ref].sentenceWeight = sentence.sentenceWeight / sentence.sentenceSplit.size();
			cout << "final normalised sentence weight : " << sentence.sentenceWeight << endl;
			ref++;
		}
	}
}

// Write out all summaries, updating the matrices and weights after each sentence selection in each summary document
void TermFrequency::writeDocs() {
	float totalWeight = 0;
	for (int length : summaryLengths) {
		string saveFile = "summary_" + to_string(length) + ".txt";
		ofstream output(saveFile);
		int currLength = 0;
		float bestVal = 0;
		int bestRef = 0;
		string bestFileName = "";
		while (currLength < length) {
			bestVal = 0;
			bestRef = 0;
			bestFileName = "";
			for (string fileName : fileNames) {
				cout << "Evaluating: " << fileName << endl;
				for (int ref = 0; ref < sentences[fileName].size(); ref++) {
					if (!(sentences[fileName][ref].removed) && sentences[fileName][ref].sentenceWeight > bestRef && sentences[fileName][ref].sentenceSplit.size() <= (length - currLength)) {
						bestVal = sentences[fileName][ref].sentenceWeight;
						bestRef = ref;
						bestFileName = fileName;
					}
				}
			}
			if (bestFileName != u8"" && currLength + sentences[bestFileName][bestRef].sentenceSplit.size() <= length) {
				currLength += sentences[bestFileName][bestRef].sentenceSplit.size();
				output << sentences[bestFileName][bestRef].sentenceOriginal << " " << endl;
				cout << "Length = " << sentences[bestFileName].size() << endl;
				cout << "Erasing element: " << bestRef << endl;
				sentences[bestFileName][bestRef].removed = true;
				cout << "Length = " << sentences[bestFileName].size() << endl;
				totalWeight += bestVal;
			}
			else {
				currLength = length;
			}
			cout << "currLength: " << currLength << endl;
			cout << "length : " << length << endl;
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
float TermFrequency::sentenceSimilarity(Sentence target, vector<Sentence> storedSentences) {
	float similarity = 0;
	//int totalWords = target.sentenceSplit.size();
	int matches = 0;
	for (string word : target.sentenceSplit) {
		for (Sentence sentence : storedSentences) {
			matches = count(sentence.sentenceSplit.begin(), sentence.sentenceSplit.end(), word);
			if (matches / (target.sentenceSplit.size() * sentence.sentenceSplit.size())) {
				similarity = matches / (target.sentenceSplit.size() * sentence.sentenceSplit.size());
			}
		}
	}
	return similarity;
}

TermFrequency::~TermFrequency()
{
}
