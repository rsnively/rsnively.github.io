#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "cs124.h"
#include "ReferenceGenome.h"
using namespace std;

int main() {

	clock_t start = clock();
	ReferenceGenome rg = ReferenceGenome();
	rg.fill("genome.txt");
	srand(time(0));

	int num_correct = 0;
	ofstream of("improvedData.txt");
	double percentage = 0;

	// Make a read.
	for (long long i = 0; i < 1000; i++) {
		int r1 = rand() % 32000;
		int r2 = rand() % 62;
		int r = r1 * r2;
		
		// Get random 30 bps from genome.
		ifstream f("genome.txt");
		if (f.is_open()) {
			char buffer[31];
			f.seekg(r);
			f.read(buffer, 30);
			buffer[30] = '\0';

			string read = buffer;
			read = add_insertion(read);
			read = add_insertion(read);

			
			//long long loc = baselineMethod("genome.txt", read);

			clock_t current = clock();
			of << current - start << '\n';
			cout << i << endl;

			
			int loc = rg.find(read);
			if (loc == r) {
				num_correct++;
			}

			percentage = ((double) num_correct) / ((double) i + 1.0);

			/*
			cout << "Location of " << read << ": " << loc << endl;
			cout << "Real Location: " << r << endl;
			double percentage = ((double) num_correct) / ((double) i+1.0) * 100.0;
			cout << num_correct << " out of " << i << " correct (" << percentage << "%)" << endl;
			clock_t current = clock();
			cout << "Time elapsed: " << current-start << "ms" << endl;
			*/
			
			f.close(); 
		}
		else {
			cout << "ERROR: Unable to open file for find\n";
			return -1;
		}
	}

	of.close();
}