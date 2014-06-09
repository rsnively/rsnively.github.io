#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ReferenceGenome.h"
#include "cs124.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//						CONSTRUCTORS & DESTRUCTORS							 //
///////////////////////////////////////////////////////////////////////////////

/**
 * Default Constructor
 *
 * Initializes an index of base pair sequences of size 4^10 (to accomodate
 * all possible sequences of size 10). At each entry of the index, there is
 * a list of the locations of its corresponding sequence (initially empty,
 * then loaded with values as the reference genome is given).
 */
ReferenceGenome::ReferenceGenome() {
	int index_length = pow(4.0, 10);
	index = vector<vector<long long>>(index_length, vector<long long>());
}


///////////////////////////////////////////////////////////////////////////////
//									ACCESSORS								 //
///////////////////////////////////////////////////////////////////////////////

/**
 * Displays the contents of the index, mostly just to make sure it is working.
 */
void ReferenceGenome::display() {
	for (vector<vector<long long>>::iterator it = index.begin(); it != index.end(); it++) {
		if (! it->empty()) {
			cout << "READ: " << decodeRead(it - index.begin()) << endl;
			for (vector<long long>::iterator jt = it->begin(); jt != it->end(); jt++) {
					cout << "    " << *jt << endl;
			}
		}
	}
}

/**
 * Given a sequence read, finds its probable location within the genome.
 */
long long ReferenceGenome::find(string read) {
	
	// Make sure the given read has size 30.
	if (read.size() != 30) {
		return -1;
	}

	// For each of the 3 sub-reads we will be examining...
	for (int r = 0; r < 3; r++) {
		// Get the locations of each sub-read, and for each such location...
		vector<long long> v = index[encodeRead(read.substr(r*10, 10))];
		for (vector<long long>::iterator it = v.begin(); it != v.end(); it++) {

			// Determine where the read would have to start if this was the
			// location of the sub-read, if this is less than zero, then skip
			// this one.
			long long startLoc = (*it) - 10*r;
			if (startLoc < 0) {
				continue;
			}

			// Open the file and extract the read at this location.
			ifstream f(file);
			if (f.is_open()) {
				char buffer[31];
				f.seekg(startLoc);
				f.read(buffer, 30);
				buffer[30] = '\0';

				if (f.gcount() == 30 && numInsertions(read, buffer, r) <=2) {

					// If we are in the first sub-read, then this is the starting
					// location.
					if (r == 0) {
						return startLoc;
					}

					// If we are in the second sub-read...
					if (r == 1) {
						// If both of the insertions occurred in the first sub-read, then
						// shift our answer by two.
						int r1_insertions = 0;

						for (int i = 9; i >= 0; i--) {
							if (buffer[i+r1_insertions] != read[i]) {
								r1_insertions++;
							}
						}

						return startLoc + r1_insertions;
					}

					// If we are in the third sub-read, then we know two insertions occured,
					// so we shift our answer by two.
					else if (r == 2) {
						return startLoc + 2;
					}
				}
				f.close();
			}
			else {
				cout << "ERROR: Unable to open file for find\n";
				return -1;
			}
		}
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//									MUTATORS								 //
///////////////////////////////////////////////////////////////////////////////

/**
 * Fill our reference genome into the index.
 */
void ReferenceGenome::fill(string filename) {

	file = filename;
	
	ifstream f(filename);
	if (f.is_open()) {

		string read = "";
		long long fileLoc = 0;
		char c;

		while (!f.eof()) {
			c = f.get();
			if (c != 'A' && c != 'C' && c != 'T' && c != 'G') {
				continue;
			}
			read += c;
			if (read.length() > 10) {
				read = read.substr(1, 10);
			}
			if (read.length() == 10) {
				index.at(encodeRead(read)).push_back(fileLoc-9);
			}
			fileLoc++;
			if (fileLoc % 100000 == 0) {
				cout << fileLoc << ": " << read << endl;
			}
		}
		f.close();
	}
	else {
		cout << "Unable to open genome file: " << filename << endl;
	}
}

