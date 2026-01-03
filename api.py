from fastapi import FastAPI
import subprocess

app = FastAPI()

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
