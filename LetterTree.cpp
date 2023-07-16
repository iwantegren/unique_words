#include "LetterTree.h"

WordStorage::WordStorage()
{
    root = std::make_unique<LetterTree>();
}

bool WordStorage::insert(const std::string &word)
{
    if (word.empty())
        return false;

    return insert(word, 0, root.get());
}

bool WordStorage::insert(const std::string &word, size_t letter_pos, LetterTree *tree)
{
    const char letter = word[letter_pos];
    const bool is_last_letter = (letter_pos == word.size() - 1);

    // emplace returns existing element or newly created
    auto letter_iter = tree->emplace(letter, TreeElement()).first;
    TreeElement &tree_el = letter_iter->second;

    if (is_last_letter)
    {
        if (tree_el.word_exists) // this word already exists
            return false;

        tree_el.word_exists = true;
        return true;
    }

    if (tree_el.next == nullptr)
    {
        tree_el.next = std::make_unique<LetterTree>();
    }

    return insert(word, letter_pos + 1, tree_el.next.get());
}