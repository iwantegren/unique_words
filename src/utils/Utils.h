#pragma once

#include <string>

bool preprocess_text_file(const std::string &in_filename, const std::string &out_filename);

int count_unique_words(const std::string &filename);
int count_unique_words_set(const std::string &filename);