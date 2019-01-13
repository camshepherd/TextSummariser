#include "stdafx.h"
#include "Sentence.h"


Sentence::Sentence()
{
	sentenceOriginal = "";
	sentenceSplit = vector<string>();
	sentenceWeight = 0;
}

Sentence::Sentence(string sentence) : sentenceOriginal(sentence){
	splitSentence(vector<string>());
	sentenceWeight = 0;
}

Sentence::Sentence(string sentence, vector<string> stopWords) : sentenceOriginal(sentence){
	splitSentence(stopWords);
	sentenceWeight = 0;
}

int Sentence::wordCount(string targetWord) {
	int count = 0;
	for (string word : sentenceSplit) {
		if (word == targetWord) {
			count++;
		}
	}
	return count;
}

void Sentence::splitSentence(vector<string> stopWords) {
	sentenceSplit = vector<string>();
	stringstream reader(sentenceOriginal);
	string line = u8"";
	string word = u8"";
	while (getline(reader, line)) {
		// Given that each line will be an entire sentence:
		for (char character : line) {
			if (isalpha(character) || isdigit(character)) {
				// Make sure that the input is all lowercase
				word += tolower(character);
			}
			// If it's possible that there is a word separator:
			else if (ispunct(character) || character == u8' ') {
				
				if (count(stopWords.begin(), stopWords.end(), word) == 0) {
					// Split as long as the word is definitely not an initialism
					if (word.length() > 2 && !(word[word.length() - 1] == u8'.' && word[word.length() - 3] == u8'.')) {
						// Allow for dumb writing of initialisms here
						sentenceSplit.push_back(word);
					}
				}
				word = u8"";
			}
		}
	}
	cout << "Original: " << endl << sentenceOriginal << endl;
	cout << "Split: " << endl;
	for (string word : sentenceSplit) {
		cout << word << ",";
	}
	cout << endl;
}

Sentence::~Sentence()
{
}
