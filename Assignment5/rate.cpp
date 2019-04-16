#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include <cassert>

using namespace std;

const int MAX_WORD_LENGTH = 20;
int makeProper(char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int separation[],
	int nPatterns);

int rate(const char document[],
	const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	const int separation[],
	int nPatterns);

int removeBadElements(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int separation[], int size, int flags[]);
void removeFlaggedValuesInDocument(const char document[], char newDocument[], int flags[], int n);


int main() {
	char a[9][MAX_WORD_LENGTH + 1] = { "plot", "have", "deranged","" ,"mad", "DERANGED", "NeFaRIoUS", "mad", "nefarious"};
	char b[9][MAX_WORD_LENGTH + 1] = { "Nefarious", "have", "robot","have" ,"scientist!","ROBOT", "PLOt", "scientist", "plot"};
	int s[9] = { 0,1,0,0 , 0,3,0,1,1};
	int count = makeProper(a, b, s, 9);
	cout << "makeProper:" << endl;
	cout << "it should be(order independent):" << endl << "deranged, robot, 3" << endl << "nefarious, plot, 1" << endl << "mad, scientist, 1" << endl << "have, have, 1" << endl << "count: 4" << endl << endl << "it is:" << endl;
	for (int j = 0; j < count; j++) {
		cout << a[j];
		cout << ", ";
		cout << b[j];
		cout << ", ";
		cout << s[j] << endl;
	}
	cout << "count: " << count << endl << endl << "Rate: these should all be 1" << endl;
	char c[250] = "i have a mad big scientist that has a plot that is nefarious";
	cout << (rate(c, a, b, s, count) == 1);
	const int TEST1_NRULES = 4;
	char test1w1[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"mad",       "deranged", "nefarious", "have"
	};
	char test1w2[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"scientist", "robot",    "plot",      "have"
	};
	int test1dist[TEST1_NRULES] = {
		1,           3,          1,           1
	};
	cout << endl << (rate("The mad UCLA scientist unleashed a deranged evil giant robot.",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 2) << endl;
	cout << (rate("The ma55d UCLA   sci23entist unleashed    a der$anged  2 3 4 5  robot.", test1w1, test1w2, test1dist, TEST1_NRULES) == 2) << endl;
	cout << (rate("**** 2018 ****", test1w1, test1w2, test1dist, TEST1_NRULES) == 0) << endl;
	cout << (rate("  That plot: NEFARIOUS!", test1w1, test1w2, test1dist, TEST1_NRULES) == 1) << endl;
	cout << (rate("deranged deranged robot deranged robot robot", test1w1, test1w2, test1dist, TEST1_NRULES) == 1) << endl;
	cout << (rate("scientist said two mad scientists suffer from deranged-robot fever.", test1w1, test1w2, test1dist, TEST1_NRULES) == 0) << endl;
	cout << (rate("That scient ist  have said two ma d scientist suffer from mad deranged-robot", test1w1, test1w2, test1dist, TEST1_NRULES) == 0) << endl;
}


int makeProper(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int separation[], int nPatterns)
{
	// declare array to mark values that should be deleted
	int flags[100];

	// sets 0 to all values in flags
	for (int i = 0; i < 100; i++) { 
		flags[i] = 0;
	}
	// Treat a negative nPatterns parameter as if it were 0
	if (nPatterns < 0) {
		nPatterns = 0;
	}

	// The function must transform every upper case letter in the pattern words into its lower case equivalent. 
	for (int i = 0; i < nPatterns; i++) {
		for (int j = 0; j < strlen(word1[i]); j++) {
			if (!isalpha(word1[i][j])) {
				flags[i] = 1; // flag
				break;
			}
			word1[i][j] = tolower(word1[i][j]);
		}
		for (int j = 0; j < strlen(word2[i]); j++) {
			if (!isalpha(word2[i][j])) {
				flags[i] = 1; // flag
				break;
			}
			word2[i][j] = tolower(word2[i][j]);
		}
	}

	// The function must remove from the collection every purported pattern for which at least one of these conditions hold: 
	// A word in the pattern contains no characters, or contains a character that is not a letter.
	// The separation is negative.
	for (int i = 0; i < nPatterns; i++) {
		if (strcmp(word1[i], "") == 0 || strcmp(word2[i], "") == 0) {
			flags[i] = 1; // flag
		}
		else if (separation[i] < 0) {
			flags[i] = 1; // flag
		}
	}

	// If two patterns have the same w1 and the same w2 values, in either order, remove the pattern whose separation value is the lesser of the two separation values.
	for (int i = 0; i < nPatterns; i++) {
		for (int j = i; j < nPatterns; j++) {
			if (i != j) {
				if (strcmp(word1[i],word1[j]) == 0 && strcmp(word2[i], word2[j]) == 0) {
					if (separation[i] > separation[j]) { // determine lesser of separation values
						separation[j] = separation[i];
					}
					flags[i] = 1; // flag
				} else if (strcmp(word1[i], word2[j]) == 0 && strcmp(word2[i], word1[j]) == 0) {
					if (separation[i] > separation[j]) { // determine lesser of separation values
						separation[j] = separation[i];
					}
					flags[i] = 1; // flag
				}
			}
		}
	}

	int numFlagged = removeBadElements(word1, word2, separation, nPatterns, flags); // removes flagged values from word1, word2, and separation
	
	// display
	/*
	for (int i = 0; i < nPatterns; i++) {
		cerr << word1[i] << " " << word2[i] << " " << separation[i] << endl;
	}
	*/

	return numFlagged;
}

int rate(const char document[], const char word1[][MAX_WORD_LENGTH + 1], const char word2[][MAX_WORD_LENGTH + 1], const int separation[], int nPatterns)
{
	const int MAX_DOCUMENT_LENGTH = 250;
	char wordsInDocument[MAX_DOCUMENT_LENGTH][MAX_DOCUMENT_LENGTH];
	
	int flags[MAX_DOCUMENT_LENGTH];
	for (int i = 0; i < MAX_DOCUMENT_LENGTH; i++) {
		flags[i] = 1;
	}
	for (int i = 0; document[i] != '\0' ; i++) {
		if ((isalpha(document[i]) || document[i] == ' ')) { // special character
			flags[i] = 0;
		}
	}
	char newDocument[MAX_DOCUMENT_LENGTH];
	removeFlaggedValuesInDocument(document, newDocument, flags, MAX_DOCUMENT_LENGTH);

	int index = 0;
	int currentWord = 0;
	int indexInCurrentWord = 0;
	while (newDocument[index] != '\0') {
		if (newDocument[index] == ' ' && (index > 0 && newDocument[index-1] != ' ')) { // if first space character in a sequence of spaces
			wordsInDocument[currentWord][indexInCurrentWord] = '\0';
			currentWord++; // increment word 
			indexInCurrentWord = 0; // reset wordIndex
		} else if (newDocument[index] != ' '){
			wordsInDocument[currentWord][indexInCurrentWord] = tolower(newDocument[index]); // put the lowercase version of character in next index of particular word
			indexInCurrentWord++; // increment wordIndex
		}
		index++;
	}
	wordsInDocument[currentWord][indexInCurrentWord] = '\0';
	/*
	cerr << endl;
	cerr << "newDocument: " << newDocument << endl;

	// display

	for (int i = 0; i <= currentWord; i++) {
		for (int j = 0; wordsInDocument[i][j] != '\0'; j++) {
			cerr << wordsInDocument[i][j];
		}
		cerr << endl;
	}
	cerr << endl;
	*/


	// rate
	if (nPatterns < 0) {
		nPatterns = 0;
	}

	int numSuccess = 0;
	for (int i = 0; i < nPatterns; i++) { // loop over each pattern
		bool foundSuccess = false;
		for (int j = 0; j <= currentWord; j++) { // loop over words
			if (strcmp(word1[i], wordsInDocument[j]) == 0) { // if word1 appears in the pattern
				int dist = separation[i];
				for (int k = 1; k <= dist + 1; k++) {
					if ((j - k) >= 0 && strcmp(word2[i], wordsInDocument[j - k]) == 0) {
						numSuccess++;
						foundSuccess = true;
					}
					else if ((j + k) <= currentWord && strcmp(word2[i], wordsInDocument[j + k]) == 0) {
						numSuccess++;
						foundSuccess = true;
					}
					if (foundSuccess) {
						break;
					}
				}
			}
			if (foundSuccess) {
				break;
			}
		}
	}
	return numSuccess;
}

int removeBadElements(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int separation[], int size, int flags[])
{
	int count = 0;
	/*
	char empty[10] = "";
	for (int i = 0; i < size; i++) {
		if (flags[i] == 1) {
			count++;
			for (int j = i; j < size-1; j++) {
				strcpy(word1[j], word1[j + 1]);
				strcpy(word2[j], word2[j + 1]);
				separation[j] = separation[j + 1];
			}
			if (size > 0) {
				strcpy(word1[size - 1], empty);
				strcpy(word2[size - 1], empty);
			}
		}
	}

	return size-count;
	*/
	for (int i = 0; i < size; i++) {
		if (flags[i] == 0) {
			count++;
			strcpy(word1[count - 1], word1[i]);
			strcpy(word2[count - 1], word2[i]);
			separation[count-1] = separation[i];
		}
	}
	return count;
}

void removeFlaggedValuesInDocument(const char document[], char newDocument[], int flags[], int n) {
	int nextIndex = 0;
	for (int i = 0; i < n; i++) {
		if (flags[i] == 0) {
			newDocument[nextIndex] = document[i];
			nextIndex++;
		}
	}
	newDocument[nextIndex] = '\0'; 
}