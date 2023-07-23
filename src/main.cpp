#include <iostream>

#include "utils/Utils.h"
#include "concurrency/ConcurrencyUtils.h"

struct Settings
{
    bool verbose = false;
    bool preprocess = false;
    bool standard_impl = false;
    bool no_concurrency = false;
    std::string input_file;
};

int main(int argc, char **argv)
{
    {
        auto rq = Concurrency::make_range_queues(10);

        for (const auto &pair : *rq.get())
        {
            // std::cout << pair.first << " - " << pair.second->front() << "\n";
        }

        return 0;
    }

    Settings s;

    // Handle input arguments
    for (auto i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-i")
        {
            if ((i + 1) < argc)
            {
                s.input_file = argv[i + 1];
            }
        }
        else if (arg == "-v")
        {
            s.verbose = true;
        }
        else if (arg == "-p")
        {
            s.preprocess = true;
        }
        else if (arg == "-s")
        {
            s.standard_impl = true;
        }
        else if (arg == "-nc")
        {
            s.no_concurrency = true;
        }
    }

    if (s.input_file.empty())
    {
        std::cerr << "Invalid input file. Exit\n";
        return 1;
    }

    if (s.verbose)
        std::cout << "Count unique words in '" << s.input_file << "' file\n";

    // Preprocess input file if needed
    if (s.preprocess)
    {
        const std::string out_filename = "processed.txt";

        if (s.verbose)
            std::cout << "Preprocess input file... ";

        if (!preprocess_text_file(s.input_file, out_filename))
        {
            if (s.verbose)
                std::cout << "Fail\n";

            std::cerr << "Unable to preprocess input file. Exit\n";
            return 1;
        }

        if (s.verbose)
            std::cout << "Done!\n";

        s.input_file = out_filename;
    }

    // Count unique words
    int unique_words = -1;
    if (s.no_concurrency)
    {
        if (s.standard_impl)
            unique_words = count_unique_words_set(s.input_file);
        else
            unique_words = count_unique_words(s.input_file);
    }
    else
    {
        unique_words = 100;
        std::cout << "running concurrency...\n";
    }

    if (unique_words == -1)
    {
        std::cerr << "Failed to open file: " << s.input_file << ". Exit\n";
        return 1;
    }

    // Output result
    if (s.verbose)
        std::cout << "Result: " << unique_words << "\n";
    else
        std::cout << unique_words << "\n";

    return 0;
}