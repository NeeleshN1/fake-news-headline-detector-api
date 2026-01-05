"use client";

import { useState } from "react";
import { loadFakeNewsModule } from "../lib/wasm";

type Label = "REAL" | "FAKE" | "UNDETERMINED";

type Result = {
  headline: string;
  label: Label;
  probFake: number; // raw model output
};

export default function Home() {
  const [input, setInput] = useState("");
  const [results, setResults] = useState<Result[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState("");

  const runInference = async () => {
    setError("");
    setResults([]);
    setLoading(true);

    try {
      const headlines = input
        .split("\n")
        .map(h => h.trim())
        .filter(Boolean);

      if (headlines.length === 0) {
        setError("Enter at least one headline.");
        setLoading(false);
        return;
      }

      const wasm = await loadFakeNewsModule();

      const outputs: Result[] = headlines.map(headline => {
        const cls: number = wasm.classify(headline);       // -1, 0, 1
        const probFake: number = wasm.confidence(headline); // P(fake)

        let label: Label = "UNDETERMINED";
        if (cls === 1) label = "FAKE";
        else if (cls === 0) label = "REAL";

        return { headline, label, probFake };
      });

      setResults(outputs);
    } catch (e) {
      console.error(e);
      setError("WASM inference failed. See console.");
    }

    setLoading(false);
  };

  return (
    <main style={{ maxWidth: 800, margin: "40px auto", padding: 20 }}>
      <h1>Fake News Headline Detector</h1>

      <textarea
        rows={6}
        placeholder="Enter one headline per line"
        value={input}
        onChange={e => setInput(e.target.value)}
        style={{ width: "100%", padding: 12, fontSize: 16 }}
      />

      <button
        onClick={runInference}
        disabled={loading}
        style={{ marginTop: 12, padding: "10px 16px", fontSize: 16 }}
      >
        {loading ? "Running..." : "Analyze Headlines"}
      </button>

      {error && <p style={{ color: "red" }}>{error}</p>}

      <div style={{ marginTop: 24 }}>
        {results.map((r, i) => {
          const confidence =
            r.label === "FAKE"
              ? r.probFake
              : r.label === "REAL"
              ? 1 - r.probFake
              : 0.5;

          return (
            <div
              key={i}
              style={{
                border: "1px solid #ddd",
                borderRadius: 8,
                padding: 12,
                marginBottom: 12,
              }}
            >
              <p><strong>{r.headline}</strong></p>

              <span
                style={{
                  padding: "4px 8px",
                  borderRadius: 4,
                  fontWeight: 600,
                  background:
                    r.label === "REAL"
                      ? "#d4f8d4"
                      : r.label === "FAKE"
                      ? "#f8d4d4"
                      : "#e0e0e0",
                }}
              >
                {r.label}
              </span>

              <div
                style={{
                  marginTop: 8,
                  height: 10,
                  background: "#eee",
                  borderRadius: 4,
                  overflow: "hidden",
                }}
              >
                <div
                  style={{
                    height: "100%",
                    width: `${Math.round(confidence * 100)}%`,
                    background:
                      r.label === "REAL"
                        ? "green"
                        : r.label === "FAKE"
                        ? "red"
                        : "gray",
                  }}
                />
              </div>

              <small>
                {r.label === "UNDETERMINED"
                  ? `Model uncertain (P(fake) ≈ ${r.probFake.toFixed(3)})`
                  : `Confidence: ${confidence.toFixed(3)}`}
              </small>
            </div>
          );
        })}
      </div>
    </main>
  );
}
