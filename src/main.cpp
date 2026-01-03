#include <iostream>
#include <string>

#include "classifier.hpp"

int main(int argc, char* argv[])
{
    // ================================
    // NON-INTERACTIVE MODE (API / Docker)
    // ================================
    if (argc > 1)
    {
        std::string headline = argv[1];

        int label = classify_headline(headline);
        double prob = confidence_headline(headline);

        if (label == 1)
        {
            std::cout << "FAKE " << prob << std::endl;
        }
        else if (label == 0)
        {
            std::cout << "REAL " << prob << std::endl;
        }
        else
        {
            std::cout << "UNDETERMINED " << prob << std::endl;
        }

        return 0; // IMPORTANT: exit immediately
    }

    // ================================
    // INTERACTIVE MODE (LOCAL TESTING)
    // ================================
    std::cout << "Fake News Headline Detector (Native)\n";
    std::cout << "----------------------------------\n";
    std::cout << "Enter a headline (empty line to quit):\n";

    std::string input;
    std::getline(std::cin >> std::ws, input);

    while (!input.empty())
    {
        int label = classify_headline(input);
        double prob = confidence_headline(input);

        std::cout << "\nPrediction: ";
        if (label == 1)
        {
            std::cout << "FAKE";
        }
        else if (label == 0)
        {
            std::cout << "REAL";
        }
        else
        {
            std::cout << "UNDETERMINED";
        }

        std::cout << "\nConfidence: " << prob << "\n\n";
        std::cout << "Enter another headline:\n";

        std::getline(std::cin, input);
    }

    return 0;
}
