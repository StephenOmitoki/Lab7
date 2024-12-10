/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <tuple>
#include <queue>
#include <string>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

// aliases for data types
using WORD = string;
using WORD_LIST = set<WORD>;
using WORD_FREQ_LIST = map<char, int>;
using FREQ_WORD_LIST = multimap<int, WORD_LIST>;
using COUNTER = tuple<int, int, int>;
using LONGEST = priority_queue<WORD>;

// Struct to store all text data and stats
struct DICTION {
    COUNTER stats; // tuple to keep counts for letters, words, and lines
    WORD_LIST words; // set to store unique words(no duplicates)
    WORD_FREQ_LIST wordFreq; // Tracks frequency of each letter, key is a character and value is the frequency of that character
    map<WORD, int> wordFrequency; // Tracks frequency of each word
    LONGEST longWord; // priority queue to store words by length (longest at the top)
};

// Function to get the next word from input
WORD getNextWord(istream& in) {
    WORD w;
    char ch;
    bool inWord = false; // Tracks if we're inside a word
    while (in.get(ch)) { // Read character by character
        if (isalpha(ch)) { // Check if character is a letter
            w.push_back(ch);
            inWord = true; // We're inside a word
        } else if (inWord) {
            return w; // Return word when it's done
        }
    }
    return w; // Return the last word
}

// Function to process the text file and collect stats
void processText(DICTION& d, istream& in) {
    // Initialize letter frequencies
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        d.wordFreq[ch] = 0;
        d.wordFreq[char(toupper(ch))] = 0;
    }

    string line;
    while (getline(in, line)) { // Read file line by line
        get<2>(d.stats)++; // Count lines
        istringstream lineStream(line);
        WORD word;
        while ((word = getNextWord(lineStream)) != "") { // Extract words
            get<1>(d.stats)++; // Count words
            d.words.insert(word); // Add to unique word list
            d.wordFrequency[word]++; // Track word frequency
            d.longWord.push(word); // Add word to priority queue
            for (char c : word) { // Process each letter in the word
                d.wordFreq[c]++;
                get<0>(d.stats)++; // Count letters
            }
        }
    }
}

// Function to display the results
void displayResults(const DICTION& d) {
    // Show text statistics
    cout << "/--------------\\\n";
    cout << "| Text Stats   |\n";
    cout << "\\--------------/\n";
    cout << "Number of Letters: " << get<0>(d.stats) << endl;
    cout << "Number of Words: " << get<1>(d.stats) << endl;
    cout << "Number of Lines: " << get<2>(d.stats) << endl;

    // Display letter frequencies
    cout << "\n/--------------\\\n";
    cout << "| Letter Freq  |\n";
    cout << "\\--------------/\n";
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        char upper = toupper(ch);
        cout << "[" << upper << "]" << " | ";
        for (int i = 0; i < min(d.wordFreq.at(upper), 10); ++i) cout << '*';
        if (d.wordFreq.at(upper) > 10) cout << " (" << (d.wordFreq.at(upper) - 10) << ")";
        cout << endl;
    }
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        cout << "[" << ch << "]" << " | ";
        for (int i = 0; i < min(d.wordFreq.at(ch), 10); ++i) cout << '*';
        if (d.wordFreq.at(ch) > 10) cout << " (" << (d.wordFreq.at(ch) - 10) << ")";
        cout << endl;
    }

    // Display dictionary (unique words)
    cout << "\n/--------------\\\n";
    cout << "| Dictionary   |\n";
    cout << "\\--------------/\n";
    cout << "Words in dictionary:\n";
    for (const auto& word : d.words) {
        cout << word << " ";
    }

    // Show word frequencies
    cout << "\n\nWord                               Frequencies:\n";
    cout << "----------------------------------------------------\n";
    vector<pair<int, WORD>> sortedWords;
    for (const auto& [word, freq] : d.wordFrequency) {
        sortedWords.push_back({ freq, word });
    }
    sort(sortedWords.begin(), sortedWords.end(), [](const pair<int, WORD>& a, const pair<int, WORD>& b) {
        if (a.second.length() == b.second.length())
            return a.second < b.second;
        return a.second.length() < b.second.length();
    });
    for (const auto& [freq, word] : sortedWords) {
        cout << std::setw(35) << std::left << word << ": " << freq << '\n';
    }

    // Display histogram
    cout << "\n/--------------\\\n";
    cout << "| Histogram    |\n";
    cout << "\\--------------/\n";
    int maxFreq = 0;
    for (const auto& [freq, word] : sortedWords) {
        maxFreq = max(maxFreq, freq);
    }
    for (int level = maxFreq; level > 0; --level) {
        for (const auto& [freq, word] : sortedWords) {
            if (freq >= level)
                cout << " " << std::right << "*";
            else
                cout << " " << std::right << " ";
        }
        cout << endl;
    }
    cout << " ----------------------------------------\n\n";
    for (auto& [freq, word] : sortedWords) {
        cout << std::left << word << "------/";
        cout << endl;
    }
}

int main() {
    DICTION dict; // Initialize struct to store all data
    ifstream file("data.txt"); // Open input file
    if (!file.is_open()) { // Check if the file was opened successfully
        cerr << "Error opening file!" << endl;
        return 1;
    }
    processText(dict, file); // Process the text file
    displayResults(dict); // Show results
    return 0;
}
