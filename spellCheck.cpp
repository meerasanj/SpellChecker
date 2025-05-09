#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;
#include "HashTable.h"
#include "BinarySearchTree.h"

// define binary node for client file (given by dr. digh)
struct BNode {
	string word;
	int lineNum;
	bool operator < (BNode otherStruct) const {
		return ( word < otherStruct.word );
	}
};
typedef BinarySearchTree<BNode> BST;
ostream & operator << (ostream & out, BNode & temp) {
	out << temp.word << " " << temp.lineNum;
	return out;
}

// function prototypes
string cleanWord(const string &word);
void spellCheckFile(ifstream &inFile, HashTable &dictionary, HashTable &ignoredWords, BST &wordsNotFound);
void processMisspelledWord(const string &word, const string &cleanedWord, int lineNumber, HashTable &dictionary, HashTable &ignoredWords, BST &wordsNotFound);
vector<string> generateSuggestions(const string &cleanedWord, HashTable &dictionary);
void displayMenu(const string& cleanedWord, int lineNumber);
void outputMisspelledWords(BST &wordsNotFound);

// main method 
int main(int argc, char *argv[]) {	
	// initialize data structures
	HashTable dictionary(500); // store dictionary (dict.txt)
	HashTable ignoredWords(100); // store words to be ignored 
	BNode defaultNode; 
	BST wordsNotFound(defaultNode);
		
	// load words into dictionary hash table
	ifstream dictFile("dict.txt");
	if(dictFile) {
		string word;
		while(dictFile >> word) {
			dictionary.insert(word);
		}
		dictFile.close();
	} else {
		cout << "Dictionary file not found. Starting with an empty dictionary.\n";
	}
	
	ifstream inputFile(argv[1]);
	if (!inputFile.is_open()) {
		cerr << "Error: Unable to open source file.\n";
		return 1;
	}

	spellCheckFile(inputFile, dictionary, ignoredWords, wordsNotFound);		
	outputMisspelledWords(wordsNotFound);

	cout << "Spell checking complete. Misspelled words have been processed.\n";

	return 0;
}

// method to spell check the input file 
void spellCheckFile(ifstream &inFile, HashTable &dictionary, HashTable &ignoredWords, BST &wordsNotFound) {
	string line;
	int lineNumber = 1;
	while(getline(inFile, line)) { // reads each line from file 
		stringstream ss(line);
		string word;
		while(ss >> word) {
			string cleanedWord = cleanWord(word);
			if (!cleanedWord.empty() && !dictionary.find(cleanedWord) && !ignoredWords.find(cleanedWord)) {
				BNode bN = {cleanedWord, lineNumber};
				wordsNotFound.insert(bN);
				processMisspelledWord(word, cleanedWord, lineNumber, dictionary, ignoredWords, wordsNotFound);
			}
		}
		lineNumber++;
	}
	inFile.close();
}

// method to display menu for word options 
void displayMenu(const string& cleanedWord, int lineNumber) {
	cout << "\n!@#$%^&*(){} THE SPELL CHECKER PROGRAM !@#$%^&*(){}\n" << endl;
	cout << cleanedWord << " On Line " << lineNumber << " Was Not Found In Dictionary\n" << endl;	
	cout << "A) Add the Word To Dictionary\n";
	cout << "I) Ignore Word, and Skip Future References\n";
	cout << "G) Go On To Next Word\n";
	cout << "S) Search For A Suggested Spelling\n";
	cout << "Q) Quit Spell Checking File\n" << endl;
	cout << "Selection: ";
}

// method to process misspelled word based on user choices 
void processMisspelledWord(const string &word, const string &cleanedWord, int lineNumber, HashTable &dictionary, HashTable &ignoredWords, BST &wordsNotFound) {
	char choice; 
	displayMenu(cleanedWord, lineNumber);
	cin >> choice;
	cin.ignore();
	choice = tolower(choice);
	if(choice == 'a') { // add word to dictionary file 
		dictionary.insert(cleanedWord);
		ofstream dictOut("dict.txt", ios::app); // use of ios::app from https://cplusplus.com/reference/ios/ios_base/openmode/#google_vignette
		dictOut << cleanedWord << endl;
		dictOut.close();
		cout << endl;
		cout << cleanedWord << " was added to the dictionary!" << endl;
	} else if(choice == 'i') { // ignore word and skip future references
		ignoredWords.insert(cleanedWord);
		cout << "\nAll future references of " << cleanedWord << " will be skipped." << endl;
	} else if(choice == 'g') {  // go to next word
		return;
	} else if(choice == 's') { // spelling suggestions
		// create vector of string type to hold suggestions  
		vector<string> suggestions = generateSuggestions(cleanedWord, dictionary);
		if(!suggestions.empty()) {
			cout << "\nSuggested Spelling(s) : ";
			for(int i = 0; i < suggestions.size(); ++i) {
				cout << suggestions[i] << "   ";
			}
			cout << endl;
		} else {
			cout << "\nNo suggested spellings were found in dictionary." << endl;
		}
		cout << "\nPlease Hit Return to Continue..." << endl;		
		cin.get();
	} else if(choice == 'q') { // quit spell checking
		cout << "Now Exiting Program...\n";
		outputMisspelledWords(wordsNotFound);
		exit(0);
	} else {
		cout << "\nInvalid Choice -- Please Hit Return to Continue." << endl;
		cin.get();
	}	

}

// method to generate spelling suggestions for a word 
vector<string> generateSuggestions(const string &cleanedWord, HashTable &dictionary) {
	vector<string> suggestions; // create vector to store spelling suggestions
	string temp;
	// swapping adjacent characters
	for (size_t i = 0; i < cleanedWord.length() - 1; ++i) {
		temp = cleanedWord;
		swap(temp[i], temp[i + 1]);
		bool found = dictionary.find(temp);
		if (found) {
			suggestions.push_back(temp);
		}
	}

	// adding one character
	for (size_t i = 0; i <= cleanedWord.length(); ++i) {
		for (char c = 'a'; c <= 'z'; ++c) {
			temp = cleanedWord;
			temp.insert(i, 1, c);
			if (dictionary.find(temp)) {
				suggestions.push_back(temp);
			}
		}
	}

	// removing one character
	for (size_t i = 0; i < cleanedWord.length(); ++i) {
		temp = cleanedWord;
		temp.erase(i, 1);
		if (dictionary.find(temp)) {
			suggestions.push_back(temp);
		}
	}

	// replacing one character
	for (size_t i = 0; i < cleanedWord.length(); ++i) {
		for (char c = 'a'; c <= 'z'; ++c) {
			temp = cleanedWord;
			temp[i] = c;
			if (dictionary.find(temp)) {
				suggestions.push_back(temp);
			}
		}
	}						

	return suggestions;
}

// method to clean word, removes non-letters
string cleanWord(const string &word) {
	string cleanedWord;
	for (int i = 0; i < word.length(); ++i) {
		if(isalpha(word[i])) {
			cleanedWord += tolower(word[i]);
		}
	}
	return cleanedWord;
} 

// method to output all words not found in dictionary to external file 
void outputMisspelledWords(BST &wordsNotFound) {
	ofstream notfoundOut("notfound.txt");
	if (!(wordsNotFound.isEmpty())) {
		wordsNotFound.printTree(notfoundOut);
	}
	notfoundOut.close();
}
