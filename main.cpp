#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>

#include "LetterTree.h"

bool preprocess_text_file(const std::string &in_filename, const std::string &out_filename)
{
    std::ifstream input(in_filename);
    std::ofstream output(out_filename);

    if (!input.is_open())
    {
        std::cerr << "Failed to open input file: " << in_filename << std::endl;
        return false;
    }

    if (!output.is_open())
    {
        std::cerr << "Failed to open output file: " << out_filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(input, line))
    {
        for (char &c : line)
        {
            if (std::isalpha(c) || std::isspace(c))
            {
                c = std::tolower(c);
                output << c;
            }
        }
    }

    input.close();
    output.close();

    return true;
}

int count_unique_words(const std::string &filename)
{
    std::ifstream file(filename);
    int count = 0;

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return -1;
    }

    WordStorage word_storage;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word)
        {
            count += word_storage.insert(word);
        }
    }

    file.close();

    return count;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "No enough agruments. Abort\n";
        return 1;
    }

    std::string in_filename(argv[1]);
    std::cout << "Count unique words in '" << in_filename << "' file\n";
    std::string out_filename = "processed.txt";

    if (!preprocess_text_file(in_filename, out_filename))
    {
        return 1;
    }

    int unique_words = count_unique_words(out_filename);
    std::cout << "Result: " << unique_words << "\n";

    return 0;
}