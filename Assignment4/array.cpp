#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int divide(string a[], int n, string divider);
void bubbleSort(string a[], int n);
void swap(string& a, string& b);

int main()
{
	string h[7] = { "greg", "gavin", "ed", "xavier", "", "eleni", "fiona" };
	assert(lookup(h, 7, "eleni") == 5);
	assert(lookup(h, 7, "ed") == 2);
	assert(lookup(h, 2, "ed") == -1);
	assert(positionOfMax(h, 7) == 3);

	string g[4] = { "greg", "gavin", "fiona", "kevin" };
	assert(differ(h, 4, g, 4) == 2);
	assert(appendToAll(g, 4, "?") == 4 && g[0] == "greg?" && g[3] == "kevin?");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "fiona?" && g[3] == "gavin?");
	
	
	string e[4] = { "ed", "xavier", "", "eleni" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "gavin", "gavin", "gavin", "xavier", "xavier" };
	assert(countRuns(d, 5) == 2);
	
	string f[3] = { "fiona", "ed", "john" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "john" && f[2] == "fiona");

	assert(divide(h, 7, "fiona") == 3);

	string people[5] = { "dianne", "fiona", "ed", "xavier", "greg" };
	assert(lookup(people, 3, "greg") == -1);  // should return -1 (not found)

	assert(appendToAll(people, 5, "!!!")==5);  // returns 5

	string candidate[6] = { "dianne", "fiona", "gavin", "xavier", "ed", "betty" };
	assert(positionOfMax(candidate, 6)==3);

	string politician[5] = { "eleni", "dianne", "fiona", "kevin", "gavin" };
	assert(rotateLeft(politician, 5, 1)==1);  // returns 1

	string de[9] = {"xavier", "betty", "john", "john", "ed", "ed", "ed", "john", "john"};
	assert(countRuns(de, 9)==5);  //  returns 5

	string folks[6] = { "betty", "john", "", "xavier", "kevin", "dianne" };
	string group[5] = { "betty", "john", "dianne", "", "xavier" };
	assert(differ(folks, 6, group, 5) == 2);  //  returns 2
	assert(differ(folks, 2, group, 1) == 1);  //  returns 1

	assert(flip(folks, 4) == 4);  // returns 4

	string names[10] = { "eleni", "gavin", "kevin", "greg", "betty", "fiona" };
	string names1[10] = { "gavin", "kevin", "greg" };
	assert(subsequence(names, 6, names1, 3)==1);  // returns 1
	string names2[10] = { "eleni", "greg" };
	assert(subsequence(names, 5, names2, 2)== -1);  // returns -1

	string set1[10] = { "dianne", "betty", "greg", "gavin" };
	assert(lookupAny(names, 6, set1, 4) == 1);  // returns 1 (a1 has "gavin" there)
	string set2[10] = { "xavier", "ed" };
	assert(lookupAny(names, 6, set2, 2) == -1);  // returns -1 (a1 has none)

	assert(divide(candidate, 6, "eleni") == 3);  //  returns 3
	string candidate2[4] = { "gavin", "kevin", "fiona", "john" };
	assert(divide(candidate2, 4, "john") == 2);  //  returns 2

	cout << "All tests succeeded" << endl;
}

int appendToAll(string a[], int n, string value) {
	// if bad n value passed in 
	if (n < 0) {
		return -1;
	}
	for (int i = 0; i < n; i++) {
		a[i] += value;
	}
	return n;
}

int lookup(const string a[], int n, string target) {
	// if bad n value passed in 
	if (n < 0) {
		return -1;
	}
	for (int i = 0; i < n; i++) {
		if (a[i] == target) {
			return i;
		}
	}
	return -1;
}

int positionOfMax(const string a[], int n) {
	string max = "";
	int indexOfMax = 0;
	// if bad n value passed in or no interesting elements
	if (n <= 0) {
		return -1;
	}
	for (int i = 0; i < n; i++) {
		if (a[i] > max) {
			max = a[i];
			indexOfMax = i;
		}
	}
	return indexOfMax;
}

int rotateLeft(string a[], int n, int pos) {
	// if bad n value passed in 
	if (n < 0) {
		return -1;
	}
	// if bad pos value passed in
	if (pos >= n) {
		return -1;
	}
	string removedItem = a[pos];
	for (int i = pos; i < n-1; i++) {
		a[i] = a[i + 1];
	}
	a[n - 1] = removedItem;
	return pos;
}

int countRuns(const string a[], int n) {
	// if bad n value passed in 
	if (n < 0) {
		return -1;
	}
	int i = 1;
	int runs = n;
	while (i < n) {
		if (a[i] == a[i - 1]) { // means there is a run here
			runs--;
		}
		i++;
	};
	return runs;
}

int flip(string a[], int n) { 
	// if bad n value passed in 
	if (n < 0) {
		return -1;
	}
	for (int i = 0; i < n / 2; i++) {
		// swap values
		string temp = a[i];
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2) {
	// if bad n value passed in for either array
	if (n1 < 0 || n2 < 0) {
		return -1;
	}
	for (int i = 0; i < n1 && i < n2; i++) {
		if (a1[i] != a2[i]) {
			return i;
		}
	}
	// return the smaller value of n1 and n2
	if (n1 < n2) {
		return n1;
	}
	else {
		return n2;
	}
}

int subsequence(const string a1[], int n1, const string a2[], int n2) {
	// if bad n value passed in for either array
	if (n1 < 0 || n2 < 0) {
		return -1;
	}
	// dealing with the zero case
	if (n2 == 0) {
		return 0;
	}
	for (int i = 0; i < n1-n2; i++) {
		if (a1[i] == a2[0]) { // find matching first letter
			int counter = 0;
			for (int j = 0; j < n2; j++) { // loop until length of potential subsequence
				if (a1[i + j] == a2[j]) {
					counter++;
				}
				if (counter == n2) { // if num matching is the length of potential subsequence
					return i;
				}
			}
		}
	}
	return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2) {
	// if bad n value passed in for either array
	if (n1 < 0 || n2 < 0) {
		return -1;
	}
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < n2; j++) {
			if (a1[i] == a2[j]) {
				return i;
			}
		}
	}
	return -1;
}

int divide(string a[], int n, string divider) {
	// if bad n value passed in
	if (n < 0) {
		return -1;
	}
	// sort array 
	bubbleSort(a, n);
	// find position of divider
	for (int i = 0; i < n; i++) {
		if (a[i] >= divider) {
			return i;
		}
	}
	return n;
}

void bubbleSort(string a[], int n) // helper method to sort array
{
	int i, j;
	bool swapped;
	for (i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				swap(a[j], a[j + 1]);
				swapped = true;
			}
		}

		if (swapped == false)
			break;
	}
}

void swap(string& a, string& b) // helper method used in bubble sort
{
	string temp = a;
	a = b;
	b = temp;
}
