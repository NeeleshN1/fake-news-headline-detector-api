FROM ubuntu:22.04

# Install system + OpenCV dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    python3 \
    python3-pip \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Build C++ backend
RUN cmake -S . -B build \
    && cmake --build build \
    && find build -type f -executable -name "fake_news" -exec cp {} ./fake_news \;

# Install Python deps
RUN pip3 install fastapi uvicorn

EXPOSE 8000

CMD ["uvicorn", "api:app", "--host", "0.0.0.0", "--port", "8000"]
