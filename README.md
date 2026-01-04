# NLP Fake News Headline Detector — Backend

This repository contains the **C++ Machine-Learning Backend** for a fake-news headline analysis system, exposed via a FastAPI service and deployed using Docker.

The backend focuses on **efficient inference and clear probabilistic outputs**, rather than definitive truth classification.

---

## Overview

The system analyzes the **linguistic structure** of a news headline and outputs a continuous *fake-likelihood score* between 0 and 1:

- `0` → more similar to real-news headline patterns  
- `1` → more similar to fake-news headline patterns  

This score is intended to be interpreted as a **signal**, not a factual judgment.

---

## Architecture

- **Model:** Logistic Regression (C++)
- **Features:** Bag-of-words vocabulary
- **Inference:** Native C++ for performance and control
- **API Layer:** FastAPI (Python)
- **Deployment:** Docker + Render

The C++ model is compiled into a native binary and invoked by the API layer for inference.

---

Built and maintained by **Neelesh Nayak**.



