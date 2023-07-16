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

    if (!input.is_open() || !output.is_open())
    {
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
    bool verbose = false;
    bool preprocess = false;
    std::string input_file;

    // Handle input arguments
    for (auto i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-i")
        {
            if ((i + 1) < argc)
            {
                input_file = argv[i + 1];
            }
        }
        else if (arg == "-v")
        {
            verbose = true;
        }
        else if (arg == "-p")
        {
            preprocess = true;
        }
    }

    if (input_file.empty())
    {
        std::cerr << "Invalid input file. Exit\n";
        return 1;
    }

    if (verbose)
        std::cout << "Count unique words in '" << input_file << "' file\n";

    // Preprocess input file if needed
    if (preprocess)
    {
        const std::string out_filename = "processed.txt";

        if (verbose)
            std::cout << "Preprocess input file... ";

        if (!preprocess_text_file(input_file, out_filename))
        {
            if (verbose)
                std::cout << "Fail\n";

            std::cerr << "Unable to preprocess input file. Exit\n";
            return 1;
        }

        if (verbose)
            std::cout << "Done!\n";

        input_file = out_filename;
    }

    // Count unique words
    int unique_words = count_unique_words(input_file);
    if (unique_words == -1)
    {
        std::cerr << "Failed to open file: " << input_file << ". Exit\n";
        return 1;
    }

    // Output result
    if (verbose)
        std::cout << "Result: " << unique_words << "\n";
    else
        std::cout << unique_words << "\n";

    return 0;
}