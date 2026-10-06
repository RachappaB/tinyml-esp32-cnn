const express = require("express");
const fs = require("fs");
const path = require("path");

const app = express();

const PORT = 3000;

// ============================================================
// FILES
// ============================================================

const CSV_FILE = path.join(__dirname, "mpu_data.csv");
const JSONL_FILE = path.join(__dirname, "mpu_raw.jsonl");

// ============================================================
// CONFIGURATION
// ============================================================

const EXPECTED_SAMPLES = 100;
const EXPECTED_SAMPLE_RATE = 50;
const EXPECTED_WINDOW_MS = 2000;

// ============================================================
// EXPRESS
// ============================================================

app.use(express.json({
  limit: "5mb"
}));

// ============================================================
// CSV HEADER
// ============================================================

const fields = [
  "device_id",
  "window_id",
  "sample_index",
  "timestamp_ms",
  "ax",
  "ay",
  "az",
  "gx",
  "gy",
  "gz"
];

const CSV_HEADER = fields.join(",") + "\n";

// Create CSV if it doesn't exist
if (!fs.existsSync(CSV_FILE)) {
  fs.writeFileSync(CSV_FILE, CSV_HEADER);
}

// Create JSONL if it doesn't exist
if (!fs.existsSync(JSONL_FILE)) {
  fs.writeFileSync(JSONL_FILE, "");
}

// ============================================================
// CSV ESCAPE
// ============================================================

function csvEscape(value) {

  const str = String(value ?? "");

  if (
    str.includes(",") ||
    str.includes('"') ||
    str.includes("\n")
  ) {
    return `"${str.replace(/"/g, '""')}"`;
  }

  return str;
}

// ============================================================
// VALIDATE NUMBER
// ============================================================

function isValidNumber(value) {

  return (
    value !== undefined &&
    value !== null &&
    Number.isFinite(Number(value))
  );
}

// ============================================================
// POST /api/mpu
// ============================================================

app.post("/api/mpu", (req, res) => {

  try {

    const {
      device_id,
      window_id,
      sample_rate,
      window_duration_ms,
      samples
    } = req.body;

    // ========================================================
    // DEVICE ID
    // ========================================================

    if (!device_id) {

      return res.status(400).json({
        success: false,
        error: "device_id is required"
      });
    }

    // ========================================================
    // WINDOW ID
    // ========================================================

    if (!window_id) {

      return res.status(400).json({
        success: false,
        error: "window_id is required"
      });
    }

    // ========================================================
    // SAMPLE RATE
    // ========================================================

    if (
      !isValidNumber(sample_rate) ||
      Number(sample_rate) <= 0
    ) {

      return res.status(400).json({
        success: false,
        error: "sample_rate must be a positive number"
      });
    }

    // ========================================================
    // WINDOW DURATION
    // ========================================================

    if (
      !isValidNumber(window_duration_ms) ||
      Number(window_duration_ms) <= 0
    ) {

      return res.status(400).json({
        success: false,
        error: "window_duration_ms must be a positive number"
      });
    }

    // ========================================================
    // SAMPLES ARRAY
    // ========================================================

    if (!Array.isArray(samples)) {

      return res.status(400).json({
        success: false,
        error: "samples must be an array"
      });
    }

    // ========================================================
    // EXACTLY 100 SAMPLES
    // ========================================================

    if (samples.length !== EXPECTED_SAMPLES) {

      return res.status(400).json({
        success: false,
        error:
          `Exactly ${EXPECTED_SAMPLES} samples are required. ` +
          `Received ${samples.length}`
      });
    }

    // ========================================================
    // VALIDATE EACH SAMPLE
    // ========================================================

    const requiredFields = [
      "timestamp_ms",
      "ax",
      "ay",
      "az",
      "gx",
      "gy",
      "gz"
    ];

    for (let i = 0; i < samples.length; i++) {

      const sample = samples[i];

      for (const field of requiredFields) {

        if (!isValidNumber(sample[field])) {

          return res.status(400).json({
            success: false,
            error:
              `${field} in sample ${i + 1} ` +
              `must be a number`
          });
        }
      }
    }

    // ========================================================
    // SERVER RECEIVED TIME
    // ========================================================

    const serverTimestamp =
      new Date().toISOString();

    // ========================================================
    // BUILD CSV DATA
    // ========================================================

    let csvData = "";

    for (let i = 0; i < samples.length; i++) {

      const sample = samples[i];

      const row = [
        device_id,
        window_id,
        i,
        sample.timestamp_ms,
        sample.ax,
        sample.ay,
        sample.az,
        sample.gx,
        sample.gy,
        sample.gz
      ];

      csvData +=
        row.map(csvEscape).join(",") +
        "\n";
    }

    // ========================================================
    // SAVE CSV
    // ========================================================

    fs.appendFileSync(
      CSV_FILE,
      csvData
    );

    // ========================================================
    // SAVE RAW JSONL
    // ========================================================

    const rawRecord = {

      server_timestamp: serverTimestamp,

      device_id,

      window_id,

      sample_rate,

      window_duration_ms,

      samples
    };

    fs.appendFileSync(
      JSONL_FILE,
      JSON.stringify(rawRecord) + "\n"
    );

    // ========================================================
    // LOG
    // ========================================================

    console.log(
      `[${serverTimestamp}] ` +
      `Received window ${window_id} ` +
      `from ${device_id} ` +
      `(${samples.length} samples)`
    );

    // ========================================================
    // RESPONSE
    // ========================================================

    return res.status(200).json({

      success: true,

      message: "MPU window saved successfully",

      device_id,

      window_id,

      samples_saved: samples.length,

      sample_rate,

      window_duration_ms,

      server_timestamp: serverTimestamp
    });

  } catch (error) {

    console.error(
      "Server error:",
      error
    );

    return res.status(500).json({

      success: false,

      error: "Internal server error"
    });
  }
});

