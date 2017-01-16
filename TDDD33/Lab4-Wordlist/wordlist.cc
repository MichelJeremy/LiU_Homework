#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <map>
#include <iomanip>
#include <limits>
#include <sstream>
#include <fstream>

using namespace std;

// Note: import <utlity> for pair-types.

vector<char> leadingJunk {'"', '\'', '('};
vector<char> trailingJunk {'!', '?', ';', ',', ':', '.', '"', '\''};

/*
 * CHANGE LOGS:
 * load_words function was modified to use only CTL containers.
 * Now, it only copies evey words into a vector<string> and returns it.
 * The transform call to put every word to lower case was moved to the cleanWords
 * function object, which is more convenient as this function accesses directly the different
 * strings
 */

// load words separated by a space into a vector<string>
vector<string> load_words(string file) {
	ifstream filestream(file);
	vector<string> vec;
	vector<string> vec2;
	copy(istream_iterator<string>(filestream), istream_iterator<string>(), back_inserter(vec));
	return vec;
}

// print vector<string> content
void print_vector (vector<string> v) {
	for (vector<string>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it  << endl;
		}
}

// print usage error messages
void usage_print(string arg){
	cerr <<"Usage:"<< arg << " FILE [-a] [-f] [-o N]"<<endl;
}

// print error messages given error code
void error_print (int i){
	if (i == 1) {
		cerr <<"Error: No arguments given." << endl;
	} else if (i == 2)
	 	cerr <<"Error: Second argument missing or invalid." << endl;
	else if (i == 3) {
	 	cerr <<"Error: could not open the given file." << endl;
	 	cerr << "Usage: ./a.out FILE [-a] [-f] [-o N]" << endl;
	} else if (i == 4) {
	 	cerr <<"Error: the third parameter must be a number " << endl;
	 	cerr << "Usage: ./a.out FILE [-a] [-f] [-o N]" << endl;
	} else if (i == 5) {
		cerr <<"Error: You  must add a number as third parameter " << endl;
		cerr << "Usage: ./a.out FILE [-a] [-f] [-o N]" << endl;
	}
}

// check if file can be accessed
bool checkfile( string filename){
	ifstream filestream(filename.c_str()); // try to open file stream, cast into pointer
	if (filestream) return true;
	else return false;
  //(filestream) ? return true : return false;
}

// print pair<string, int>
void printPair(const pair<string, int> &p, int &max_length) {
  cout << p.first << " " << setw(max_length - p.first.length() + 1) << p.second << endl;
}

// function object, clean the words
string cleanWord(string &word) {
  vector<char>::iterator it;
  it = find(leadingJunk.begin(), leadingJunk.end(), word.at(0));
  if (it != leadingJunk.end()) {
    word.erase(0, 1);
  }

  it = find(trailingJunk.begin(), trailingJunk.end(), word.back());
  if (it != trailingJunk.end()) {
    word.pop_back();
  }

  // remove 's if length is at least 2 to avoid out of range exceptions
  if (word.length() >= 2) {
    if (word.back() == 's') {
      if (word.at(word.length() - 2) == '\'') {
        word.pop_back();
        word.pop_back();
      }
    }
  }
	// transform words to lower case before returning it
	transform(word.begin(), word.end(), word.begin(), ::tolower);
  return word;
}

// function object, add the valid words into a new vector<string>
void validWord(string word, vector<string> &validWords) {
  string::iterator it = word.begin();
  int doublehyphens = 1;
  // check that characters are a-z 0-9 A-Z and hyphens
  if (regex_match(word, regex("^[A-Za-z0-9-]+"))) {
    // first and alst char not hyphens
    if ((word.at(0) != '-') && (word.back() != '-')) {
      //cout << word << endl;
      while ((it = adjacent_find(it, word.end())) != word.end()) {
        char currentVal = *it;
        while (it != word.end() && *it == currentVal) {
          if (*it == '-') doublehyphens = 0;
          if (it < word.end())  ++it;
        }
      }
      // doublehyphens = 1 <=> no adjacent hyphens
      if (doublehyphens == 1) {
        // check length at least 3
        if (word.length() >= 3) {
          // word is valid, add it to valid words
          validWords.push_back(word);
          //cout << word << " added" << endl;
        }
			}
		}
	}
}

