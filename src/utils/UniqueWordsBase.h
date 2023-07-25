#pragma once

#include <string>

class UniqueWords
{
public:
    UniqueWords(const std::string &filename) : filename(filename){};

    virtual int count() = 0;

protected:
    const std::string &filename;
};