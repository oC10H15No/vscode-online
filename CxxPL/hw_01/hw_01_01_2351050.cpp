#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
Problem 1:
Create a program that counts the occurrence of the word 'that' in a file (use the string
class' operator '==' to find the word)
*/

// Function to clean a word by removing punctuation and converting to lowercase
string clean_word(const string& word) {
    string cleaned;
    
    for (char c : word) {
        // Check if the character is not punctuation
        if (!(c == '.' || c == ',' || c == ';' || c == ':' || c == '!' ||
              c == '?' || c == '(' || c == ')' || c == '[' || c == ']' ||
              c == '{' || c == '}' || c == '"' || c == '\'' || c == '-' || c == '/')) {
            
            // Convert uppercase letters to lowercase
            if (c >= 'A' && c <= 'Z') {
                c = c + ('a' - 'A');
            }
            
            // Append the cleaned character to the result string
            cleaned += c;
        }
    }
    
    return cleaned;
}

int main() 
{
    string targetWord = "that";

    ifstream file;
    file.open("input.txt");  // Replace "input.txt" with the actual filename
    if (!file.is_open()) {
        cerr << "File could not be opened!" << std::endl;
        exit(1);
    }
    
    string word;
    int that_count = 0;
    while (file >> word) {
        string cleaned = clean_word(word);
        if (cleaned == targetWord) { //use the string class' operator '==' to find the word
            that_count++;
        }
    }
    cout << "The word 'that' appears " << that_count << " times in the file." << endl;

    return 0;
}