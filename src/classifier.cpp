// Headline classification and Confidence results
// This module is responsible for:
// 1) Converting the raw news headlines into feature vectors
// 2) Producing fake news headline likelihood score

#include "classifier.hpp"

#include "text_preprocessing.hpp"
#include "vocabulary.hpp"
#include "vectorizer.hpp"
#include "logistic_regression.hpp"

#include <mutex>
#include <stdexcept>
#include <string>

// Global model state
static Vocabulary vocab{};
static LogisticRegression* model{nullptr};
static std::once_flag initFlag{};

// Confidence thresholds
static constexpr double FAKE_THRESHOLD{0.7};
static constexpr double REAL_THRESHOLD{0.3};

static void initialize()
{
    // Default paths (running from repo root)
    std::string vocabPath{"model/vocab.txt"};
    std::string modelPath{"model/model.bin"};

    try
    {
        vocab = loadVocabulary(vocabPath);
    }
    catch (const std::exception&)
    {
        // Fallback: running from build/ directory
        vocabPath = "../model/vocab.txt";
        modelPath = "../model/model.bin";
        vocab = loadVocabulary(vocabPath);
    }

    model = new LogisticRegression(vocab.size());

    try
    {
        model->load(modelPath);
    }
    catch (const std::exception&)
    {
        // Propagate real loading errors
        throw;
    }
}

int classify_headline(const std::string& headline)
{
    std::call_once(initFlag, initialize);

    auto tokens{preprocessHeadline(headline)};
    auto vec{vectorizeHeadline(tokens, vocab, vocab.size())};

    double prob{model->predictProbability(vec)};

    if (prob >= FAKE_THRESHOLD)
    {
        return 1;
    }
    if (prob <= REAL_THRESHOLD)
    {
        return 0;
    }

    return -1;
}

double confidence_headline(const std::string& headline)
{
    std::call_once(initFlag, initialize);

    auto tokens{preprocessHeadline(headline)};
    auto vec{vectorizeHeadline(tokens, vocab, vocab.size())};

    return model->predictProbability(vec);
}
