#include "cs124.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string decodeRead(int read) {
	if (read < 0 || read >= pow(4.0, 10)) {
		return "XXXXXXXXXX";
	}

	string dec = "";

	for (int i = 0; i < 10; i++) {
		switch (read % 4) {
			case CYTOSINE: dec = 'C' + dec; break;
			case GUANINE: dec = 'G' + dec; break;
			case ADENINE: dec = 'A' + dec; break;
			case THYMINE: dec = 'T' + dec; break;
			default: return "XXXXXXXXXX";
		}
		read /= 4;
	}
	return dec;
}

int encodeRead(string read) {
	if (read.length() != 10) {
		return -1;
	}

	int enc = 0;

	for (int i = 0; i < 10; i++) {
		enc *= 4;
		switch(read[i]) {
			case 'C': enc += CYTOSINE; break;
			case 'G': enc += GUANINE; break;
			case 'A': enc += ADENINE; break;
			case 'T': enc += THYMINE; break;
			default: return -1;
		}
	}

	return enc;
}

// Return the number of characters different between strings s1 and s2 (should
// only be used on strings of same length, only considers first characters if
// they differ).
int numDifferences(string s1, string s2) {
	int count = 0;

	for (int i = 0; i < s1.length() && i < s2.length(); i++) {
		if (s1[i] != s2[i]) {
			count++;
		}
	}
	return count;
}

// Returns how many insertions must have taken place for these two strings to
// be in the same location in the genome. It is important to distinguish which
// string is the read (because this is the one in which we will allow
// insertions).
int numInsertions(string fromRead, string fromGenome, int correctRead) {
	if (fromRead.length() != 30 || fromGenome.length() != 30) {
		cout << "ERROR: Sequences not of expected length\n";
		return -1;
	}

	switch (correctRead) {
		case 0: return numInsertionsR1(fromRead, fromGenome);
		case 1: return numInsertionsR2(fromRead, fromGenome);
		case 2: return numInsertionsR3(fromRead, fromGenome);
	}

	cout << "ERROR: Unknown sub-read\n";
	return -1;
}

// Given that there are no insertions in the first sub-read, how many
// would there have to be in the other portion of the read for this
// to be its corresponding genome location?
int numInsertionsR1(string read, string genome) {

	int count = 0;

	for (int i = 10; i < 30; i++) {
		if (read[i] != genome[i-count]) {
			count++;
		}
	}

	return count;
}

// Given second sub-read has no insertions, how many are in the rest?
int numInsertionsR2(string read, string genome) {

	int frontCount = 0;
	int backCount = 0;

	for (int i = 9; i >= 0; i--) {
		if (read[i] != genome[i+frontCount]) {
			frontCount++;
		}
	}

	for (int i = 20; i < 30; i++) {
		if (read[i] != genome[i-backCount]) {
			backCount++;
		}
	}
	return frontCount + backCount;
}

// Given third sub-read has no insertions, how many are in the rest?
int numInsertionsR3(string read, string genome) {

	int count = 0;

	for (int i = 19; i >= 0; i--) {
		if (read[i] != genome[i+count]) {
			count++;
		}
	}

	return count;
}

string int_to_string(int n) {
	if (n < 10) {
		char c = '0' + n;
		return string(1, c);
	}

	int d1 = n / 10;
	int d2 = n % 10;

	char c1 = '0' + d1;
	char c2 = '0' + d2;

	string s1 = string(1, d1);
	string s2 = string(1, d2);

	return s1 + s2;
}

string add_insertion(string r) {

	int ins = rand() % 30;
	int bp = rand() % 4;

	for (int i = 29; i > ins; i--) {
		r[i] = r[i-1];
	}

	switch(bp) {
		case 0: r[ins] = 'C'; break;
		case 1: r[ins] = 'G'; break;
		case 2: r[ins] = 'A'; break;
		case 3: r[ins] = 'T'; break;
		default: break;
	}

	return r.substr(0, 30);
}

int baselineMethod(string genomeFile, string read) {
	
	ifstream f(genomeFile);
	if (f.is_open()) {
		char c;
		string ref_read = "";
		int fileLoc = 0;
		while (!f.eof()) {
			c = f.get();
			if (c != 'A' && c != 'C' && c != 'T' && c != 'G') {
				continue;
			}
			ref_read += c;
			if (ref_read.length() > 30) {
				ref_read = ref_read.substr(1, 30);
			}
			if (ref_read.length() == 30) {
				if (compareWithInsertions(ref_read, read) <= 2) {
					return fileLoc-29;
				}
			}

			fileLoc++;
		}

		f.close();
	}
	else {
		cout << " ERROR opening file for baseline method.\n";
	}

	return -1;
}

int compareWithInsertions(string g, string r) {
	int insertions = 0;
	
	for (int i = 0; i < 30; i++) {
		if (r[i] != g[i - insertions]) {
			insertions++;
		}
	}

	return insertions;
}