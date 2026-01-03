#include "vectorizer.hpp"
#include "vocabulary.hpp"

std::vector<double>
vectorizeHeadline(const std::vector<std::string>& tokens,
                  const Vocabulary& vocab,
                  size_t vocabSize)
{
    // Dense BoW vector (can switch to sparse later)
    std::vector<double> vec(vocabSize, 0.0);

    for (const auto& word : tokens)
    {
        auto it = vocab.find(word);
        if (it != vocab.end())
        {
            vec[it->second] += 1.0;
        }
    }

    return vec;
}
