#include "Utils.h"

#include <fstream>
#include <sstream>
#include <unordered_set>

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

int TreeUniqueWords::count()
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

int SetUniqueWords::count()
{
    std::ifstream file(filename);
    int count = 0;

    if (!file.is_open())
    {
        return -1;
    }

    std::unordered_set<std::string> set;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word)
        {
            count += set.emplace(word).second;
        }
    }

    file.close();

    return count;
}
