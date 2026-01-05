cmake_minimum_required(VERSION 3.16)
project(fake_news_detector LANGUAGES CXX)

# -------------------------
# Compiler settings
# -------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fix for Windows MSBuild PowerShell bug (keep this)
set(CMAKE_VS_GLOBALS
    "UsePowerShell=false"
)

# -------------------------
# Dependencies
# -------------------------
find_package(OpenCV REQUIRED)

# -------------------------
# Include directories
# -------------------------
include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
)

# -------------------------
# Shared source files
# -------------------------
set(COMMON_SOURCES
    src/classifier.cpp
    src/vocabulary.cpp
    src/vectorizer.cpp
    src/logistic_regression.cpp
    src/text_preprocessing.cpp
)

# -------------------------
# Inference executable
# -------------------------
add_executable(fake_news
    src/main.cpp
    ${COMMON_SOURCES}
)

target_link_libraries(fake_news
    ${OpenCV_LIBS}
)

# -------------------------
# Training executable
# -------------------------
add_executable(train
    src/train.cpp
    src/dataset.cpp
    ${COMMON_SOURCES}
)

target_link_libraries(train
    ${OpenCV_LIBS}
)
