#include "classifier.hpp"

#include "text_preprocessing.hpp"
#include "vocabulary.hpp"
#include "vectorizer.hpp"
#include "logistic_regression.hpp"

#include <mutex>
#include <stdexcept>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif

static Vocabulary vocab;
static LogisticRegression* model = nullptr;
static std::once_flag initFlag;

// Confidence thresholds
static constexpr double FAKE_THRESHOLD = 0.7;
static constexpr double REAL_THRESHOLD = 0.3;

static void initialize()
{
#ifdef __EMSCRIPTEN__
    const std::string vocabPath = "/model/vocab.txt";
    const std::string modelPath = "/model/model.bin";
#else
    // Native: be robust to running from repo root OR build/Debug
    std::string vocabPath = "model/vocab.txt";
    std::string modelPath = "model/model.bin";

    try
    {
        vocab = loadVocabulary(vocabPath);
    }
    catch (const std::exception&)
    {
        // Fallback: if you're running from inside build/, try going up one level
        vocabPath = "../model/vocab.txt";
        modelPath = "../model/model.bin";
        vocab = loadVocabulary(vocabPath);
    }
#endif

#ifdef __EMSCRIPTEN__
    vocab = loadVocabulary(vocabPath);
#endif

    model = new LogisticRegression(vocab.size());

#ifdef __EMSCRIPTEN__
    model->load(modelPath);
#else
    try
    {
        model->load(modelPath);
    }
    catch (const std::exception&)
    {
        // If vocab fallback succeeded, modelPath already points to ../model/model.bin
        // If vocab loaded from model/, modelPath is model/model.bin
        // If model load still fails, let it throw (real error).
        throw;
    }
#endif
}

int classify_headline(const std::string& headline)
{
    std::call_once(initFlag, initialize);

    auto tokens = preprocessHeadline(headline);
    auto vec = vectorizeHeadline(tokens, vocab, vocab.size());

    double sum = 0.0;
    for (double v : vec) sum += v;

#ifdef __EMSCRIPTEN__
    emscripten::val::global("console").call<void>(
        "log",
        std::string("Vector sum: ") + std::to_string(sum)
    );
#else
    // Optional native debug (comment out if you don't want logs)
    // std::cerr << "Vector sum: " << sum << "\n";
    (void)sum; // suppress unused warning if debug line is off
#endif

    double prob = model->predictProbability(vec);

    if (prob >= FAKE_THRESHOLD) return 1;
    if (prob <= REAL_THRESHOLD) return 0;
    return -1;
}

double confidence_headline(const std::string& headline)
{
    std::call_once(initFlag, initialize);

    auto tokens = preprocessHeadline(headline);
    auto vec = vectorizeHeadline(tokens, vocab, vocab.size());

    return model->predictProbability(vec);
}