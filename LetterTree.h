#pragma once

#include <string>
#include <memory>
#include <unordered_map>

struct TreeElement
{
    bool word_exists = false;
    std::unique_ptr<std::unordered_map<char, TreeElement>> next;
};

using LetterTree = std::unordered_map<char, TreeElement>;
using LetterTreePtr = std::unique_ptr<LetterTree>;

class WordStorage
{
public:
    WordStorage();

    bool insert(const std::string &word);

private:
    bool insert(const std::string &word, size_t letter_pos, LetterTree *tree);

    LetterTreePtr root;
};