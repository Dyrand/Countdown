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
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <map>

/*
const std::map<char,uintmax_t> prime_map{
  {'a',2},{'b',3},{'c',5},{'d',7},{'e',11},{'f',13},{'g',17},{'h',19},
  {'i',23},{'j',29},{'k',31},{'l',37},{'m',41},{'n',43},{'o',47},
  {'p',53},{'q',59},{'r',61},{'s',67},{'t',71},{'u',73},{'v',79},
  {'w',83},{'x',89},{'y',97},{'z',101}
};
*/

const std::map<char,uintmax_t> prime_map{
  {'e',2},{'a',3},{'r',5},{'i',7},{'o',11},{'t',13},{'n',17},{'s',19},
  {'l',23},{'c',29},{'u',31},{'d',37},{'p',41},{'m',43},{'h',47},
  {'g',53},{'b',59},{'f',61},{'y',67},{'w',71},{'k',73},{'v',79},
  {'x',83},{'z',89},{'j',97},{'q',101}
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
    std::cout << "Guaranteed maximum letters for a word: " << int(log(float(std::numeric_limits<std::size_t>::max()))/log(float(MAX_PRIME))) << std::endl;

    //Check for anagram subset flag, -s
    bool subset = false;
    for(int index(0); index < argc && !subset; ++index) {
      if(strcmp(argv[index],"-s") == 0) {
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
    std::size_t unloadable = 0;
    while(wordlistFile >> currentWord) {
      std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), tolower);
      std::size_t product = 1;
      bool too_long = false;
      bool potential_overflow = false;
      for(auto& c : currentWord)
      {
        product *= prime_map.at(c);
        if((product > PRE_MAX_VALUE && &c != &currentWord.back()) || potential_overflow) {
          //In the range of a possible overflow
          potential_overflow = true;
          if(product/c < PRE_MAX_VALUE) {
            //std::cout << currentWord << " is too long" << std::endl;
            ++unloadable;
            too_long = true;
            break;
          }
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
    std::cout << unloadable << " words not loaded!" << std::endl;

    std::string anagram = argv[1];

    std::cout << "Searching for anagrams of " << anagram << std::endl;

    std::size_t product = 1;
    bool potential_overflow = false;
    for(auto& c : anagram)
    {
      product *= prime_map.at(c);
      if((product > PRE_MAX_VALUE && &c != &currentWord.back()) || potential_overflow) {
        //In the range of a possible overflow
        if(product/c < PRE_MAX_VALUE && potential_overflow) {
          std::cout << anagram << " is too long" << std::endl;
          std::exit(EXIT_FAILURE);
        }
        potential_overflow = true;
      }
    }

    if(subset) {
      bool match = false;
      for(auto& pair : words) {
        if(product % pair.first == 0) {
          match = true;
          for(auto& str: pair.second) {std::cout << "MATCH: " << str << std::endl;}
        }
      }
      if(!match)
      {std::cout << "No matches found" << std::endl;}
    }
    else {
      if(words.find(product) != words.end()) {
        for(auto& str: words.at(product)) {std::cout << "MATCH: " << str << std::endl;}
      }
      else {
        std::cout << "No matches found" << std::endl;
      }
    }
}