// -a (1/3)
// function object, add the words in a map<string, int>
void alphabetic_increasing(string word, map<string, int> &frequencies, int &max_length) {
  map<string, int>::iterator it;
  it = frequencies.find(word);
  if (word.length() > max_length) max_length = word.length();
  // if element was not present
  if (it == frequencies.end()) frequencies[word] = 1;
  else {
    // get old value
    int old_value = frequencies.find(word)->second;
    frequencies[word] = old_value + 1;
  }
}

// -f 2/3
// it is done in the main directly, not in a function

// -o 3/3
// function object, print the words in the order they arrived using valdWords
// if line size is going to become greater than N, print on a new line
void printSameOrder(string word, int N, int &current_size) {
  if (current_size + word.length() >= N ) {
    cout << "\n" << word << " ";
    current_size = word.length() + 1;
  } else {
    cout << word << " ";
    current_size += word.length() + 1;
  }
}


int main(int argc, char* argv[]) {
  int current_size = 0;
  int N;

  string filename; // name of the test file
	vector<string> wordList; // list all words of the file
	vector<string> validWords; // contains list of all valid words
	map<string, int> frequencies; // contains words with frequency
	int max_length; // current max length of the line
  string param;


	if ((argc != 3) && (argc != 4)) {
			error_print(argc);
			usage_print(argv[0]);
			return 0;
	} else if (argc == 3) {
		if(checkfile(argv[1])){
			param = argv[2];
			if (param.compare("-a") == 0) {
        //load words
        wordList = load_words(argv[1]);
        //first, clean the words
        for_each(wordList.begin(), wordList.end(), cleanWord);
        // get the valid words, put them into a new vector
        for_each(wordList.begin(), wordList.end(),
								[&](string word) {
								validWord(word, validWords);
				});
        // // map for -a
        for_each(validWords.begin(), validWords.end(),
				 				[&](string word) {
									alphabetic_increasing(word, frequencies, max_length);
								});
        //print map
        for_each(frequencies.begin(), frequencies.end(),
								[&](const pair<string, int> p) {
									printPair(p, max_length);
								});
			} else if (param.compare("-f") == 0) {
        //load words
        wordList = load_words(argv[1]);
        //first, clean the words
        for_each(wordList.begin(), wordList.end(), cleanWord);
        // get the valid words, put them into a new vector
        for_each(wordList.begin(), wordList.end(),
								[&](string word) {
									validWord(word, validWords);
				});
        // map for -a
        for_each(validWords.begin(), validWords.end(),
				 				[&](string word) {
									alphabetic_increasing(word, frequencies, max_length);
								});
        // put map into vector<pair>
        vector<pair<string, int>> decreasing_values(frequencies.begin(), frequencies.end());
        // Sort the vector according to the word count in descending order.
        sort(decreasing_values.begin(), decreasing_values.end(),
              [](const pair<string, int> &one, const pair<string, int> &two) {
                return one.second > two.second;});
        for_each(decreasing_values.begin(), decreasing_values.end(),
								[&](const pair<string, int> p) {
									printPair(p, max_length);
								});
			} else if (param.compare("-o") == 0) {
				error_print(5);
				return 0;
			} else {
					error_print(2);
					usage_print(argv[0]);
					return 0;
			}
		} else {
			error_print(3);
			return 0;
		}
	}
	else { /* argc == 4*/
		if(checkfile(argv[1])) {
			param = argv[2];
			if (param.compare("-o") == 0) {
				param = argv[3];
				if (!(atoi(param.c_str()))) {
					error_print(4);
					return 0;
				}
        //load words
        wordList = load_words(argv[1]);
        //first, clean the words
        for_each(wordList.begin(), wordList.end(), cleanWord);
        // get the valid words, put them into a new vector
        for_each(wordList.begin(), wordList.end(),
								[&](string word) {
								validWord(word, validWords);
				});
        // print words in same order
        for_each(validWords.begin(), validWords.end(),
                [&](string word) {
                printSameOrder(word, stoi(param), current_size);
                });
        cout << "\n";
			} else {
				error_print(2);
				return 0;
			}
		} else {
			error_print(3);
			return 0;
		}
	}
  return 0;
}
