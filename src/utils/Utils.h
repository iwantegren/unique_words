#pragma once

#include "UniqueWordsBase.h"

bool preprocess_text_file(const std::string &in_filename, const std::string &out_filename);

class TreeUniqueWords : public UniqueWords
{
public:
    TreeUniqueWords(const std::string &filename) : UniqueWords(filename){};

    int count() override;
};

class SetUniqueWords : public UniqueWords
{
public:
    SetUniqueWords(const std::string &filename) : UniqueWords(filename){};

    int count() override;
};