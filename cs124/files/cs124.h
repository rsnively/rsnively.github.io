#ifndef CS124
#define CS124

#include <iostream>
#include <string>
using namespace std;

#define CYTOSINE 0
#define GUANINE  1
#define ADENINE  2
#define THYMINE	 3

string decodeRead(int read);
int encodeRead(string read);
int numDifferences(string s1, string s2);

int numInsertions(string fromRead, string fromGenome, int correctRead);
int numInsertionsR1(string read, string genome);
int numInsertionsR2(string read, string genome);
int numInsertionsR3(string read, string genome);

string int_to_string(int n);
string add_insertion(string r);
int baselineMethod(string genomeFile, string read);

int compareWithInsertions(string g, string r);


#endif // CS124 Included