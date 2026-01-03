from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
import subprocess

app = FastAPI()

# Allow frontend access
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def health():
    return {"status": "ok"}

@app.post("/predict")
def predict(headline: str):
    result = subprocess.run(
        ["./fake_news", headline],
        capture_output=True,
        text=True
    )

    output = result.stdout.strip().split()
    return {
        "label": output[0],
        "confidence": float(output[1])
    }
