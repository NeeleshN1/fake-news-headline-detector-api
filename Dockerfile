FROM ubuntu:22.04

# Install system dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Build C++ backend
RUN cmake -S . -B build \
    && cmake --build build --config Release \
    && cp build/fake_news ./fake_news

# Install Python dependencies
RUN pip3 install fastapi uvicorn

# Expose API port
EXPOSE 8000

# Start FastAPI
CMD ["uvicorn", "api:app", "--host", "0.0.0.0", "--port", "8000"]
