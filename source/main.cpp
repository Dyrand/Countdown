/*
    This file is part of Countdown.

    Countdown is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Countdown is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Countdown.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <cstring>
#include <vector>
#include <utility>
#include <algorithm>
#include <thread>
#include <future>
#include <limits>
#include <cstdint>
#include <map>
#include <exception>

template <typename IterLhs, typename IterRhs>
auto findUnionOf(IterLhs lhsIter, const IterLhs& lhsEnd,
                 const IterRhs rhsBegin, const IterRhs& rhsEnd) {
    std::vector<std::string> result;

    for(; lhsIter != lhsEnd; ++lhsIter) {
        if(std::find(rhsBegin, rhsEnd, *lhsIter) != rhsEnd) {
            result.push_back(*lhsIter);
        }
    }

    return result;
}

const std::map<char,uintmax_t> prime_map{
  {'a',2},{'b',3},{'c',5},{'d',7},{'e',11},{'f',13},{'g',17},{'h',19},
  {'i',23},{'j',29},{'k',31},{'l',37},{'m',41},{'n',43},{'o',47},
  {'p',53},{'q',59},{'r',61},{'s',67},{'t',71},{'u',73},{'v',79},
  {'w',83},{'x',89},{'y',97},{'z',101}
};

const int MAX_PRIME = 101;
const std::size_t PRE_MAX_VALUE = std::numeric_limits<std::size_t>::max()/MAX_PRIME;

int main(int argc, char* argv[]) {
    // program name and word list
    if(argc < 3) {
        std::cout << "Please supply a word to check anagrams of and a newline delimited list of words as a file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    //Print guranteed maximum number of letters for std::size_t
    std::cout << "Guranteed maximum letters for a word: " << int(log(float(std::numeric_limits<std::size_t>::max()))/log(float(MAX_PRIME))) << std::endl;

    //Check for anagram subset flag, -s
    bool subset = false;
    for(int index(0); index < argc && !subset; ++index) {
      if(strcmp(argv[index],"-s")) {
        subset = true;
      }
    }

    const char* const wordlistPath = argv[2];

    std::ifstream wordlistFile(wordlistPath, std::ios_base::in);

    if(! wordlistFile.is_open()) {
        std::cout << "Could not open file " << wordlistPath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string currentWord;
    std::map<std::size_t, std::vector<std::string> > words;

    std::cout << "Loading words..." << std::endl;

    //Calculate values for words
    while(wordlistFile >> currentWord) {
      std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), tolower);
      std::size_t product = 1;
      bool too_long = false;
      for(auto& c : currentWord)
      {
        product *= prime_map.at(c);
        if(product > PRE_MAX_VALUE && &c != &currentWord.back()){
          //std::cout << currentWord << " is too long" << std::endl;
          too_long = true;
          break;
        }
      }

      if(too_long == false) {
        if(words.find(product) == words.end())
        {words.emplace(product,std::vector<std::string>{currentWord});}
        else
        {words.at(product).emplace_back(currentWord);}
      }
    }

    std::cout << words.size() << " words loaded!" << std::endl;

    std::string anagram = argv[1];

    std::cout << "Searching for anagrams of " << anagram << std::endl;

    unsigned int product = 1;
    for(auto& c : anagram)
    {
      product *= prime_map.at(c);
      if(product > PRE_MAX_VALUE && &c != &currentWord.back()){
        std::cout << currentWord << " is too long" << std::endl;
        std::exit(EXIT_FAILURE);
        break;
      }
    }

    if(words.find(product) != words.end()) {
      for(auto& str: words.at(product)) {
        std::cout << "MATCH: " << str << std::endl;
      }
    }
    else {
      std::cout << "No matches found" << std::endl;
    }


    /*
    std::string anagram = argv[1];
    std::transform(anagram.begin(), anagram.end(), anagram.begin(), tolower);

    std::cout << "Searching for anagrams of " << anagram << std::endl;

    std::sort(anagram.begin(), anagram.end());

    std::vector<std::string> anagramPermutations;

    do {
        anagramPermutations.push_back(anagram);
    } while(std::next_permutation(anagram.begin(), anagram.end()));

    std::cout << "There are " << anagramPermutations.size() << " possible words" << std::endl;


    std::vector<std::future<std::vector<std::string>>> futures;

    const int jump = 20000;
    auto wordIter = words.cbegin();

    for(;;) {
        decltype(words)::const_iterator nextEnd;

        if(std::distance(wordIter, words.cend()) <= jump) {
            nextEnd = words.cend();
        } else {
            nextEnd = wordIter + jump;
        }

        futures.emplace_back(std::async(std::launch::async, [&anagramPermutations, wordIter, nextEnd](){
            return findUnionOf(wordIter, nextEnd, anagramPermutations.cbegin(), anagramPermutations.cend());
        }));

        if(nextEnd == words.cend()) {
            break;
        }

        wordIter += jump;
    }

    bool matchFound = false;

    for(auto& future : futures) {
        for(auto& word : future.get()) {
            matchFound = true;
            std::cout << "MATCH: " << word << std::endl;
        }
    }

    if(! matchFound) {
        std::cout << "No matches found" << std::endl;
    }*/
}
