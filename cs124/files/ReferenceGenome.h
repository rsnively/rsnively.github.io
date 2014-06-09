#ifndef REFERENCE_GENOME
#define REFERENCE_GENOME

#include <iostream>
#include <vector>
#include "cs124.h"
#include <string>
using namespace std;

/**
 * The ReferenceGenome class is a means of storing the base pairs that make up
 * the human genome. Rather than storing the genome in sequence, a hash must
 * be made to make looking up sequences easier.
 */
class ReferenceGenome {
	public:
		// Constructors & Destructors
		ReferenceGenome();

		// Accessors
		void display();
		long long find(string read);

		// Mutators
		void fill(string filename);

	private:
		// Members
		vector<vector<long long>> index;
		string file; // Eventually this will be replaced by a file, for lookups.

};

#endif // REFERENCE_GENOME defined