// ============================================================
// TEST ROUTE
// ============================================================

app.get("/", (req, res) => {

  res.json({

    server: "ESP32 MPU ML Data Server",

    status: "running",

    endpoint: "/api/mpu",

    sample_rate: EXPECTED_SAMPLE_RATE,

    window_duration_ms:
      EXPECTED_WINDOW_MS,

    samples_per_window:
      EXPECTED_SAMPLES
  });
});

// ============================================================
// DATASET INFO
// ============================================================

app.get("/api/mpu/info", (req, res) => {

  let csvSize = 0;
  let jsonlSize = 0;

  try {

    if (fs.existsSync(CSV_FILE)) {

      csvSize =
        fs.statSync(CSV_FILE).size;
    }

    if (fs.existsSync(JSONL_FILE)) {

      jsonlSize =
        fs.statSync(JSONL_FILE).size;
    }

  } catch (error) {

    console.error(error);
  }

  res.json({

    csv_file: CSV_FILE,

    jsonl_file: JSONL_FILE,

    csv_size_bytes: csvSize,

    jsonl_size_bytes: jsonlSize,

    expected_sample_rate:
      EXPECTED_SAMPLE_RATE,

    expected_window_duration_ms:
      EXPECTED_WINDOW_MS,

    expected_samples_per_window:
      EXPECTED_SAMPLES
  });
});

// ============================================================
// START SERVER
// ============================================================

app.listen(
  PORT,
  "0.0.0.0",
  () => {

    console.log(
      "=========================================="
    );

    console.log(
      "ESP32 MPU ML SERVER"
    );

    console.log(
      "=========================================="
    );

    console.log(
      `Server running on port ${PORT}`
    );

    console.log(
      `POST endpoint: http://0.0.0.0:${PORT}/api/mpu`
    );

    console.log(
      `CSV: ${CSV_FILE}`
    );

    console.log(
      `JSONL: ${JSONL_FILE}`
    );

    console.log(
      `Sample rate: ${EXPECTED_SAMPLE_RATE} Hz`
    );

    console.log(
      `Window: ${EXPECTED_WINDOW_MS} ms`
    );

    console.log(
      `Samples/window: ${EXPECTED_SAMPLES}`
    );

    console.log(
      "=========================================="
    );
  }
);