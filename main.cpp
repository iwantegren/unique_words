#include <iostream>

#include "Utils.h"

int main(int argc, char **argv)
{
    bool verbose = false;
    bool preprocess = false;
    bool standart_impl = false;
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
        else if (arg == "-s")
        {
            standart_impl = true;
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
    int unique_words;
    if (standart_impl)
        unique_words = count_unique_words_set(input_file);
    else
        unique_words = count_unique_words(input_file);

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